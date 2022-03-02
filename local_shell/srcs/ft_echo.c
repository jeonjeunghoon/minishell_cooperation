/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 15:52:03 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/02 16:41:31 by jeunjeon         ###   ########.fr       */
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

int	n_option(char **argv, int *start_ptr)
{
	int	i;

	i = 2;
	while (ft_strncmp(argv[*start_ptr], "-n", 3) == 0)
		(*start_ptr)++;
	if (*start_ptr == 1)
		return (FALSE);
	return (TRUE);
}

void	ft_echo(t_mini *mini, t_argv *argv)
{
	int		start_ptr;
	int		n_flag;
	int		error_fd;

	// if (!argv->is_or)
	// {
	// 	error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	// 	dup2(error_fd, 2);
	// 	close(error_fd);
	// }
	n_flag = FALSE;
	if (argv->argv[1] == NULL)
		write(1, "\n", 1);
	else
	{
		start_ptr = 1;
		n_flag = n_option(argv->argv, &start_ptr);
		while (argv->argv[start_ptr] != NULL)
		{
			print_msg(mini->envp, argv->argv, start_ptr, n_flag);
			start_ptr++;
		}
	}
	exit_num_set(0);
}
