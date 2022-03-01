/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_copy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:28:51 by seungcoh          #+#    #+#             */
/*   Updated: 2022/02/16 17:33:27 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void fd_copy(int fd, int fd2)
{
    int		len;
	char	buf[1024];

    while (1)
    {
        len = read(fd, buf, sizeof(buf));
        if (!len)
            break;
        write(fd2, buf, len);
    }
}