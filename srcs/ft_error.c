/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 10:42:11 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 17:23:19 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	error_symbol(char symbol, int exit_num)
{
	printf("minishell: syntax error near unexpected token `%c'\n", symbol);
	exit_num_set(exit_num);
}

void	error_1(char *cmd, char *msg, int exit_num)
{
	printf("minishell: %s: %s\n", cmd, msg);
	exit_num_set(exit_num);
}

void	error_2(char *cmd, char *argv, char *msg, int exit_num)
{
	printf("minishell: %s: %s: %s\n", cmd, argv, msg);
	exit_num_set(exit_num);
}

void	ft_error(void)
{
	printf("Minishell error !!!\n");
	exit_num_set(1);
	exit(g_exit_state);
}
