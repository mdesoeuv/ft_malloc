/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 11:45:12 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/21 15:58:37 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*tab;
	size_t	i;

	tab = (char *)malloc(count * size);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < (count * size))
	{
		tab[i] = '9';
		i++;
	}
	tab[i - 1] = '\0';
	return ((void *)tab);
}
