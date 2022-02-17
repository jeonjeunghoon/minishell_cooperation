/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 15:52:13 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/17 11:23:20 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	check_argv(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
			return (ERROR);
		i++;
	}
	return (0);
}

int	exit_exception(int argc, char **argv)
{
	if (argc == 1)
		return (0);
	else if (argc > 2)
	{
		error_1("exit", "too many arguments", 1);
		return (ERROR);
	}
	else if (argc == 2)
	{
		if (check_argv(argv[1]) == ERROR)
		{
			error_2("exit", argv[1], "numeric argument required", 255);
			return (ERROR);
		}
	}
	return (0);
}

void	ft_exit(t_argv *argv)
{
	int	argc;
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
		if (!(argv->is_pipe || argv->was_pipe))
			exit(g_exit_state);
	}
	else if (pid == 0)
	{
		if (!argv->is_or)
		{
			error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(error_fd, 2);
			close(error_fd);
		}
		argc = 0;
		argc = ft_two_dimension_size(argv->argv);
		if (exit_exception(argc, argv->argv) == ERROR)
			exit(g_exit_state);
		if (!(argv->is_pipe || argv->was_pipe))
			printf("logout\n");
		if (argc == 2)
			exit_num_set(ft_atoi(argv->argv[1]));
		exit(g_exit_state);
	}
}
