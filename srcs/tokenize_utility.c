/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:51:19 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/21 15:17:09 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	basic_str(t_refine *refine)
{
	refine->new_str[refine->j] = refine->str[refine->i];
	refine->i++;
	refine->j++;
}

void	dollar_str(t_refine *refine)
{
	if (refine->str[refine->i + 1] == '\0' || refine->str[refine->i + 1] == '\"')
		basic_str(refine);
	else if ((refine->str[refine->i + 1] == '\'' || \
			refine->str[refine->i + 1] == '\"') && refine->is_basic == TRUE)
		refine->i++;
	else if (refine->str[refine->i + 1] == '?')
		exitnum_str(refine);
	else
	{
		refine->i++;
		refine->name = get_envname_parse(refine->str, &refine->i);
		refine->env = ft_getenv(refine->envp, refine->name);
		ft_free(&refine->name);
		if (refine->env == NULL && \
			(refine->str[refine->i] != '\"' || refine->str[refine->i] != '\0'))
			return ;
		else
			env_str(refine);
		refine->env = NULL;
	}
}

void	double_quote_str(t_refine *refine)
{
	refine->is_double = TRUE;
	refine->is_basic = FALSE;
	while (refine->str[refine->i] == '\"' && refine->str[refine->i] != '\0')
		refine->i++;
	if (refine->str[refine->i] == '$')
		dollar_str(refine);
	else
	{
		while (refine->str[refine->i] && refine->str[refine->i] != '\"')
			basic_str(refine);
	}
	while (refine->str[refine->i] == '\"' && refine->str[refine->i] != '\0')
		refine->i++;
	refine->is_double = FALSE;
	refine->is_basic = TRUE;
}

void	single_quote_str(t_refine *refine)
{
	int	num;

	num = 0;
	refine->is_single = TRUE;
	refine->is_basic = FALSE;
	while (refine->str[refine->i] == '\'' && refine->str[refine->i] != '\0')
	{
		refine->i++;
		num++;
	}
	if (num % 2 == 0 && refine->str[refine->i] == '$')
	{
		dollar_str(refine);
		while (refine->str[refine->i] != '\'' && refine->str[refine->i] != '\0')
			refine->i++;
	}
	else
	{
		while (refine->str[refine->i] && refine->str[refine->i] != '\'')
			basic_str(refine);
	}
	while (refine->str[refine->i] == '\'' && refine->str[refine->i] != '\0')
		refine->i++;
	refine->is_single = FALSE;
	refine->is_basic = TRUE;
}

void	swung_dash_str(t_refine *refine)
{
	if (refine->str[refine->i + 1] == '\0' || refine->str[refine->i + 1] == '/')
	{
		refine->i++;
		refine->env = ft_getenv(refine->envp, "HOME");
		if (refine->env == NULL)
			ft_error("$Home env error", 1);
		else
			env_str(refine);
	}
	else
		basic_str(refine);
}

void	create_refined_str(t_refine *refine)
{
	int	len;

	len = ft_strlen(refine->str);
	refine->new_str = (char *)malloc(sizeof(char) * (len + 1));
	refine->new_str[len] = '\0';
	while (refine->str[refine->i])
	{
		if (refine->str[refine->i] == '~')
			swung_dash_str(refine);
		else if (refine->str[refine->i] == '\'')
			single_quote_str(refine);
		else if (refine->str[refine->i] == '\"')
			double_quote_str(refine);
		else if (refine->str[refine->i] == '$')
			dollar_str(refine);
		else
			basic_str(refine);
	}
	if (refine->str[refine->i] == '\0')
		refine->new_str[refine->j] = '\0';
}
