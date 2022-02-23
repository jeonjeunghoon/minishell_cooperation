/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utility2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 00:37:13 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/23 13:31:26 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_envname_parse(char *str, int *i)
{
	char	*name;
	int		len;
	int		j;

	len = (*i);
	while (str[len] && str[len] != '$' && \
			str[len] != '\"' && str[len] != '\'' && \
			str[len] != '/' && str[len] != '=' && \
			str[len] != ' ')
		len++;
	len = len - (*i);
	name = (char *)malloc(sizeof(char) * (len + 1));
	name[len] = '\0';
	j = 0;
	while (j < len && str[*i] && str[*i] != '$' && \
			str[*i] != '\"' && str[*i] != '\'' && \
			str[len] != '=' && str[len] != ' ')
	{
		name[j] = str[*i];
		(*i)++;
		j++;
	}
	return (name);
}

void	env_str(t_refine *refine)
{
	int		len;
	int		i;
	char	*tmp;

	tmp = ft_strjoin(refine->new_str, refine->env);
	ft_free(&refine->new_str);
	refine->new_str = tmp;
	refine->j = ft_strlen(refine->new_str);
}

t_bool	stream_condition(char c)
{
	if (c != '|' && c != '>' && c != '<' && c != '&')
		return (FALSE);
	return (TRUE);
}

t_bool	str_condition(char c, t_token *token)
{
	if (c == '|' || c == '>' || c == '<' || c == '&')
		return (FALSE);
	exception_utility(c, &(token->single_quote), &(token->double_quote));
	if (token->single_quote == FALSE && token->double_quote == FALSE && \
		c == ' ')
		return (FALSE);
	return (TRUE);
}
