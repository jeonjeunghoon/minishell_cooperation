/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeunjeon <jeunjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:57:37 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/09 16:16:29 by jeunjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_pwd(char **argv)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		printf("%s\n", cwd);
	else if (cwd == NULL)
		error_2(argv[0], argv[1], strerror(errno), 1);
	free(cwd);
	exit_num_set(g_exit_state);
}
