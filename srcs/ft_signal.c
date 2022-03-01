/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:52:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/01 16:26:11 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	sig_func(int signum)
{
	if (signum == SIGINT)
	{
		if (g_sig->type == BASIC)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 1);
			rl_redisplay();
			g_sig->exitnum = 1;
		}
		else if (g_sig->type == HEREDOC)
		{
			ioctl(STDIN_FILENO, TIOCSTI, "\n");
			g_sig->signum = SIGINT;
			g_sig->exitnum = 1;
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
	else if (g_sig->type == EXECVE)
		signal(SIGQUIT, sig_func);
}

/*

1. 기본, heredoc (부모프로세스)
	c: 새로운 프롬프트, 1
	\: SIG_IGN, 0
	d: 프로세스 종료, 0

2. execve (자식프로세스)
	c: 새로운 프롬프트, 130
	\: 프로세스 종료, 출력: Quit 3, 131
	d: 프로세스 종료, 0

*/