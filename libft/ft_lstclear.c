/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:56:10 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 09:29:57 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* switch to the next element before freeing current element */

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cursor;

	if (*lst == NULL)
		return ;
	while (*lst != NULL)
	{
		cursor = *lst;
		(*del)((*lst)->content);
		*lst = (*lst)->next;
		free(cursor);
	}
	*lst = NULL;
}
