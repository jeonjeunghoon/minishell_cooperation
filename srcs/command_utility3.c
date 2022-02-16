/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 22:18:14 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/16 22:21:03 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	close_original_fd(t_argv *argv, int *original_fd)
{
	if (argv->is_pipe || argv->is_redirect)
	{
		dup2(original_fd[0], STDIN_FILENO);
		dup2(original_fd[1], STDOUT_FILENO);
		close(original_fd[0]);
		close(original_fd[1]);
	}
}

void	set_original_fd(t_argv *argv, int *original_fd)
{
	if (argv->is_pipe || argv->is_redirect)
	{
		pipe(original_fd);
		dup2(STDIN_FILENO, original_fd[0]);
		dup2(STDOUT_FILENO, original_fd[1]);
	}
}
