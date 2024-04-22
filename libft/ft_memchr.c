/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:32:06 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 09:31:39 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t		i;
	const char	*tab;

	tab = (const char *)s;
	i = 0;
	while (i < n)
	{
		if (tab[i] == (char)c)
			return ((void *)&tab[i]);
		i++;
	}
	return (NULL);
}
