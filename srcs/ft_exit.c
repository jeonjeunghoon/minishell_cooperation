/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 15:52:13 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 16:54:25 by jeunjeon         ###   ########.fr       */
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
	if (argc >= 2)
	{
		if (check_argv(argv[1]) == ERROR)
			error_2("exit", argv[1], "numeric argument required", 255);
		else
			error_1("exit", "too many arguments", 1);
		return (ERROR);
	}
	return (0);
}

void	ft_exit(t_argv *argv)
{
	int	argc;
	int		stat_loc;
	pid_t	pid;

	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, WUNTRACED);
		pipe_tmp_copy(argv);
		exit_num_set(stat_loc & 127);
		if (!(argv->is_pipe || argv->was_pipe))
			exit(g_exit_state);
	}
	else if (pid == 0)
	{
		when_there_is_pipe(argv);
		argc = 0;
		argc = ft_two_dimension_size(argv->argv);
		if (exit_exception(argc, argv->argv) == ERROR)
			exit(g_exit_state);
		printf("logout\n");
		if (argc == 2 && check_argv(argv->argv[1]) == 0)
			exit_num_set(ft_atoi(argv->argv[1]));
		else
			exit_num_set(EXIT_SUCCESS);
		exit(g_exit_state);
	}
}
