/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utility.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 01:28:25 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/11 01:35:44 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	set_envp(char ***envp)
{
	char	*buf;
	int		fd;
	int		len;
	int		i;

	fd = open(".export_tmp", O_RDONLY, 0644);
	get_next_line(fd, &buf);
	len = ft_atoi(buf);
	ft_free(&buf);
	ft_two_dimension_free(envp);
	*envp = (char **)malloc(sizeof(char *) * (len + 1));
	(*envp)[len] = NULL;
	i = 0;
	while (i < len)
	{
		get_next_line(fd, &buf);
		(*envp)[i] = ft_strdup(buf);
		printf("%s\n", (*envp)[i]);
		ft_free(&buf);
		i++;
	}
	close(fd);
	unlink(".export_tmp");
}

void	create_export_tmp(char **envp)
{
	int	fd;
	int	i;

	fd = open(".export_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	printf("%zu\n", ft_two_dimension_size(envp));
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
