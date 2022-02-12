/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 13:54:20 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/12 22:57:29 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	heredoc(char *delimiter)
{
	int		fd;
	char	*input;
	char	*line;

	fd = open(".heredoc_tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == ERROR)
	{
		error_1(".heredoc_tmp", "No such file or directory", 1);
		return (ERROR);
	}
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
	close(fd);
	fd = open(".heredoc_tmp", O_RDONLY, 0644);
	if (fd == ERROR)
	{
		error_1(".heredoc_tmp", "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	append(char *file)
{
	int fd;

	fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == ERROR)
	{
		error_1(file, "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	rtol(char *file)
{
	int fd;

	fd = open(file, O_RDONLY, 0644);
	if (fd == ERROR)
	{
		error_1(file, "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	ltor(char *file)
{
	int fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == ERROR)
	{
		error_1(file, "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
