/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:45:59 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/13 18:41:28 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	show_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

void	ft_env(t_mini *mini, t_argv *argv)
{
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
		if (ft_two_dimension_size(argv->argv) != 1)
		{
			error_2(argv->argv[0], argv->argv[1], "With no options or arguments", 1);
			exit(g_exit_state);
		}
		show_env(mini->envp);
		exit(g_exit_state);
	}
}
