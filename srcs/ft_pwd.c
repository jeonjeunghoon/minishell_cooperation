/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:57:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 16:57:14 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

void	ft_pwd(t_argv *argv)
{
	char	*cwd;
	int		stat_loc;
	pid_t	pid;

	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, WUNTRACED);
		pipe_tmp_copy(argv);
		exit_num_set(stat_loc & 127);
	}
	else if (pid == 0)
	{
		cwd = getcwd(NULL, 0);
		when_there_is_pipe(argv);
		if (cwd)
			printf("%s\n", cwd);
		else if (cwd == NULL)
			error_2(argv->argv[0], argv->argv[1], strerror(errno), 1);
		free(cwd);
		exit(g_exit_state);
	}
}
