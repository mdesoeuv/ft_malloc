/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 11:03:01 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 09:44:31 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	ch;
	int		i;

	i = 0;
	ch = c;
	while (s[i])
		i++;
	if (ch == s[i])
		return ((char *)&s[i]);
	while (i >= 0)
	{
		if (ch == s[i])
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
