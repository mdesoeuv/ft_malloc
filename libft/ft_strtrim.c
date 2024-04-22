/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 16:18:57 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/05 16:55:48 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isinset(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ft_trim_count(char const *s, int *j, char const *set)
{
	int	i;
	int	trim;

	i = 0;
	trim = 0;
	while (ft_isinset(s[i], set))
	{
		i++;
		trim++;
		(*j)++;
	}
	if (i >= ft_strlen(s))
		return (trim);
	while (s[i])
		i++;
	i--;
	while (ft_isinset(s[i], set))
	{
		i--;
		trim++;
	}
	return (trim);
}

char	*ft_strtrim(char const *s, char const *set)
{
	int		i;
	int		j;
	int		len;
	int		trim;
	char	*strtrim;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	trim = ft_trim_count(s, &j, set);
	len = ft_strlen(s);
	strtrim = (char *)malloc(sizeof(char) * (len - trim + 1));
	if (!strtrim)
		return (NULL);
	while (i < (len - trim))
		strtrim[i++] = s[j++];
	strtrim[i] = '\0';
	return (strtrim);
}
