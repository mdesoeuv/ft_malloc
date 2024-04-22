/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 10:47:40 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 12:16:36 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	const unsigned char	*tab_src;
	unsigned char		*tab_dest;
	size_t				i;

	i = 0;
	if (!src)
		return (dest);
	tab_dest = (unsigned char *)dest;
	tab_src = (const unsigned char *)src;
	while (i < size)
	{
		tab_dest[i] = tab_src[i];
		i++;
	}
	return (dest);
}
