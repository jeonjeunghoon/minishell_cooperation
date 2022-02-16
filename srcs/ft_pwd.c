/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:57:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/16 14:43:12 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

void	ft_pwd(t_argv *argv)
{
	char	*cwd;
	int		stat_loc;
	pid_t	pid;
	int		redirect_fd[2];
	int		error_fd;

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
		cwd = getcwd(NULL, 0);
		error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(error_fd, 2);
		close(error_fd);
		when_there_is_pipe(argv);
		if (cwd)
			printf("%s\n", cwd);
		else if (cwd == NULL)
		{
			error_2(argv->argv[0], argv->argv[1], strerror(errno), 1);
			free(cwd);
		}
		free(cwd);
		cwd = NULL;
		exit(g_exit_state);
	}
}
