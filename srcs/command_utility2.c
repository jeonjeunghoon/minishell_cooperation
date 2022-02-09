/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 14:14:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 16:26:04 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_wstopsig(int stat_loc)
{
	int	x;
	int	sig;

	x = (*(int *)&(stat_loc));
	sig = x >> 8;
	return (sig);
}

t_bool	ft_wifexited(int stat_loc)
{
	int	x;
	int	status;

	x = *(int *)&(stat_loc);
	status = x & 0177;
	if (status == 0)
		return (TRUE);
	return (FALSE);
}

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

void	exe_cmd(char *cmd_path, t_argv *argv, char **envp, t_bool sig_flag)
{
	pid_t	pid;
	pid_t	child;
	int		stat_loc;
	int		fd;
	int		fd2;
	int		len;
	char	buf[1024];
 
	sig_flag = TRUE;
	ft_signal(&sig_flag);
	pid = fork();
	if (pid > 0)
	{
		child = waitpid(pid, &stat_loc, 0x00000002); // WUNTRACED
		pipe_tmp_copy(argv);
		if (ft_wifexited(stat_loc) == TRUE)
			exit_num_set(ft_wstopsig(stat_loc));
	}
	else if (pid == 0)
	{
		when_there_is_pipe(argv);
		if (execve(cmd_path, argv->argv, envp) == -1)
		{
			printf("%s\n", strerror(errno));
			exit_num_set(1);
			exit(g_exit_state);
		}
	}
}
