/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:13:22 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 16:53:10 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const char	*tab_src;
	char		*tab_dest;
	size_t		i;

	if (!src || len == 0 || src == dst)
		return (dst);
	tab_src = (const char *)src;
	tab_dest = (char *)dst;
	if (tab_src > tab_dest)
	{
		i = -1;
		while (++i < len)
			tab_dest[i] = tab_src[i];
	}
	else
	{
		i = len;
		while (i-- > 0)
			tab_dest[i] = tab_src[i];
	}
	return (dst);
}
