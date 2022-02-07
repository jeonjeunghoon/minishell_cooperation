/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 14:57:53 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/07 18:49:21 by seungcoh         ###   ########.fr       */
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

void	exe_cmd(char *cmd_path, t_argv *argv, char **envp, t_bool sig_flag)
{
	pid_t	pid;
	pid_t	child;
	int		stat_loc;
	int		fd;
	int		fd2;
	char	**new_argv;
	int		size;
	char	buf[1024];

	sig_flag = TRUE;
	ft_signal(&sig_flag);
	pid = fork();
	if (pid > 0)
	{
		child = waitpid(pid, &stat_loc, WUNTRACED);
		//pipe_tmp는 입력, pipe_tmp2는 출력 파일로 고정
		//이를 위한 복사
		if (argv->is_pipe)
		{
			fd2 = open("pipe_tmp2", O_RDONLY, 0644);
			fd = open("pipe_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			while (read(fd2, buf, sizeof(buf)))
				write(fd, buf, ft_strlen(buf));
			close(fd);
			close(fd2);
			//pipe_tmp2지우는 코드 추가
		}
		if (!argv->is_input && argv->was_pipe)
			free(argv->argv[ft_two_dimension_size(argv->argv)]);
		if (WIFEXITED(stat_loc))
			exit_num_set(WSTOPSIG(stat_loc));
	}
	else if (pid == 0)
	{
		//printf("\n[%s %d %d %d %s]\n\n", cmd_path, argv->is_pipe, argv->was_pipe, argv->is_input, argv->argv[0]);
		if (!argv->is_input && argv->was_pipe)// 명령어에 input이 없고 명령어 앞에 pipe가 있을 경우 이전 명령어가 만든 출력을 읽어와 argv에 넣어야함
		{
			argv->argv[ft_two_dimension_size(argv->argv)] = ft_strdup("pipe_tmp");
			argv->argv[ft_two_dimension_size(argv->argv) + 1] = 0;
		}
		/*printf("[cmd_path]:[%s]\n\n", cmd_path);
		int i = -1;
		printf("[argv]\n");
		while (argv->argv[++i])
			printf("%s\n", argv->argv[i]);
		int i = -1;
		printf("\n[envp]\n");
		while (envp[++i])
			printf("%s\n", envp[i]);
		printf("\n[result]\n");*/
		if (argv->is_pipe) //pipe 존재시 파일에 출력
		{
			fd2 = open("pipe_tmp2", O_RDWR | O_CREAT | O_TRUNC, 0644);
			dup2(fd2, 1);
		}
		execve(cmd_path, argv->argv, envp);
		//close(fd2);
	}
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
	return (0);
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
