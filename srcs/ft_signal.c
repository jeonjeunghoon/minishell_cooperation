/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:52:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/06 18:09:01 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		exit_num_set(1);
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		exit_num_set(130);
		printf("\n");
	}
	else if (sig == SIGQUIT)
	{
		exit_num_set(131);
		printf("Quit: 3\n");
	}
}

void	ft_signal(t_bool *sig_flag)
{
	if (*sig_flag == FALSE)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (*sig_flag == TRUE)
	{
		*sig_flag = FALSE;
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
	}
}
