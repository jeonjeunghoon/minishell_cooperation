/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:52:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/02 18:10:19 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	sig_func(int signum)
{
	if (signum == SIGINT)
	{
		if (g_sig->type == BASIC || g_sig->type == HEREDOC)
		{
			if (g_sig->type == BASIC)
			{
				printf("\n");
				rl_on_new_line();
				rl_replace_line("", 1);
				rl_redisplay();
				g_sig->exitnum = 1;
			}
			else
			{
				ioctl(STDIN_FILENO, TIOCSTI, "\n");
				g_sig->signum = SIGINT;
				g_sig->exitnum = 1;
			}
		}
		else
		{
			printf("\n");
			g_sig->exitnum = 130;
		}
	}
	else if (signum == SIGQUIT)
	{
		if (g_sig->type == EXECVE)
		{
			printf("Quit: 3\n");
			g_sig->exitnum = 131;
		}
	}
}

void	ft_signal(void)
{
	if (g_sig->type == BASIC)
		signal(SIGINT, sig_func);
	else if (g_sig->type == HEREDOC)
		signal(SIGINT, sig_func);
	else if (g_sig->type == EXECVE)
		signal(SIGINT, sig_func);
	if (g_sig->type == BASIC || g_sig->type == HEREDOC)
		signal(SIGQUIT, SIG_IGN);
	else
		signal(SIGQUIT, sig_func);
}
