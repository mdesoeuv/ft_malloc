/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:03:02 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 12:17:39 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char			*tab_dest;
	const unsigned char		*tab_src;
	unsigned char			ch;
	size_t					i;

	i = 0;
	tab_dest = (unsigned char *)dst;
	tab_src = (const unsigned char *)src;
	ch = c;
	while (i < n)
	{
		tab_dest[i] = tab_src[i];
		if (tab_src[i] == ch)
			return ((void *)&tab_dest[++i]);
		i++;
	}
	return (NULL);
}
