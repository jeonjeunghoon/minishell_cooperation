/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 13:54:20 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/10 17:51:47 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	heredoc(t_argv *redirect, t_argv *delimiter) // 명령어 실행 후 프로그램이 종료됩니다. 또한 시그널 처리를 위해 부모프로세스에서 실행합니다.
{
	int		fd;
	char	*input;

	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	input = NULL;
	while (TRUE)
	{
		input = readline("heredoc> ");
		if (ft_strncmp(input, delimiter->argv[0], ft_strlen(delimiter->argv[0]) + 1) == 0)
		{
			ft_free(&input);
			break ;
		}
		write(fd, ft_strjoin(input, "\n"), ft_strlen(input) + 1);
		ft_free(&input);
	}
	fd = open(".heredoc_tmp", O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	append(t_argv *redirect, t_argv *file) // 여러 번 반복해서 append를 할 경우 출력스트림이 정상적으로 작동하지 않습니다.
{
	int fd;

	fd = open(file->argv[0], O_CREAT | O_RDWR | O_APPEND, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	rtol(t_argv *redirect, t_argv *file)
{
	int fd;

	fd = open(file->argv[0], O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	ltor(t_argv *redirect, t_argv *file)
{
	int fd;

	fd = open(file->argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
