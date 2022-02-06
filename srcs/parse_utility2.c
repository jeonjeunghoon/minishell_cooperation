/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utility2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:16:26 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/01/27 15:49:22 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exception_utility(char c, t_bool *sin, t_bool *dou)
{
	if (c == '\'')
	{
		if (*sin == FALSE && *dou == FALSE)
			*sin = TRUE;
		else if (*sin == TRUE && *dou == FALSE)
			*sin = FALSE;
	}
	if (c == '\"')
	{
		if (*dou == FALSE && *sin == FALSE)
			*dou = TRUE;
		else if (*dou == TRUE && *sin == FALSE)
			*dou = FALSE;
	}
}

void	argv_lst_init(t_argv **str, t_argv **stream, int *size)
{
	*size = 0;
	*str = NULL;
	*stream = NULL;
}
