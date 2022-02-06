/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 14:57:53 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/06 18:08:23 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	create_path_bundle(t_mini *mini)
{
	char	*path_str;

	path_str = ft_getenv(mini->envp, "PATH");
	if (path_str == NULL)
		return ;
	if (mini->path == NULL)
		mini->path = ft_split(path_str, ':');
}

void	exe_cmd(char *cmd_path, char **argv, char **envp, t_bool sig_flag)
{
	pid_t	pid;
	pid_t	child;
	int		stat_loc;

	sig_flag = TRUE;
	ft_signal(&sig_flag);
	pid = fork();
	if (pid > 0)
	{
		child = waitpid(pid, &stat_loc, WUNTRACED);
		if (WIFEXITED(stat_loc))
			exit_num_set(WSTOPSIG(stat_loc));
	}
	else if (pid == 0)
		execve(cmd_path, argv, envp);
}

char	*check_cmd(t_mini *mini, char *cmd, struct stat *file_info)
{
	char		*file_path;

	file_path = NULL;
	if (is_absolute_path(&file_path, cmd, file_info) == TRUE)
		return (file_path);
	else if (is_relative_path(mini, &file_path, cmd, file_info) == TRUE)
		return (file_path);
	return (NULL);
}

int	mini_command(t_mini *mini, char *cmd, char **argv)
{
	if ((ft_strncmp(cmd, "echo", 5)) == 0)
		ft_echo(mini, argv);
	else if ((ft_strncmp(cmd, "cd", 3)) == 0)
		ft_cd(mini, argv);
	else if ((ft_strncmp(cmd, "pwd", 4)) == 0)
		ft_pwd(argv);
	else if ((ft_strncmp(cmd, "env", 4)) == 0)
		ft_env(mini, argv);
	else if ((ft_strncmp(cmd, "export", 7)) == 0)
		ft_export(mini, argv);
	else if ((ft_strncmp(cmd, "unset", 6)) == 0)
		ft_unset(mini, argv);
	else if ((ft_strncmp(cmd, "exit", 5)) == 0)
		ft_exit(argv);
	else
		return (FALSE);
	return (TRUE);
}
