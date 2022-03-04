/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 16:01:40 by seungcoh          #+#    #+#             */
/*   Updated: 2022/03/04 11:39:14 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	joinstr(t_wild *wild, char *str)
{
	char	*tmp;

	if (wild->str == 0)
		wild->str = ft_strdup(str);
	else
	{
		tmp = ft_strjoin(wild->str, str);
		free(wild->str);
		wild->str = tmp;
	}
}

t_bool	refine_wild_token(t_wild *wild, t_list **wild_token, \
char *token, t_mini *mini)
{
	wild->tmp_token.token = (char *)malloc(sizeof(char) * \
	(wild->i - wild->start_idx + 1));
	ft_strlcpy(wild->tmp_token.token, token + wild->start_idx, \
	wild->i - wild->start_idx + 1);
	if (refine_str(mini, &wild->tmp_token, mini->envp) == ERROR)
		return (ERROR);
	ft_lstadd_back(wild_token, ft_lstnew(wild->tmp_token.token));
	joinstr(wild, wild->tmp_token.token);
	return (0);
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

t_list	*get_wild_token(t_mini *mini, char *token, t_wild *wild)
{
	t_list	*wild_token;
	int		ret;

	wild_token = 0;
	while (token[wild->i])
	{
		if (!jmp_to_wild(wild, token))
		{
			if (token[wild->i] == '*')
			{
				ret = 0;
				if (wild->i - wild->start_idx)
					ret = refine_wild_token(wild, &wild_token, token, mini);
				flag_set(wild, ret);
				joinstr(wild, "*");
			}
			wild->i++;
		}
	}
	ret = 0;
	if (wild->start_idx && wild->i - wild->start_idx)
		ret = refine_wild_token(wild, &wild_token, token, mini);
	flag_set2(wild, ret);
	return (wild_token);
}

t_list	*get_wild_str(t_mini *mini, char *token)
{
	t_wild	wild;
	t_list	*wild_token;
	t_list	*ret;

	mini->wild_chk = 1;
	init_get_wild_str(&wild, &wild_token);
	wild_token = get_wild_token(mini, token, &wild);
	if (wild.flag & 4)
	{
		ret = (find_wild_str(wild_token, \
		get_ls_list(mini->envp), wild.flag));
		if (!ret)
			ft_lstadd_back(&ret, ft_lstnew(wild.str));
		return (ret);
	}
	mini->wild_chk = 0;
	return (0);
}
