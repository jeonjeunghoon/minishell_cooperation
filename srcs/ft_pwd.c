/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:57:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/12 22:29:11 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

void	ft_pwd(t_argv *argv)
{
	char	*cwd;
	int		stat_loc;
	pid_t	pid;

	exit_num_set(0);
	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, 0x00000002);
		pipe_tmp_copy(argv);
		exit_num_set(ft_wexitstatus(stat_loc));
	}
	else if (pid == 0)
	{
		cwd = getcwd(NULL, 0);
		when_there_is_pipe(argv);
		if (set_redirect(argv) == ERROR)
			exit(g_exit_state);
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
