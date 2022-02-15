/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 21:37:12 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/16 00:53:28 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_envname(char *name)
{
	char	*res;

	res = ft_strjoin(name, "=");
	return (res);
}

char	*ft_getenv(char **envp, char *name)
{
	char	*envname;
	size_t	env_len;
	int		i;

	envname = get_envname(name);
	env_len = ft_strlen(envname);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], envname, env_len) == 0)
		{
			free(envname);
			return (&(envp[i][env_len]));
		}
		i++;
	}
	free(envname);
	return (NULL);
}

void	exit_num_set(int num)
{
	g_exit_state = num;
}

void	token_free(t_list *lst)
{
	t_list	*head;
	t_token	*token;

	while (lst != NULL)
	{
		head = NULL;
		token = NULL;
		head = lst;
		token = head->content;
		lst = lst->next;
		ft_free(&token->token);
		free(token);
		token = NULL;
		free(head);
	}
}

void	argv_free(t_list *lst)
{
	t_list	*head;
	t_argv	*argv;

	while (lst != NULL)
	{
		head = NULL;
		argv = NULL;
		head = lst;
		argv = head->content;
		lst = lst->next;
		ft_two_dimension_free(&(argv->argv));
		free(argv);
		argv = NULL;
		free(head);
	}
}
