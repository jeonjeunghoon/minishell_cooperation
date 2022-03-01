/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_setting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 14:27:05 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/01 16:26:36 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	terminal_setting_save(t_mini *mini)
{
	tcgetattr(STDIN_FILENO, &(mini->org_term));
	tcgetattr(STDOUT_FILENO, &(mini->org_term));
}

int	terminal_setting_on(t_mini *mini)
{
	if (tcgetattr(STDOUT_FILENO, &(mini->new_term)) == -1)
		return (ERROR);
	(mini->new_term).c_lflag = ~(ECHOCTL);
	(mini->new_term).c_cc[VMIN] = 1;
	(mini->new_term).c_cc[VTIME] = 0;
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &(mini->new_term)) == -1)
		return (ERROR);
	return (0);
}

void	terminal_setting_reset(t_mini *mini)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &(mini->org_term));
	tcsetattr(STDOUT_FILENO, TCSANOW, &(mini->org_term));
}
