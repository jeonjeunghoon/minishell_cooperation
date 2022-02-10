/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 14:14:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/10 16:37:25 by jeunjeon         ###   ########.fr       */
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

void	set_redirect(t_argv *redirect, t_argv *file)
{
	if (redirect == NULL)
		return ;
	if (redirect->is_ltor)
		ltor(redirect, file);
	else if (redirect->is_rtol)
		rtol(redirect, file);
	else if (redirect->is_append)
		append(redirect, file);
	else if (redirect->is_heredoc)
		heredoc(redirect, file);
}

void	exe_cmd(char *cmd_path, t_argv *argv, char **envp, t_argv *redirect, t_argv *file)
{
	pid_t	pid;
	int		stat_loc;

	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, 0x00000002);
		pipe_tmp_copy(argv);
		if (ft_wifexited(stat_loc) == TRUE)
			exit_num_set(ft_wstopsig(stat_loc));
	}
	else if (pid == 0)
	{
		when_there_is_pipe(argv);
		set_redirect(redirect, file);
		if (execve(cmd_path, argv->argv, envp) == -1)
		{
			printf("%s\n", strerror(errno));
			exit_num_set(1);
			exit(g_exit_state);
		}
	}
}
