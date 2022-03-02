/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 16:39:07 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/02 15:11:29 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	refine_heredoc(t_mini *mini, char **input, char **envp)
{
	t_refine	*refine;

	refine = (t_refine *)malloc(sizeof(t_refine));
	refine_init(refine);
	refine->envp = envp;
	refine->str = *input;
	create_refined_str(mini, refine);
	ft_free(input);
	*input = ft_strdup(refine->new_str);
	ft_free(&refine->new_str);
	refine_init(refine);
	free(refine);
	refine = NULL;
}

void	no_symbol(t_mini *mini, char *symbol)
{
	error_1(mini, symbol, "command not found", 127);
}

char	*valid_symbol_list(char *str, int i)
{
	if (str[i] == '>' && str[i + 1] == '>')
		return (ft_strdup(">>"));
	else if (str[i] == '<' && str[i + 1] == '<')
		return (ft_strdup("<<"));
	else if (str[i] == '<' && str[i + 1] == '>')
		return (ft_strdup("<>"));
	else if (str[i] == '|' && str[i + 1] == '|')
		return (ft_strdup("||"));
	else if (str[i] == '&' && str[i + 1] == '&')
		return (ft_strdup("&&"));
	else if (str[i] == '>' && str[i + 1] == '|')
		return (ft_strdup(">|"));
	else if (str[i] == '>')
		return (ft_strdup(">"));
	else if (str[i] == '<')
		return (ft_strdup("<"));
	else if (str[i] == '|')
		return (ft_strdup("|"));
	return (ft_strdup(""));
}

t_bool	is_valid_symbol(t_mini *mini, char *str, char *prev_str, char *next_str, char **envp)
{
	int	i;
	char	*symbol;
	char	*near_symbol;

	near_symbol = NULL;
	symbol = NULL;
	i = 0;
	while (str[i])
	{
		if (symbol != NULL)
		{
			near_symbol = valid_symbol_list(str, i);
			if (near_symbol[0] == '\0')
			{
				ft_free(&near_symbol);
				near_symbol = ft_strdup(&(str[i]));
			}
			break ;
		}
		symbol = valid_symbol_list(str, i);
		if (symbol[0] == '\0')
		{
			ft_free(&symbol);
			no_symbol(mini, str);
			return (FALSE);
		}
		if ((ft_strlen(symbol) == 2))
			i += 1;
		i++;
	}
	if ((prev_str == NULL || next_str == NULL) && \
		((ft_strncmp(symbol, "|", 2) == 0) || \
		(ft_strncmp(symbol, "||", 3) == 0) || \
		(ft_strncmp(symbol, "&&", 3) == 0)))
	{
		error_symbol(mini, symbol, 258);
		ft_free(&symbol);
		if (near_symbol != NULL)
			ft_free(&near_symbol);
		return (FALSE);
	}
	else if (near_symbol != NULL)
	{
		error_symbol(mini, near_symbol, 258);
		ft_free(&symbol);
		ft_free(&near_symbol);
		return (FALSE);
	}
	else if (next_str == NULL)
	{
		error_symbol(mini, "newline", 258);
		ft_free(&symbol);
		if (near_symbol != NULL)
			ft_free(&near_symbol);
		return (FALSE);
	}
	if ((ft_strncmp(symbol, ">", 2) == 0) || \
		(ft_strncmp(symbol, ">>", 3) == 0) || \
		(ft_strncmp(symbol, "<>", 3) == 0) || \
		(ft_strncmp(symbol, ">|", 3) == 0))
	{
		int	fd;

		if ((ft_strncmp(symbol, ">>", 3) == 0))
		{
			if ((fd = open(next_str, O_CREAT | O_TRUNC | O_APPEND | O_EXCL, 0644)) == ERROR)
			{
				ft_free(&symbol);
				if (near_symbol != NULL)
					ft_free(&near_symbol);
				return (TRUE);
			}
		}
		else
			fd = open(next_str, O_CREAT | O_TRUNC, 0644);
		if (fd == ERROR)
		{
			error_1(mini, next_str, "Create directory error", 1);
			exit (1);
		}
		close(fd);
	}
	if ((ft_strncmp(symbol, "<", 2) == 0))
	{
		int fd;

		fd = open(next_str, O_RDONLY, 0644);
		if (fd == ERROR)
		{
			error_1(mini, next_str, "No such file or directory", 1);
			ft_free(&symbol);
			if (near_symbol != NULL)
				ft_free(&near_symbol);
			return (FALSE);
		}
		close(fd);
	}
	if ((ft_strncmp(symbol, "<<", 3) == 0))
	{
		int		fd;
		char	*input;
	
		fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		g_sig->type = HEREDOC;
		if (fd == ERROR)
		{
			error_1(mini, ".heredoc_tmp", "No such file or directory", 1);
			exit (1);
		}
		input = NULL;
		while (TRUE)
		{
			input = readline("> ");
			if (g_sig->signum == SIGINT || input == NULL)
			{
				ft_free(&input);
				if (g_sig->signum != SIGINT)
				{
					ft_putstr_fd("\x1b[1A", STDOUT_FILENO);
					ft_putstr_fd("> ", STDOUT_FILENO);
					exit_num_set(mini, 0);
				}
				return (0);
			}
			if (ft_strncmp(input, next_str, ft_strlen(next_str) + 1) == 0)
			{
				ft_free(&input);
				break ;
			}
			if (input != NULL)
				refine_heredoc(mini, &input, envp);
			write(fd, input, ft_strlen(input));
			write(fd, "\n", 1);
			ft_free(&input);
		}
		close(fd);
		g_sig->type = BASIC;
	}
	ft_free(&symbol);
	if (near_symbol != NULL)
		ft_free(&near_symbol);
	return (TRUE);
}

int	check_stream_symbol(t_mini *mini, t_list *token_lst, char **envp)
{
	t_list	*head;
	char	*str;
	char	*prev_str;
	char	*next_str;

	head = token_lst;
	prev_str = NULL;
	str = NULL;
	next_str = NULL;
	while (head != NULL)
	{
		str = ((t_token *)head->content)->token;
		if (str == NULL)
			return (ERROR);
		if (is_stream(str[0]) && ((t_token *)head->content)->is_stream == TRUE)
		{
			if (head->pre != NULL)
				prev_str = ((t_token *)head->pre->content)->token;
			if (head->next != NULL)
				next_str = ((t_token *)head->next->content)->token;
			if (is_valid_symbol(mini, str, prev_str, next_str, envp) == FALSE)
				return (ERROR);
		}
		head = head->next;
		prev_str = NULL;
		str = NULL;
		next_str = NULL;
	}
	return (0);
}

int	create_argv_lst(t_mini *mini, t_list **argv_lst, t_list *token_lst)
{
	int		size;
	// int		is_or;
	t_argv	*str;
	t_argv	*stream;
	t_list	*head;
	int		i;

	argv_lst_init(&str, &stream, &size);
	head = token_lst;
	// is_or = 0;
	while (token_lst != NULL)
	{
		if (stream_flag_str(token_lst->content) == FALSE)
		{
			size++;
			// is_or = 0;
		}
		if (stream_flag_str(token_lst->content) == TRUE || \
							token_lst->next == NULL)
		{
			// if (is_or)
			// {
			// 	i = 0;
			// 	while(((t_token *)token_lst->content)->token[i] == '|' && i !=2)
			// 		i++;
			// 	if (i == 2)
			// 		error_symbol(mini, "||", 2);
			// 	else
			// 		error_symbol(mini, "|", 2);
			// 	return (ERROR);
			// }
			if (size != 0)
				create_argv(&str, head, argv_lst, size);
			if (stream_flag_str(token_lst->content) == TRUE)
				/*is_or = */create_stream(&stream, token_lst->content, argv_lst);
			argv_lst_init(&str, &stream, &size);
			head = token_lst->next;
		}
		token_lst = token_lst->next;
	}
	return 0;
}

void	create_token_lst(t_mini *mini, t_list **lst, char *input, char **envp)
{
	t_token	*token;
	t_token	*new_token;
	t_list	*wild_str;
	t_list	*tmp;
	int		i;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]) == FALSE)
		{
			token = (t_token *)malloc(sizeof(t_token));
			token_init(token);
			wild_str = tokenize(mini, token, input, &i, envp);
			if (wild_str)
			{
				while (wild_str)
				{
					tmp = wild_str->next;
					new_token = (t_token *)malloc(sizeof(t_token));
					new_token->double_quote = token->double_quote;
					new_token->is_stream = token->is_stream;
					new_token->single_quote = token->single_quote;
					new_token->token = (char *)wild_str->content;
					ft_lstadd_back(lst, ft_lstnew(new_token));
					free(wild_str);
					wild_str = tmp;
				}
				free(token->token);
				free(token);
			}
			else if(wild_str == (t_list *)ERROR)
				free(token);
			else
				ft_lstadd_back(lst, ft_lstnew(token));
			token = NULL;
		}
		else
			i++;
	}
}

int	exception_handling(t_mini *mini, char *input)
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
			ft_error(mini, "unspecified special characters like \\ or ;", 1);
			return (ERROR);
		}
		if (input[i] == '\'' || input[i] == '\"')
			exception_utility(input[i], &sin, &dou);
		i++;
	}
	if (sin == TRUE || dou == TRUE)
	{
		ft_error(mini, "unclosed quotes like \' or \"", 1);
		return (ERROR);
	}
	return (0);
}

int	ft_parsing(t_mini *mini)
{
	if (exception_handling(mini, mini->input->user_input) == ERROR)
		return (ERROR);
	add_history(mini->input->user_input);
	create_token_lst(mini, &(mini->input->token_lst), \
					mini->input->user_input, mini->envp);
	if (create_argv_lst(mini, &(mini->input->argv_lst), mini->input->token_lst) == ERROR)
		return (ERROR);
	if (check_stream_symbol(mini, mini->input->token_lst, mini->envp) == ERROR)
		return (ERROR);
	return (0);
}
