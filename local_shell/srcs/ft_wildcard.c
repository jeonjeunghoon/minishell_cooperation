/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:01:40 by seungcoh          #+#    #+#             */
/*   Updated: 2022/03/03 20:28:34 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_get_wild_str(t_wild *wild, t_list **wild_token)
{
	wild->i = 0;
	wild->start_idx = 0;
	*wild_token = 0;
	wild->flag = 0;
	wild->str = 0;
}

void	refine_wild_token(t_wild *wild, t_list **wild_token, \
char *token, t_mini *mini)
{
	char	*tmp;

	wild->tmp_token.token = (char *)malloc(sizeof(char) * \
	(wild->i - wild->start_idx + 1));
	ft_strlcpy(wild->tmp_token.token, token + wild->start_idx, \
	wild->i - wild->start_idx + 1);
	if (refine_str(mini, &wild->tmp_token, mini->envp) != ERROR)
	{
		ft_lstadd_back(wild_token, ft_lstnew(wild->tmp_token.token));
		if (wild->str == 0)
			wild->str = ft_strdup(wild->tmp_token.token);
		else
		{
			tmp = ft_strjoin(wild->str, wild->tmp_token.token);
			free(wild->str);
			wild->str = tmp;
		}
	}
}

t_bool	jmp_to_wild(t_wild	*wild, char *token)
{
	if (token[wild->i] == '\"')
	{
		while (token[++wild->i] != '\"')
			;
		wild->i++;
		return (1);
	}
	else if (token[wild->i] == '\'')
	{
		while (token[++wild->i] != '\'')
			;
		wild->i++;
		return (1);
	}
	else if (token[wild->i] == '$' && token[wild->i + 1] == '*')
	{
		wild->i += 2;
		return (1);
	}
	return (0);
}

void	flag_control(t_wild	*wild)
{
	if ((wild->i - wild->start_idx) && (wild->start_idx == 0))
		wild->flag |= 1;
	wild->flag |= 4;
	wild->start_idx = wild->i + 1;
}

t_list	*get_wild_str(t_mini *mini, char *token)
{
	t_wild	wild;
	t_list	*wild_token;
	char	*tmp;
	t_list	*ret;

	init_get_wild_str(&wild, &wild_token);
	while (token[wild.i])
	{
		if (!jmp_to_wild(&wild, token))
		{
			if (token[wild.i] == '*')
			{
				if (wild.i - wild.start_idx)
					refine_wild_token(&wild, &wild_token, token, mini);
				flag_control(&wild);
				tmp = ft_strjoin(wild.str, "*");
				free(wild.str);
				wild.str = tmp;
			}
			wild.i++;
		}
	}
	if (wild.start_idx && wild.i - wild.start_idx)
		refine_wild_token(&wild, &wild_token, token, mini);
	if (wild.start_idx && wild.i - wild.start_idx)
		wild.flag |= 2;
	if (wild.flag & 4)
	{
		ret = (find_wild_str(wild_token, \
		get_ls_list(mini->envp), wild.flag));
		if (!ret)
			ft_lstadd_back(&ret, ft_lstnew(wild.str));
		return (ret);
	}
	return (0);
}
