/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:39:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/23 21:27:45 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	valid_symbol_list(char *str, int i)
{
	// if (str[i] == '<' && str[i + 1] == '<' && str[i + 2] == '<')
	// 	return (RTOL3);
	if (str[i] == '>' && str[i + 1] == '>')
		return (LTOR2);
	else if (str[i] == '<' && str[i + 1] == '<')
		return (RTOL2);
	else if (str[i] == '<' && str[i + 1] == '>')
		return (RL);
	else if (str[i] == '|' && str[i + 1] == '|')
		return (V2);
	else if (str[i] == '&' && str[i + 1] == '&')
		return (E2);
	else if (str[i] == '>' && str[i + 1] == '|')
		return (LV);
	else if (str[i] == '>' && str[i + 1] == '&')
		return (LE);
	else if (str[i] == '<' && str[i + 1] == '&')
		return (RE);
	else if (str[i] == '>')
		return (LTOR1);
	else if (str[i] == '<')
		return (RTOL1);
	else if (str[i] == '|')
		return (V1);
	else if (str[i] == '&')
		return (E1);
	return (0);
}

t_bool	is_valid_symbol(char *str, char *prev_str, char *next_str)
{
	int	i;
	int	symbol;
	int	near_symbol;

	near_symbol = 0;
	symbol = 0;
	i = 0;
	while (str[i])
	{
		if (symbol != 0)
		{
			near_symbol = valid_symbol_list(str, i);
			break ;
		}
		symbol = valid_symbol_list(str, i);
		if (symbol == RTOL3)
			i += 2;
		else if (symbol == LTOR2 || symbol == RTOL2 || symbol == RL || \
				symbol == V2 || symbol == E2 || symbol == LV || \
				symbol == LE || symbol == RE)
			i += 1;
		i++;
	}
	if (prev_str == NULL && \
		(symbol == V1 || symbol == V2 || symbol == E1 || symbol == E2))
	{
		if (symbol == V1)
			error_symbol("|", 258);
		else if (symbol == V2)
			error_symbol("||", 258);
		else if (symbol == E1)
			error_symbol("&", 258);
		else if (symbol == E2)
			error_symbol("&&", 258);
		return (FALSE);
	}
	else if (next_str == NULL)
	{
		if (symbol == V1)
			error_symbol("|", 258);
		else if (symbol == V2)
			error_symbol("||", 258);
		else if (symbol == E1)
			error_symbol("&", 258);
		else if (symbol == E2)
			error_symbol("&&", 258);
		else
			error_symbol("newline", 258);
		return (FALSE);
	}
	else if (near_symbol != 0)
	{
		if (near_symbol == LTOR1)
			error_symbol(">", 258);
		else if (near_symbol == LTOR2)
			error_symbol(">>", 258);
		else if (near_symbol == RTOL1)
			error_symbol("<", 258);
		else if (near_symbol == RTOL2)
			error_symbol("<<", 258);
		else if (near_symbol == RTOL3)
			error_symbol("<<<", 258);
		else if (near_symbol == RL)
			error_symbol("<>", 258);
		else if (near_symbol == V1)
			error_symbol("|", 258);
		else if (near_symbol == V2)
			error_symbol("||", 258);
		else if (near_symbol == E1)
			error_symbol("&", 258);
		else if (near_symbol == E2)
			error_symbol("&&", 258);
		else if (near_symbol == LV)
			error_symbol(">|", 258);
		else if (near_symbol == LE)
			error_symbol(">&", 258);
		else if (near_symbol == RE)
			error_symbol("<&", 258);
		return (FALSE);
	}
	return (TRUE);
}

int	check_stream_symbol(t_list *token_lst)
{
	t_list	*head;
	char	*str;
	char	*prev_str;
	char	*next_str;

	head = token_lst;
	str = NULL;
	prev_str = ((t_token *)head->content)->token;
	next_str = NULL;
	while (head != NULL)
	{
		str = ((t_token *)head->content)->token;
		if (str == NULL)
			return (ERROR);
		if (is_stream(str[0]) && ((t_token *)head->content)->is_stream == TRUE)
		{
			if (prev_str == str)
				prev_str = NULL;
			if (head->next != NULL)
				next_str = ((t_token *)head->next->content)->token;
			if (is_valid_symbol(str, prev_str, next_str) == FALSE)
				return (ERROR);
		}
		head = head->next;
		str = NULL;
		next_str = NULL;
	}
	return (0);
}

int	create_argv_lst(t_list **argv_lst, t_list *token_lst)
{
	int		size;
	int		is_or;
	t_argv	*str;
	t_argv	*stream;
	t_list	*head;
	int		i;

	argv_lst_init(&str, &stream, &size);
	head = token_lst;
	is_or = 0;
	while (token_lst != NULL)
	{
		if (stream_flag_str(token_lst->content) == FALSE)
		{
			size++;
			is_or = 0;
		}
		if (stream_flag_str(token_lst->content) == TRUE || \
							token_lst->next == NULL)
		{
			if (is_or)
			{
				i = 0;
				while(((t_token *)token_lst->content)->token[i] == '|' && i !=2)
					i++;
				if (i == 2)
					error_symbol("||", 2);
				else
					error_symbol("|", 2);
				return (ERROR);
			}
			if (size != 0)
				create_argv(&str, head, argv_lst, size);
			if (stream_flag_str(token_lst->content) == TRUE)
				is_or = create_stream(&stream, token_lst->content, argv_lst);
			argv_lst_init(&str, &stream, &size);
			head = token_lst->next;
		}
		token_lst = token_lst->next;
	}
	return 0;
}

void	create_token_lst(t_list **lst, char *input, char **envp)
{
	t_token	*token;
	int		i;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]) == FALSE)
		{
			token = (t_token *)malloc(sizeof(t_token));
			token_init(token);
			if (tokenize(token, input, &i, envp) != ERROR)
				ft_lstadd_back(lst, ft_lstnew(token));
			else
				free(token);
			token = NULL;
		}
		else
			i++;
	}
}

int	exception_handling(char *input)
{
	int		i;
	t_bool	sin;
	t_bool	dou;

	sin = FALSE;
	dou = FALSE;
	i = 0;
	while (input[i])
	{
		if ((input[i] == ';' || input[i] == '\\') && sin == FALSE && dou == FALSE)
		{
			ft_error("unspecified special characters like \\ or ;", 1);
			return (ERROR);
		}
		if (input[i] == '\'' || input[i] == '\"')
			exception_utility(input[i], &sin, &dou);
		i++;
	}
	if (sin == TRUE || dou == TRUE)
	{
		ft_error("unclosed quotes like \' or \"", 1);
		return (ERROR);
	}
	return (0);
}

int	ft_parsing(t_mini *mini)
{
	if (exception_handling(mini->input->user_input) == ERROR)
		return (ERROR);
	add_history(mini->input->user_input);
	create_token_lst(&(mini->input->token_lst), \
					mini->input->user_input, mini->envp);
	if (create_argv_lst(&(mini->input->argv_lst), mini->input->token_lst) == ERROR)
		return (ERROR);
	if (check_stream_symbol(mini->input->token_lst) == ERROR)
		return (ERROR);
	return (0);
}
