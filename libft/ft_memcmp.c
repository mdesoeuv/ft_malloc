/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 14:47:14 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 09:31:55 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*tab_s1;
	const unsigned char	*tab_s2;
	size_t				i;

	if (n == 0)
		return (0);
	i = 0;
	tab_s1 = (const unsigned char *)s1;
	tab_s2 = (const unsigned char *)s2;
	while (i < n)
	{
		if (tab_s1[i] - tab_s2[i] != 0)
			return (tab_s1[i] - tab_s2[i]);
		i++;
	}
	i--;
	return (tab_s1[i] - tab_s2[i]);
}
