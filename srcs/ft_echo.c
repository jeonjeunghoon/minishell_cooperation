/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 15:52:03 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/13 18:43:11 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_msg(char **envp, char **argv, int start_ptr, int n_flag)
{
	char	*str;

	ft_putstr_fd(argv[start_ptr], 1);
	if (argv[start_ptr + 1] != NULL)
		write(1, " ", 1);
	if (argv[start_ptr + 1] == NULL && n_flag == FALSE)
		write(1, "\n", 1);
}

int	n_option(char *argv, int *start_ptr)
{
	int	i;

	i = 2;
	if (ft_strncmp(argv, "-n", 2) == 0)
	{
		while (argv[i] == 'n' && argv[i])
			i++;
		if (argv[i] != '\0')
			return (FALSE);
	}
	else
		return (FALSE);
	*start_ptr = 2;
	return (TRUE);
}

void	ft_echo(t_mini *mini, t_argv *argv)
{
	int		start_ptr;
	int		n_flag;
	int		stat_loc;
	pid_t	pid;
	int		redirect_fd[2];

	exit_num_set(0);
	if (set_redirect(argv, &(redirect_fd[0])) == ERROR)
		exit(g_exit_state);
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
		exit_num_set(ft_wexitstatus(stat_loc));
	}
	else if (pid == 0)
	{
		when_there_is_pipe(argv);
		n_flag = FALSE;
		if (argv->argv[1] == NULL)
			write(1, "\n", 1);
		else
		{
			start_ptr = 1;
			n_flag = n_option(argv->argv[start_ptr], &start_ptr);
			while (argv->argv[start_ptr] != NULL)
			{
				print_msg(mini->envp, argv->argv, start_ptr, n_flag);
				start_ptr++;
			}
		}
		exit(g_exit_state);
	}
}
