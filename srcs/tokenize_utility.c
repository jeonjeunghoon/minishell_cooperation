/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:51:19 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/23 21:21:40 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	basic_str(t_refine *refine)
{
	char	*ptr;
	int		len;
	int		i;

	len = ft_strlen(refine->new_str) + 1;
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	ptr[len] = '\0';
	i = 0;
	while (i < len - 1)
	{
		ptr[i] = refine->new_str[i];
		i++;
	}
	ptr[i] = refine->str[refine->i];
	ft_free(&refine->new_str);
	refine->new_str = ptr;
	refine->j = i + 1;
	refine->i++;
}

void	dollar_str(t_refine *refine)
{
	if (refine->str[refine->i + 1] == '\0')
		basic_str(refine);
	else if ((refine->str[refine->i + 1] == '\'' || \
			refine->str[refine->i + 1] == '\"') && refine->is_basic == TRUE)
		refine->i++;
	else if (refine->str[refine->i + 1] == '?')
		exitnum_str(refine);
	else if (refine->str[refine->i + 1] >= '0' && refine->str[refine->i + 1] <= '9')
	{
		if (refine->str[refine->i + 1] == '0')
		{
			char *ptr;

			ptr = ft_strjoin(refine->new_str, "-minishell");
			ft_free(&refine->new_str);
			refine->new_str = ptr;
			refine->j = ft_strlen(refine->new_str);
		}
		refine->i += 2;
	}
	else
	{
		refine->i++;
		refine->name = get_envname_parse(refine->str, &refine->i);
		refine->env = ft_getenv(refine->envp, refine->name);
		ft_free(&refine->name);
		if (refine->env == NULL)
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
	while (refine->str[refine->i] != '\"' && refine->str[refine->i] != '\0')
	{
		if (refine->str[refine->i] == '$')
			dollar_str(refine);
		else
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
	while (refine->str[refine->i] != '\'' && refine->str[refine->i] != '\0')
	{
		if (num % 2 == 0 && refine->str[refine->i] == '$')
			dollar_str(refine);
		else
			basic_str(refine);
	}
	while (refine->str[refine->i] == '\'' && refine->str[refine->i] != '\0')
		refine->i++;
	refine->is_single = FALSE;
	refine->is_basic = TRUE;
}

void	swung_dash_str(t_refine *refine)
{
	if ((refine->str[refine->i + 1] == '\0' || refine->str[refine->i + 1] == '/') && \
		refine->i == 0)
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

int	create_refined_str(t_refine *refine)
{
	int	len;
	int	i;

	len = ft_strlen(refine->str);
	refine->new_str = (char *)malloc(sizeof(char) * (len + 1));
	refine->new_str[len] = '\0';
	i = 0;
	while (i < len)
		refine->new_str[i++] = '\0';
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
	if (refine->new_str[0] == '\0')
		return (ERROR);
	return (0);
}
