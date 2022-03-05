/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:45:11 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/06 02:50:08 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_envname_export(char *argv)
{
	int		i;
	int		size;
	char	*res;

	size = 0;
	while (argv[size] && argv[size] != '=')
		size++;
	if (argv[size] != '=')
		return (NULL);
	res = (char *)malloc(sizeof(char) * (size + 1));
	res[size] = '\0';
	i = 0;
	while (i < size && argv[i])
	{
		res[i] = argv[i];
		i++;
	}
	return (res);
}

char	**create_export_envp(char **envp, char *env)
{
	char	**new;
	char	*envname;
	int		size;
	int		i;
	int		j;

	size = ft_two_dimension_size(envp);
	envname = get_envname_export(env);
	if (ft_getenv(envp, envname) == NULL)
		size++;
	new = (char **)malloc(sizeof(char *) * (size + 1));
	new[size] = NULL;
	i = 0;
	j = 0;
	while (i < size && envp[j])
	{
		if (ft_strncmp(envp[j], envname, ft_strlen(envname)) == 0)
			j++;
		if (envp[j] != NULL)
			new[i++] = ft_strdup(envp[j++]);
	}
	new[i] = ft_strdup(env);
	ft_free(&envname);
	return (new);
}

int	is_valid_export(char *argv, int i)
{
	if (argv[0] == '=' || (argv[0] >= '0' && argv[0] <= '9'))
		return (ERROR);
	if (argv[i] == '_' || argv[i] == '=' \
		|| (argv[i] >= 'a' && argv[i] <= 'z') \
		|| (argv[i] >= 'A' && argv[i] <= 'Z') \
		|| (argv[i] >= '0' && argv[i] <= '9'))
		return (0);
	return (ERROR);
}

int	check_export_argv(char *argv)
{
	int		i;
	char	*error_msg;

	i = 0;
	while (argv[i])
	{
		if (is_valid_export(argv, i) == ERROR)
		{
			error_msg = ft_strjoin_bothside("`", argv, "'");
			error_2("export", error_msg, "not a valid identifier", 1);
			ft_free(&error_msg);
			return (ERROR);
		}
		i++;
	}
	return (0);
}

void	print_export(char **export_list)
{
	int	i;

	i = 0;
	while (export_list[i] != NULL)
	{
		printf("%s\n", export_list[i]);
		i++;
	}
}

void	ft_export(t_mini *mini, t_argv *argv)
{
	int		i;
	char	**new_envlst;

	exit_num_set(0);
	if (ft_two_dimension_size(argv->argv) > 1)
	{
		i = 1;
		while (argv->argv[i])
		{
			if (check_export_argv(argv->argv[i]) != ERROR)
			{
				new_envlst = create_export_envp(mini->env_list, argv->argv[i]);
				ft_two_dimension_free(&(mini->env_list));
				mini->env_list = new_envlst;
			}
			i++;
		}
	}
	else
		print_export(mini->export_list);
}
