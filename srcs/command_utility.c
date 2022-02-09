/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 14:57:53 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 16:12:20 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	check_filemode_cmdpath(char *cmd, struct stat **file_info, char *cmd_path)
{
	t_bool	is_error;

	is_error = 0;
	if (cmd_path == NULL)
	{
		error_1(cmd, "command not found", 127);
		is_error = ERROR;
	}
	else if (ft_s_isreg((*file_info)->st_mode) == FALSE)
	{
		error_1(cmd, "No such file or directory", 127);
		is_error = ERROR;
	}
	free(*file_info);
	*file_info = NULL;
	return (is_error);
}

void	create_path_bundle(t_mini *mini)
{
	char	*path_str;

	path_str = ft_getenv(mini->envp, "PATH");
	if (path_str == NULL)
		return ;
	if (mini->path == NULL)
		mini->path = ft_split(path_str, ':');
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
		stat(*cmd_path, file_info);
		if (ft_s_isreg((*file_info).st_mode) == TRUE)
			break ;
		ft_free(cmd_path);
		i++;
	}
	ft_free(&tmp);
}

void	set_absolute_path(char **cmd_path, char *cmd, struct stat *file_info)
{
	stat(cmd, file_info);
	if (ft_s_isreg((*file_info).st_mode) == TRUE)
		*cmd_path = ft_strdup(cmd);
}

int	check_cmd(t_mini *mini, char *cmd, char **cmd_path)
{
	struct stat	*file_info;

	file_info = (struct stat *)malloc(sizeof(struct stat));
	if (cmd[0] == '/')
		set_absolute_path(cmd_path, cmd, file_info);
	else
		set_relative_path(mini, cmd_path, cmd, file_info);
	if (check_filemode_cmdpath(cmd, &file_info, *cmd_path) == ERROR)
		return (ERROR);
	if (*cmd_path == NULL)
		*cmd_path = ft_strdup(cmd);
	return (0);
}
