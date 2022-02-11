/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 13:54:20 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/11 18:12:07 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	heredoc(t_argv *argv, char *delimiter, int *terminal_fd)
{
	int		fd;
	char	*input;

	if (argv->is_heredoc == FALSE)
		return ;
	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	input = NULL;
	while (TRUE)
	{
		input = readline("heredoc> ");
		if (ft_strncmp(input, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			ft_free(&input);
			break ;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		ft_free(&input);
	}
	fd = open(".heredoc_tmp", O_RDONLY, 0644);
	dup2(STDIN_FILENO, *terminal_fd);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	append(char *file)
{
	int fd;

	fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	rtol(char *file)
{
	int fd;

	fd = open(file, O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	ltor(char *file)
{
	int fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
