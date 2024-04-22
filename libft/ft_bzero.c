/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 10:33:43 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 09:28:17 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*tab;
	size_t	i;

	if (s == NULL)
		return ;
	i = 0;
	tab = (char *)s;
	while (i < n)
	{
		tab[i] = 0;
		i++;
	}
}
