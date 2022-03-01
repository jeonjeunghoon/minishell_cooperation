/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utility.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 13:54:20 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/27 18:50:49 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	heredoc(t_mini *mini)
{
	int		fd;

	fd = open(".heredoc_tmp", O_RDONLY, 0644);
	if (fd == ERROR)
	{
		error_1(mini, ".heredoc_tmp", "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	append(t_mini *mini, char *file)
{
	int 	fd;

	fd = open(file, O_WRONLY | O_APPEND, 0644);
	if (fd == ERROR)
	{
		error_1(mini, file, "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	rtol(t_mini *mini, char *file)
{
	int 	fd;

	fd = open(file, O_RDONLY, 0644);
	if (fd == ERROR)
	{
		error_1(mini, file, "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	ltor(t_mini *mini, char *file)
{
	int 	fd;

	fd = open(file, O_WRONLY | O_TRUNC, 0644);
	if (fd == ERROR)
	{
		error_1(mini, file, "No such file or directory", 1);
		return (ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
