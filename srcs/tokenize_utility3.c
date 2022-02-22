/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utility3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 17:56:11 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/22 16:23:15 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exitnum_str(t_refine *refine)
{
	char	*exit_num;
	char	*tmp;
	int		len;

	exit_num = ft_itoa(g_exit_state);
	refine->new_str[refine->j] = '\0';
	tmp = ft_strjoin(refine->new_str, exit_num);
	ft_free(&exit_num);
	ft_free(&refine->new_str);
	refine->new_str = tmp;
	refine->i += 2;
	refine->j = ft_strlen(refine->new_str);
}
