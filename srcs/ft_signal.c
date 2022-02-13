/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:52:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/13 18:38:04 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	sig_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		exit_num_set(130);
	}
}

void	sig_execve(int sig)
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

void	sig_basic(int sig)
{
	exit_num_set(1);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	ft_signal(int sig_flag)
{
	if (sig_flag == BASIC)
	{
		signal(SIGINT, sig_basic);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTERM, SIG_DFL);
	}
	else if (sig_flag == EXECVE)
	{
		signal(SIGINT, sig_execve);
		signal(SIGQUIT, sig_execve);
		signal(SIGTERM, SIG_DFL);
	}
	else if (sig_flag == HEREDOC)
	{
		signal(SIGINT, sig_heredoc);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTERM, SIG_DFL);
	}
}

/*

1. 기본 (부모프로세스)
	c: 새로운 프롬프트, 1
	\: SIG_IGN, 0
	d: 프로세스 종료, 0

2. execve (자식프로세스)
	c: 새로운 프롬프트, 130
	\: 프로세스 종료, 출력: Quit 3, 131
	d: 프로세스 종료, 0

3. heredoc (부모프로세스)
	c: 프로세스 종료, 130
	\: SIG_IGN, 0
	d: 프로세스 종료, 0

*/