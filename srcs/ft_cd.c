/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 15:44:33 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 15:34:31 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_env_cd(t_mini *mini, char *old_pwd)
{
	t_argv	argvt;
	char	**argv;
	char	*pwd;
	int		i;

	argv = (char **)malloc(sizeof(char *) * 4);
	argv[3] = NULL;
	argv[0] = ft_strdup("export");
	pwd = getcwd(NULL, 0);
	argv[1] = ft_strjoin("PWD=", pwd);
	free(pwd);
	pwd = NULL;
	argv[2] = ft_strjoin("OLDPWD=", old_pwd);
	argvt.argv = argv;
	argvt.is_pipe = 0;
	argvt.was_pipe = 0;
	ft_export(mini, &argvt);
	i = 0;
	while (i < 4)
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
	argv = NULL;
}

int	go_to_home(char **envp, char *path)
{
	if (path == NULL)
	{
		path = ft_getenv(envp, "HOME");
		if (path == NULL)
		{
			error_2("cd", "HOME", "not set");
			exit_num_set(1);
			return (ERROR);
		}
		else
			chdir(path);
	}
	return (0);
}

int	check_path(char *path)
{
	if (chdir(path) == ERROR)
	{
		error_2("cd", path, strerror(errno));
		exit_num_set(1);
		return (ERROR);
	}
	return (0);
}

char	*get_path(char **envp, char *argv)
{
	char	*ptr;

	ptr = NULL;
	if (argv[0] == '$')
		ptr = ft_getenv(envp, &(argv[1]));
	else
	{
		if (argv == NULL || ft_strncmp(argv, "~", 2) == 0)
			ptr = ft_getenv(envp, "HOME");
		else
			return (argv);
	}
	return (ptr);
}

void	ft_cd(t_mini *mini, t_argv* argv)
{
	char	*path;
	char	*old_pwd;
	int		i;
	int		stat_loc;
	pid_t	pid;

	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, &stat_loc, WUNTRACED);
		pipe_tmp_copy(argv);
		exit_num_set(g_exit_state);
	}
	else if (pid == 0)
	{
		when_there_is_pipe(argv);
		path = NULL;
		i = 1;
		old_pwd = ft_getenv(mini->envp, "PWD");
		while (argv->argv[i])
		{
			path = get_path(mini->envp, argv->argv[i]);
			if (path != NULL)
			{
				if (check_path(path) == ERROR)
					return;
				break;
			}
			i++;
		}
		if (go_to_home(mini->envp, path) == ERROR)
			return;
		set_env_cd(mini, old_pwd);
		exit(0);
	}
}
