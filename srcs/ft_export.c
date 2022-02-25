/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:45:11 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/25 20:04:30 by jeunjeon         ###   ########.fr       */
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
	free(envname);
	return (new);
}

int	is_valid_export(char *argv, int i)
{
	if (argv[0] == '=' || (argv[0] >= '0' && argv[0] <= '9'))
		return (ERROR);
	if (argv[i] == '_' || argv[i] == '='\
		|| (argv[i] >= 'a' && argv[i] <= 'z') \
		|| (argv[i] >= 'A' && argv[i] <= 'Z') \
		|| (argv[i] >= '0' && argv[i] <= '9'))
		return (0);
	return (ERROR);
}

int	check_export_argv(char *argv)
{
	int		i;
	t_bool	is_env;
	char	*error_msg;

	is_env = FALSE;
	i = 0;
	while (argv[i] && is_env == FALSE)
	{
		if (is_valid_export(argv, i) == ERROR)
		{
			error_msg = ft_strjoin_bothside("`", argv, "'");
			error_2("export", error_msg, "not a valid identifier", 1);
			free(error_msg);
			return (ERROR);
		}
		if (argv[i] == '=')
			is_env = TRUE;
		i++;
	}
	if (is_env == FALSE)
		return (ERROR);
	return (0);
}

void	ft_export(t_mini *mini, t_argv *argv)
{
	int		i;
	char	**new_envp;
	int		error_fd;

	exit_num_set(0);
	// if (!argv->is_or)
	// {
	// 	error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	// 	dup2(error_fd, 2);
	// 	close(error_fd);
	// }
	if (ft_two_dimension_size(argv->argv) > 1)
	{
		i = 1;
		while (argv->argv[i])
		{
			if (check_export_argv(argv->argv[i]) != ERROR)
			{
				new_envp = create_export_envp(mini->envp, argv->argv[i]);
				ft_two_dimension_free(&(mini->envp));
				mini->envp = new_envp;
			}
			i++;
		}
	}
	else
		ft_env(mini, argv);
}
