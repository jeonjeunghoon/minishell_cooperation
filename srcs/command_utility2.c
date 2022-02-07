/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 14:14:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/07 17:50:36 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	check_filemode_cmdpath(char *cmd, struct stat **file_info, char *cmd_path)
{
	t_bool	is_error;

	is_error = 0;
	if (S_ISDIR((*file_info)->st_mode))
	{
		error_1(cmd, "is a directory");
		exit_num_set(126);
		is_error = ERROR;
	}
	else if (!S_ISREG((*file_info)->st_mode))
	{
		error_1(cmd, "No such file or directory");
		exit_num_set(127);
		is_error = ERROR;
	}
	else if (cmd_path == NULL)
	{
		error_1(cmd, "command not found");
		exit_num_set(127);
		is_error = ERROR;
	}
	free(*file_info);
	*file_info = NULL;
	return (is_error);
}

void	set_relative_path(t_mini *mini, char **cmd_path, char *cmd, struct stat *file_info)
{
	char		*tmp;
	int			i;

	create_path_bundle(mini);
	if (mini->path == NULL)
		return ;
	tmp = ft_strjoin("/", cmd);
	i = 0;
	while (mini->path[i])
	{
		*cmd_path = ft_strjoin(mini->path[i], tmp);
		if (stat(*cmd_path, file_info) == SUCCESS)
			break ;
		ft_free(cmd_path);
		i++;
	}
	ft_free(&tmp);
}

void	set_absolute_path(char **cmd_path, char *cmd, struct stat *file_info)
{
	if (stat(cmd, file_info) == SUCCESS)
		*cmd_path = ft_strdup(cmd);
}
