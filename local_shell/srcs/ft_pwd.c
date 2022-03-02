/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:57:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/03/02 19:53:46 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_pwd(t_mini *mini, t_argv *argv)
{
	char	*cwd;
	int		error_fd;

	exit_num_set(0);
	cwd = getcwd(NULL, 0);
	if (!argv->is_or)
	{
		error_fd = open(".error_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(error_fd, 2);
		close(error_fd);
	}
	if (cwd)
		printf("%s\n", cwd);
	else if (cwd == NULL)
		error_2(argv->argv[0], argv->argv[1], strerror(errno), 1);
	free(cwd);
	cwd = NULL;
}
