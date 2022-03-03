/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_symbol_utility.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 17:09:18 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/03 21:03:19 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	open_heredoc(t_mini *mini, char *next_str)
{
	int		fd;
	char	*input;

	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == ERROR)
	{
		error_1(".heredoc_tmp", "No such file or directory", 1);
		exit (1);
	}
	while (TRUE)
	{
		input = readline("> ");
		if (is_close_heredoc(input, next_str) == TRUE)
		{
			close(fd);
			return (FALSE);
		}
		if (input != NULL)
			refine_heredoc(mini, &input);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		ft_free(&input);
	}
	close(fd);
	return (TRUE);
}

t_bool	open_file(t_mini *mini, char *symbol, char *next_str)
{
	int		fd;
	t_bool	ret;

	ret = TRUE;
	if ((ft_strncmp(symbol, "<<", 3) == 0))
	{
		g_sig->type = HEREDOC;
		ft_signal();
		ret = open_heredoc(mini, next_str);
		g_sig->type = BASIC;
		ft_signal();
		return (ret);
	}
	else if ((ft_strncmp(symbol, "<", 2) == 0))
	{
		fd = open(next_str, O_RDONLY, 0644);
		if (fd == ERROR)
		{
			error_1(next_str, "No such file or directory", 1);
			return (FALSE);
		}
		close(fd);
	}
	return (TRUE);
}

t_bool	create_file(char *symbol, char *next_str)
{
	int	fd;

	if ((ft_strncmp(symbol, ">>", 3) == 0))
	{
		fd = open(next_str, O_CREAT | O_TRUNC | O_APPEND | O_EXCL, 0644);
		if (fd == ERROR)
			return (TRUE);
		close(fd);
	}
	else if ((ft_strncmp(symbol, ">", 2) == 0) || \
			(ft_strncmp(symbol, "<>", 3) == 0) || \
			(ft_strncmp(symbol, ">|", 3) == 0))
	{
		fd = open(next_str, O_CREAT | O_TRUNC, 0644);
		if (fd == ERROR)
		{
			error_1(next_str, "Create file error", 1);
			exit (1);
		}
		close(fd);
	}
	return (TRUE);
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
