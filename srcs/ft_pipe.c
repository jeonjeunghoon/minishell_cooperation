/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:17:17 by seungcoh          #+#    #+#             */
/*   Updated: 2022/02/12 22:46:27 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pipe_tmp_copy(t_argv *argv)
{
    int fd[2];
    int len;
    int buf[1024];

    if (argv->was_pipe)
		unlink(".pipe_tmp");
	if (argv->is_pipe)
	{
		fd[1] = open(".pipe_tmp2", O_RDONLY, 0644);
		fd[0] = open(".pipe_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		while (1)
		{
			len = read(fd[1], buf, sizeof(buf));
			if (!len)
				break;
			write(fd[0], buf, len);
			// write(1, buf, len);
		}
		close(fd[0]);
		close(fd[1]);
		unlink(".pipe_tmp2");
	}
}
void    when_there_is_pipe(t_argv *argv)
{
   	int fd[2];
    printf("\n[%d %d %d %s]\n\n", argv->is_pipe, argv->was_pipe, argv->is_input, argv->argv[0]);
	if (argv->was_pipe)// 명령어에 input이 없고 명령어 앞에 pipe가 있을 경우 이전 명령어가 만든 출력을 읽어와 argv에 넣어야함
	{
		fd[0] = open(".pipe_tmp", O_RDONLY, 0644);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	if (argv->is_pipe) //pipe 존재시 파일에 출력
	{
		fd[1] = open(".pipe_tmp2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}