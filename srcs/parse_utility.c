/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utility.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 00:00:31 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/23 14:35:37 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	is_valid_symbol(char *str)
{
	int		len;

	len = ft_strlen(str);
	if (len == 1 && str[0] == '&')
	{
		error_1(str, "invalid symbol", 258);
		return (FALSE);
	}
	if (len == 2 && str[0] != str[1])
	{
		error_symbol(str[1], 258);
		return (FALSE);
	}
	if (len > 2 || (len == 2 && str[0] != str[1]))
	{
		error_symbol(str[len - 1], 258);
		return (FALSE);
	}
	return (TRUE);
}

int	stream_flag_str(t_token *token)
{
	int		i;
	char	*str;

	if (token->single_quote == TRUE || token->double_quote == TRUE)
		return (0);
	i = 0;
	str = token->token;
	while (str[i] != '\0')
	{
		if (str[i] == '|' || str[i] == '>' || str[i] == '<' || str[i] == '&')
			return (TRUE);
		i++;
	}
	return (0);
}

void	token_init(t_token *token)
{
	token->token = NULL;
	token->single_quote = FALSE;
	token->double_quote = FALSE;
}

void	create_stream(t_argv **stream, t_token *token, t_list **argv_lst)
{
	(*stream) = (t_argv *)malloc(sizeof(t_argv));
	argv_init(*stream);
	(*stream)->argv = (char **)malloc(sizeof(char *) * 2);
	(*stream)->argv[1] = NULL;
	(*stream)->argv[0] = ft_strdup(token->token);
	// (*stream)->argv[0] = token->token;
	(*stream)->is_redirect = TRUE;
	/*if (token->token[0] == '&' && token->token[1] == '&')
	{
		(*stream)->is_and = TRUE;
	}
	else if (token->token[0] == '|' && token->token[1] == '|')
		(*stream)->is_or = TRUE;
	else */
	if ((token->token[0] == '|' && token->token[1] == '|') ||\
	(token->token[0] == '|' && token->token[1] == 0) ||\
	token->token[0] == '&' && token->token[1] == '&')
	{
		(*stream)->is_stream = TRUE;
		(*stream)->is_redirect = FALSE;
	}
	ft_lstadd_back(argv_lst, ft_lstnew(*stream));
}

void	check_stream(t_argv *argv, t_list *token_lst, char *str)
{
	if (str[0] == '|' && str[1] == 0)
		argv->is_pipe = TRUE;
	if (str[0] == '&' && str[1] == '&')
		argv->is_and = TRUE;
	if (str[0] == '|' && str[1] == '|')
		argv->is_or = TRUE;
}

void	create_argv(t_argv **argv, t_list *token_lst, \
					t_list **argv_lst, int size)
{
	int		i;

	(*argv) = (t_argv *)malloc(sizeof(t_argv));
	(*argv)->argv = (char **)malloc(sizeof(char *) * (size + 1));
	(*argv)->argv[size] = NULL;
	argv_init(*argv);
	i = 0;
	while (token_lst != NULL && i < size && \
			stream_flag_str(token_lst->content) == FALSE)
	{
		(*argv)->argv[i] = ft_strdup(((t_token *)token_lst->content)->token);
		// (*argv)->argv[i] = ((t_token *)token_lst->content)->token;
		if ((*argv)->argv[i][0] == '*')
			(*argv)->is_wildcard = TRUE;
		i++;
		token_lst = token_lst->next;
	}
	if (token_lst != NULL)
		check_stream(*argv, token_lst, ((t_token *)token_lst->content)->token);
	(*argv)->is_stream = FALSE;
	ft_lstadd_back(argv_lst, ft_lstnew(*argv));
}
