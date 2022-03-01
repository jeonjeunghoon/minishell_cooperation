/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utility3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 17:56:11 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/26 14:52:52 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exitnum_str(t_mini *mini, t_refine *refine)
{
	char	*exitnum;
	char	*tmp;
	int		len;

	exitnum = ft_itoa(mini->sig->exitnum);
	refine->new_str[refine->j] = '\0';
	tmp = ft_strjoin(refine->new_str, exitnum);
	ft_free(&exitnum);
	ft_free(&refine->new_str);
	refine->new_str = tmp;
	refine->i += 2;
	refine->j = ft_strlen(refine->new_str);
}
