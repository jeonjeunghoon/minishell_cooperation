/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 22:18:14 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/24 02:09:13 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	close_original_fd(t_mini *mini, t_argv *argv)
{
	dup2(mini->origin_fd[0], STDIN_FILENO);
	dup2(mini->origin_fd[1], STDOUT_FILENO);
	close(mini->origin_fd[0]);
	close(mini->origin_fd[1]);
}

void	set_original_fd(t_mini *mini, t_argv *argv)
{
	pipe(mini->origin_fd);
	dup2(STDIN_FILENO, mini->origin_fd[0]);
	dup2(STDOUT_FILENO, mini->origin_fd[1]);
}
