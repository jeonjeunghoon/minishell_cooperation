/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 14:14:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/16 15:40:40 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	ft_s_isreg(int mode)
{
	if ((mode & 0170000) == 0100000)
		return (TRUE);
	return (FALSE);
}

t_bool	ft_s_isdir(int mode)
{
	if ((mode & 0170000) == 0040000)
		return (TRUE);
	return (FALSE);
}

void	remove_redirection(t_argv *argv)
{
	char	**new_argv;
	int		len;
	int		i;
	int		j;

	len = ft_two_dimension_size(argv->argv);
	i = 0;
	while (argv->argv[i])
	{
		if (argv->argv[i][0] == '>' || argv->argv[i][0] == '<')
			len -= 2;
		i++;
	}
	new_argv = (char **)malloc(sizeof(char *) * (len + 1));
	new_argv[len] = NULL;
	i = 0;
	j = 0;
	while (i < len && argv->argv[j])
	{
		if (argv->argv[j][0] == '>' || argv->argv[j][0] == '<')
			j += 2;
		new_argv[i] = ft_strdup(argv->argv[j]);
		i++;
		j++;
	}
	ft_two_dimension_free(&argv->argv);
	argv->argv = new_argv;
}

int	set_redirect(t_argv *argv, int *redirect_fd)
{
	int		i;

	if (argv->is_redirect == FALSE)
		return (0);
	pipe(redirect_fd);
	dup2(STDIN_FILENO, redirect_fd[0]);
	dup2(STDOUT_FILENO, redirect_fd[1]);
	i = 0;
	while (argv->argv[i])
	{
		if (argv->argv[i][0] == '>' && argv->argv[i][1] == '>')
		{
			append(argv->argv[i + 1]);
			i++;
		}
		else if (argv->argv[i][0] == '>')
		{
			ltor(argv->argv[i + 1]);
			i++;
		}
		else if (argv->argv[i][0] == '<' && argv->argv[i][1] == '<')
		{
			heredoc(argv->argv[i + 1]);
			i++;
		}
		else if (argv->argv[i][0] == '<')
		{
			if (rtol(argv->argv[i + 1]) == ERROR)
				return (ERROR);
			i++;
		}
		i++;
	}
	remove_redirection(argv);
	return (0);
}

void	exe_cmd(char *cmd_path, t_argv *argv, char **envp)
{
	pid_t	pid;
	int		stat_loc;
	int		redirect_fd[2];
	t_bool	sig_flag;
	int		error_fd;

	if (set_redirect(argv, &(redirect_fd[0])) == ERROR)
		return ;
	ft_signal(EXECVE);
	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, 0x00000002);
		pipe_tmp_copy(argv);
		if (argv->is_redirect == TRUE)
		{
			dup2(redirect_fd[0], STDIN_FILENO);
			dup2(redirect_fd[1], STDOUT_FILENO);
			close(redirect_fd[0]);
			close(redirect_fd[1]);
		}
		unlink(".heredoc_tmp");
		if (ft_wifexited(stat_loc) == TRUE)
			exit_num_set(ft_wstopsig(stat_loc));
	}
	else if (pid == 0)
	{
		error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(error_fd, 2);
		close(error_fd);
		when_there_is_pipe(argv);
		if (argv->argv[0][0] == '\0')
		{
			exit_num_set(0);
			exit(g_exit_state);
		}
		if (execve(cmd_path, argv->argv, envp) == -1)
		{
			if (argv->is_redirect == TRUE)
			{
				dup2(redirect_fd[0], STDIN_FILENO);
				dup2(redirect_fd[1], STDOUT_FILENO);
				close(redirect_fd[0]);
				close(redirect_fd[1]);
			}
			if (errno == 2)
				error_1(cmd_path, "command not found", 127);
			else if (errno == 13)
				error_1(cmd_path, "is a directory", 126);
			else
				ft_error(strerror(errno), 1);
			exit(g_exit_state);
		}
	}
}
