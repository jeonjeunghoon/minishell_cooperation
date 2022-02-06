/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 14:14:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/06 15:31:11 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	check_filemode_cmdpath(char **argv, struct stat file_info, char *cmd_path)
{
	if (S_ISDIR(file_info.st_mode))
	{
		error_1(argv[0], "is a directory");
		exit_num_set(126);
		return (ERROR);
	}
	if (!S_ISREG(file_info.st_mode))
	{
		error_1(argv[0], "No such file or directory");
		exit_num_set(127);
		return (ERROR);
	}
	if (cmd_path == NULL)
	{
		error_1(argv[0], "command not found");
		exit_num_set(127);
		return (ERROR);
	}
	return (0);
}

t_bool	is_relative_path(t_mini *mini, char **file_path, char *cmd, struct stat *file_info)
{
	char		*tmp;
	int			i;

	create_path_bundle(mini);
	if (mini->path == NULL)
		return (FALSE);
	tmp = ft_strjoin("/", cmd);
	i = 0;
	while (mini->path[i])
	{
		*file_path = ft_strjoin(mini->path[i], tmp);
		stat(*file_path, file_info);
		if (S_ISREG(file_info->st_mode))
			break ;
		ft_free(file_path);
		i++;
	}
	ft_free(&tmp);
	if (*file_path == NULL)
		return (FALSE);
	return (TRUE);
}

t_bool	is_absolute_path(char **file_path, char *cmd, struct stat *file_info)
{
	stat(cmd, file_info);
	if (S_ISREG(file_info->st_mode))
	{
		*file_path = ft_strdup(cmd);
		return (TRUE);
	}
	return (FALSE);
}
