/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   w_utility.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 17:27:26 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 17:30:42 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_wexitstatus(int stat_loc)
{
	int	x;
	int	status;

	x = *(int *)&(stat_loc);
	x = x >> 8;
	status = x & 0x000000ff;
	return (status);
}

int	ft_wstopsig(int stat_loc)
{
	int	x;
	int	sig;

	x = (*(int *)&(stat_loc));
	sig = x >> 8;
	return (sig);
}

t_bool	ft_wifexited(int stat_loc)
{
	int	x;
	int	status;

	x = *(int *)&(stat_loc);
	status = x & 0177;
	if (status == 0)
		return (TRUE);
	return (FALSE);
}
