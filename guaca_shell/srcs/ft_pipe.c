/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:17:17 by seungcoh          #+#    #+#             */
/*   Updated: 2022/02/24 04:28:40 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pipe_tmp_copy(t_mini *mini, t_argv *argv)
{
    int fd[2];

	if (argv->was_pipe == FALSE)
		return ;
    if (argv->was_pipe)
		unlink(".pipe_tmp");
	if (argv->is_pipe)
	{
		fd[1] = open(".pipe_tmp2", O_RDONLY, 0644);
		fd[0] = open(".pipe_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		fd_copy(fd[1], fd[0]);
		close(fd[0]);
		close(fd[1]);
		unlink(".pipe_tmp2");
	}
}
t_bool	when_there_is_pipe(t_mini *mini, t_argv *argv)
{
   	int fd[2];

	if (argv->is_pipe == FALSE)
		return (FALSE);
	if (argv->was_pipe)// 명령어에 input이 없고 명령어 앞에 pipe가 있을 경우 이전 명령어가 만든 출력을 읽어와 argv에 넣어야함
	{
		fd[0] = open(".pipe_tmp", O_RDONLY, 0644);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	if (argv->is_pipe) //pipe 존재시 파일에 출력
	{
		fd[1] = open(".pipe_tmp2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	return (TRUE);
}
