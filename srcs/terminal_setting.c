/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_setting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 14:27:05 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/01 14:38:17 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	terminal_setting_on(t_mini *mini)
{
	if (tcgetattr(STDIN_FILENO, &(mini->term)) == -1)
		return (ERROR);
	(mini->term).c_lflag &= ~(ECHOCTL);
	(mini->term).c_cc[VMIN] = 1;
	(mini->term).c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(mini->term)) == -1)
		return (ERROR);
	return (0);
}

int	terminal_setting_off(t_mini *mini)
{
	// if (tcgetattr(STDIN_FILENO, &(mini->term)) == -1)
	// 	return (ERROR);
	// (mini->term).c_lflag = 0;
	// (mini->term).c_cc[VMIN] = 1;
	// (mini->term).c_cc[VTIME] = 0;
	// if (tcsetattr(STDIN_FILENO, TCSANOW, &(mini->term)) == -1)
	// 	return (ERROR);
	return (0);
}
