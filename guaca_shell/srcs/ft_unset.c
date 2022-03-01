/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:45:35 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/27 19:11:44 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**create_unset_envp(char **envp, int *position, int size)
{
	char	**new;
	int		i;
	int		j;
	int		k;

	new = (char **)malloc(sizeof(char *) * (size + 1));
	new[size] = NULL;
	i = 0;
	j = 0;
	k = 0;
	while (i < size && envp[j])
	{
		if (j == position[k] && position[k] != -1)
		{
			j++;
			k++;
		}
		new[i] = ft_strdup(envp[j]);
		i++;
		j++;
	}
	return (new);
}

void	get_position(int *position, char **envp, char **argv)
{
	int		i;
	int		j;
	int		k;
	char	*envname;

	i = 1;
	k = 0;
	while (argv[i])
	{
		if (ft_getenv(envp, argv[i]) != NULL)
		{
			j = 0;
			envname = get_envname(argv[i]);
			while (envp[j] && position[k] != -1)
			{
				if (ft_strncmp(envp[j], envname, ft_strlen(envname)) == 0)
					position[k++] = j;
				j++;
			}
			free(envname);
			envname = NULL;
		}
		i++;
	}
}

void	position_init(int **position, int *size, char **envp, char **argv)
{
	int	i;

	*size = 0;
	i = 1;
	while (argv[i] != NULL)
	{
		if (ft_getenv(envp, argv[i]) != NULL)
			(*size)++;
		i++;
	}
	*position = (int *)malloc(sizeof(int) * ((*size) + 1));
	(*position)[(*size)] = -1;
	i = 0;
	while ((*position)[i] != -1)
	{
		(*position)[i] = 0;
		i++;
	}
	(*size) = 0;
}

int	check_unset_argv(t_mini *mini, char **argv, int *size)
{
	int		i;
	int		j;
	char	*msg_argv;

	i = 0;
	while (argv[++i])
	{
		j = -1;
		while (argv[i][++j])
		{
			if ((argv[i][j] != '_' \
				&& !(argv[i][j] >= 'a' && argv[i][j] <= 'z') \
				&& !(argv[i][j] >= 'A' && argv[i][j] <= 'Z') \
				&& !(argv[i][j] >= '0' && argv[i][j] <= '9')) \
				|| (argv[i][0] >= '0' && argv[i][0] <= '9'))
			{
				msg_argv = ft_strjoin_bothside("`", argv[i], "'");
				error_2(mini, argv[0], msg_argv, "not a valid identifier", 1);
				free(msg_argv);
				(*size)--;
				return (ERROR);
			}
		}
	}
	return (0);
}

void	ft_unset(t_mini *mini, t_argv *argv)
{
	int		size;
	int		*position;
	char	**new;
	int		error_fd;

	exit_num_set(mini, 0);
	// if (!argv->is_or)
	// {
	// 	error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	// 	dup2(error_fd, 2);
	// 	close(error_fd);
	// }
	size = ft_two_dimension_size(argv->argv) - 1;
	if (ft_two_dimension_size(argv->argv) > 1)
	{
		if (check_unset_argv(mini, argv->argv, &size) == ERROR)
			return ;
		if (size != 0)
		{
			position_init(&position, &size, mini->envp, argv->argv);
			get_position(position, mini->envp, argv->argv);
			size = ft_two_dimension_size(mini->envp) - ft_numlen(position);
			new = create_unset_envp(mini->envp, position, size);
			ft_two_dimension_free(&(mini->envp));
			mini->envp = new;
			free(position);
			position = NULL;
		}
	}
}
