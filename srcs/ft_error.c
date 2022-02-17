/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 10:42:11 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/17 11:46:32 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	error_symbol(char symbol, int exit_num)
{
	//printf("minishell: syntax error near unexpected token `%c'\n", symbol);
	write(2, "minishell: syntax error near unexpected token `", 48);
	write(2, &symbol, ft_strlen(&symbol));
	write(2, "'\n", 3);
	exit_num_set(exit_num);
}

void	error_2(char *cmd, char *argv, char *msg, int exit_num)
{
	//printf("minishell: %s: %s: %s\n", cmd, argv, msg);
	write(2, "minishell: ", 12);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, argv, ft_strlen(argv));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit_num_set(exit_num);
}

void	error_1(char *cmd, char *msg, int exit_num)
{
	//printf("minishell: %s: %s\n", cmd, msg);
	write(2, "minishell: ", 12);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit_num_set(exit_num);
}

void	ft_error(char *msg, int exit_num)
{
	//printf("minishell: %s\n", msg);
	write(2, "minishell: ", 12);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit_num_set(exit_num);
}
