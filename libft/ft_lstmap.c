/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 15:24:11 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/08 16:11:27 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*start;
	t_list	*new_elem;

	(void)del;
	start = NULL;
	while (lst != NULL)
	{
		new_elem = ft_lstnew((*f)(lst->content));
		ft_lstadd_back(&start, new_elem);
		lst = lst->next;
	}
	return (start);
}
