/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 16:04:04 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 09:42:27 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	var_init(int *len, int *j)
{
	*len = 0;
	*j = 0;
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		j;
	int		len;
	char	*cat;

	var_init(&len, &j);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	while (s1[len])
		len++;
	while (s2[j++])
		len++;
	cat = (char *)malloc(sizeof(char) * (len + 1));
	if (!cat)
		return (NULL);
	var_init(&len, &j);
	while (s1[len])
		cat[j++] = s1[len++];
	len = 0;
	while (s2[len])
		cat[j++] = s2[len++];
	cat[j] = '\0';
	return (cat);
}
