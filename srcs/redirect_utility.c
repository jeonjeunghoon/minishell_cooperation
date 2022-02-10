/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 13:54:20 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/10 16:57:45 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	heredoc(t_argv *redirect, t_argv *file)
{
	
}

void	append(t_argv *redirect, t_argv *file)
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
