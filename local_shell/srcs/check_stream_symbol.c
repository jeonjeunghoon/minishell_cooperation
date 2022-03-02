/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_stream_symbol.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 17:08:29 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/02 19:01:26 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	stream_symbol_error(char *prev_str, char *next_str, \
							char *symbol, char *near_symbol)
{
	if ((prev_str == NULL || next_str == NULL) && \
		((ft_strncmp(symbol, "|", 2) == 0) || \
		(ft_strncmp(symbol, "||", 3) == 0) || \
		(ft_strncmp(symbol, "&&", 3) == 0)))
	{
		error_symbol(symbol, 258);
		symbol_free(&symbol, &near_symbol);
		return (FALSE);
	}
	else if (near_symbol != NULL)
	{
		error_symbol(near_symbol, 258);
		symbol_free(&symbol, &near_symbol);
		return (FALSE);
	}
	else if (next_str == NULL)
	{
		error_symbol("newline", 258);
		symbol_free(&symbol, &near_symbol);
		return (FALSE);
	}
	return (TRUE);
}

void	near_symbol_exist(char **near_symbol, char *str, int i)
{
	*near_symbol = valid_symbol_list(str, i);
	if ((*near_symbol)[0] == '\0')
	{
		ft_free(near_symbol);
		*near_symbol = ft_strdup(&(str[i]));
	}
}

t_bool	check_str(char **symbol, char **near_symbol, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (*symbol != NULL)
		{
			near_symbol_exist(near_symbol, str, i);
			break ;
		}
		*symbol = valid_symbol_list(str, i);
		if ((*symbol)[0] == '\0')
		{
			error_1(*symbol, "command not found", 127);
			symbol_free(symbol, near_symbol);
			return (FALSE);
		}
		if ((ft_strlen(*symbol) == 2))
			i += 1;
		i++;
	}
	return (TRUE);
}

t_bool	is_valid_symbol(t_mini *mini, char *str, char *prev_str, char *next_str)
{
	char	*symbol;
	char	*near_symbol;
	t_bool	ret;

	near_symbol = NULL;
	symbol = NULL;
	ret = TRUE;
	if (check_str(&symbol, &near_symbol, str) == FALSE)
		return (FALSE);
	if (stream_symbol_error(prev_str, next_str, symbol, near_symbol) == FALSE)
		return (FALSE);
	if (create_file(symbol, next_str) == FALSE)
	{
		symbol_free(&symbol, &near_symbol);
		return (FALSE);
	}
	if (open_file(mini, symbol, next_str) == FALSE)
	{
		symbol_free(&symbol, &near_symbol);
		return (FALSE);
	}
	symbol_free(&symbol, &near_symbol);
	system("leaks minishell");
	return (ret);
}

int	check_stream_symbol(t_mini *mini, t_list *token_lst)
{
	t_list	*head;
	char	*str;
	char	*prev_str;
	char	*next_str;

	head = token_lst;
	while (head != NULL)
	{
		prev_str = NULL;
		next_str = NULL;
		str = ((t_token *)head->content)->token;
		if (is_stream(str[0]) && ((t_token *)head->content)->is_stream == TRUE)
		{
			if (head->pre != NULL)
				prev_str = ((t_token *)head->pre->content)->token;
			if (head->next != NULL)
				next_str = ((t_token *)head->next->content)->token;
			if (is_valid_symbol(mini, str, prev_str, next_str) == FALSE)
				return (ERROR);
		}
		head = head->next;
	}
	return (0);
}
