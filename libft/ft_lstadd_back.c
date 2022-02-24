/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungcoh <seungcoh@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 16:04:51 by jeunjeon          #+#    #+#             */
/*   Updated: 2022/02/24 14:06:26 by seungcoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tail;

	tail = NULL;
	if (*lst == NULL)
	{
		*lst = new;
		new->next = NULL;
		new->pre = NULL;
		//return ;
	}
	else
	{
		tail = *lst;
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = new;
		new->pre = tail;
		new->next = NULL;
	}
	printf("pre:%p new:%p\n", new->pre, new);
}
