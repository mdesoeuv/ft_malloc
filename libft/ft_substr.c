/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 15:48:19 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/09 01:39:51 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*strsub;
	unsigned int	max;
	unsigned int	i;

	if (!s)
		return (NULL);
	if (start >= (unsigned int)ft_strlen(s))
		return (ft_strdup(""));
	if (ft_strlen(s) - start < len)
		len = ft_strlen(s) - start;
	strsub = (char *)malloc((sizeof(char) * len) + 1);
	if (!strsub)
		return (NULL);
	i = 0;
	max = start + len;
	while (s[start] && start < max)
	{
		strsub[i] = s[start];
		i++;
		start++;
	}
	strsub[i] = '\0';
	return (strsub);
}
