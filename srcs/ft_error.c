/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 10:42:11 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/26 14:43:45 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	error_symbol(t_mini *mini, char *symbol, int exitnum)
{
	write(2, "-minishell: syntax error near unexpected token `", 49);
	write(2, symbol, ft_strlen(symbol));
	write(2, "'\n", 3);
	exit_num_set(mini, exitnum);
}

void	error_2(t_mini *mini, char *cmd, char *argv, char *msg, int exitnum)
{
	write(2, "-minishell: ", 13);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, argv, ft_strlen(argv));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit_num_set(mini, exitnum);
}

void	error_1(t_mini *mini, char *cmd, char *msg, int exitnum)
{
	write(2, "-minishell: ", 13);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit_num_set(mini, exitnum);
}

void	ft_error(t_mini *mini, char *msg, int exitnum)
{
	write(2, "-minishell: ", 13);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit_num_set(mini, exitnum);
}
