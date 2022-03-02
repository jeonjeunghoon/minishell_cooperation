/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_symbol_utility2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 17:10:41 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/02 18:55:48 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	symbol_free(char **symbol, char **near_symbol)
{
	if (*symbol != NULL)
		ft_free(symbol);
	if (*near_symbol != NULL)
		ft_free(near_symbol);
}

void	refine_heredoc(t_mini *mini, char **input)
{
	t_refine	*refine;

	refine = (t_refine *)malloc(sizeof(t_refine));
	refine_init(refine);
	refine->envp = mini->envp;
	refine->str = *input;
	create_refined_str(mini, refine);
	ft_free(input);
	*input = ft_strdup(refine->new_str);
	ft_free(&refine->new_str);
	refine_init(refine);
	free(refine);
	refine = NULL;
}

void	heredoc_catch_signal(char **input)
{
	ft_free(input);
	if (g_sig->signum != SIGINT)
	{
		ft_putstr_fd("\x1b[1A", STDOUT_FILENO);
		ft_putstr_fd("> ", STDOUT_FILENO);
		exit_num_set(0);
	}
}

t_bool	is_close_heredoc(char *input, char *next_str)
{
	if (g_sig->signum == SIGINT || input == NULL)
	{
		heredoc_catch_signal(&input);
		return (TRUE);
	}
	if (ft_strncmp(input, next_str, ft_strlen(next_str) + 1) == 0)
	{
		ft_free(&input);
		return (TRUE);
	}
	return (FALSE);
}
