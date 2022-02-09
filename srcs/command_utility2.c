/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utility2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 14:14:58 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 13:44:42 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_wstopsig(int stat_loc)
{
	int	x;
	int	sig;

	x = (*(int *)&(stat_loc));
	sig = x >> 8;
	return (sig);
}

t_bool	ft_wifexited(int stat_loc)
{
	int	x;
	int	status;

	x = *(int *)&(stat_loc);
	status = x & 0177;
	if (status == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	ft_s_isreg(int mode)
{
	if ((mode & 0170000) == 0100000)
		return (TRUE);
	return (FALSE);
}

t_bool	ft_s_isdir(int mode)
{
	if ((mode & 0170000) == 0040000)
		return (TRUE);
	return (FALSE);
}

void	exe_cmd(char *cmd_path, t_argv *argv, char **envp, t_bool sig_flag)
{
	pid_t	pid;
	pid_t	child;
	int		stat_loc;
	int		fd;
	int		fd2;
	int		len;
	char	buf[1024];
 
	sig_flag = TRUE;
	ft_signal(&sig_flag);
	pid = fork();
	if (pid > 0)
	{
		child = waitpid(pid, &stat_loc, 0x00000002); // WUNTRACED
		//pipe_tmp는 입력, pipe_tmp2는 출력 파일로 고정
		//이를 위한 복사
		if (argv->was_pipe)
			unlink("pipe_tmp");
		if (argv->is_pipe)
		{
			fd2 = open("pipe_tmp2", O_RDONLY, 0644);
			fd = open("pipe_tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
			while (1)
			{
				len = read(fd2, buf, sizeof(buf));
				if (!len)
					break;
				//write(1, buf, len);
				write(fd, buf, len);
			}
			close(fd);
			close(fd2);
			unlink("pipe_tmp2");
		}
		if (ft_wifexited(stat_loc) == TRUE)
			exit_num_set(ft_wstopsig(stat_loc));
	}
	else if (pid == 0)
	{
		printf("\n[%s %d %d %d %s]\n\n", cmd_path, argv->is_pipe, argv->was_pipe, argv->is_input, argv->argv[0]);
		if (argv->was_pipe)// 명령어에 input이 없고 명령어 앞에 pipe가 있을 경우 이전 명령어가 만든 출력을 읽어와 argv에 넣어야함
		{
			fd = open("pipe_tmp", O_RDONLY, 0644);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (argv->is_pipe) //pipe 존재시 파일에 출력
		{
			fd2 = open("pipe_tmp2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd2, STDOUT_FILENO);
			close(fd2);
		}
		execve(cmd_path, argv->argv, envp);
	}
}
