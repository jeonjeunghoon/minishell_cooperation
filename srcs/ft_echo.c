/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 15:52:03 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/17 11:23:24 by seungcoh         ###   ########.fr       */
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
