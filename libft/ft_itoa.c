/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 11:13:36 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 09:29:39 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nbrsize(int n)
{
	int			size;
	long int	nbr;

	size = 1;
	nbr = n;
	if (nbr < 0)
	{
		size++;
		nbr = -nbr;
	}
	while (nbr / 10 > 0)
	{
		size++;
		nbr /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	long int	nbr;
	int			size;
	char		*str;

	nbr = n;
	size = ft_nbrsize(n);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	if (nbr < 0)
	{
		nbr = -nbr;
		str[0] = '-';
	}
	str[size] = '\0';
	size--;
	while (nbr / 10 > 0)
	{
		str[size--] = nbr % 10 + '0';
		nbr /= 10;
	}
	str[size--] = nbr % 10 + '0';
	return (str);
}
