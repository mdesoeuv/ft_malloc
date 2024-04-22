/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 17:14:32 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/10/18 15:04:07 by mdesoeuv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	f_display_list(t_list *begin_list);

void	ft_lstadd_front(t_list **alst, t_list *new_elem)
{
	new_elem->next = *alst;
	*alst = new_elem;
}
