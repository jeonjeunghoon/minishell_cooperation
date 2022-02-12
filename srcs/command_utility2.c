/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 14:14:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/12 22:45:17 by jeunjeon         ###   ########.fr       */
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

int	set_redirect(t_argv *argv)
{
	int		i;

	if (argv->is_redirect == FALSE)
		return (0);
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

	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, 0x00000002);
		pipe_tmp_copy(argv);
		unlink(".heredoc_tmp");
		if (ft_wifexited(stat_loc) == TRUE)
			exit_num_set(ft_wstopsig(stat_loc));
	}
	else if (pid == 0)
	{
		when_there_is_pipe(argv);
		if (set_redirect(argv) == ERROR)
			exit(g_exit_state);
		if (execve(cmd_path, argv->argv, envp) == -1)
		{
			printf("%s\n", strerror(errno));
			exit_num_set(1);
			exit(g_exit_state);
		}
	}
}
