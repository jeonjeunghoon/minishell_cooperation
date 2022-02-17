/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:45:59 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/17 11:23:22 by seungcoh         ###   ########.fr       */
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
	int		original_fd[2];
	int		error_fd;

	exit_num_set(0);
	set_original_fd(argv, original_fd);
	when_there_is_pipe(argv);
	if (set_redirect(argv) == ERROR)
		exit(g_exit_state);
	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, 0x00000002);
		pipe_tmp_copy(argv);
		close_original_fd(argv, original_fd);
		exit_num_set(ft_wexitstatus(stat_loc));
	}
	else if (pid == 0)
	{	
		if (!argv->is_or)
		{
			error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(error_fd, 2);
			close(error_fd);
		}
		if (ft_two_dimension_size(argv->argv) != 1)
		{
			error_2(argv->argv[0], argv->argv[1], "With no options or arguments", 1);
			exit(g_exit_state);
		}
		show_env(mini->envp);
		exit(g_exit_state);
	}
}
