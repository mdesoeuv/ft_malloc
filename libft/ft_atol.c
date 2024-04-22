/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 15:28:34 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/08 17:46:47 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static void	init(long *result, long *prev_res, int *sign, int *i)
{
	*sign = 1;
	*result = 0;
	*prev_res = 0;
	*i = 0;
}

long int	ft_atol(char const *str)
{
	int				i;
	int				sign;
	long			result;
	long			prev_res;

	init(&result, &prev_res, &sign, &i);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0') * sign;
		if (result / 10 != prev_res && sign == 1)
			return (-1);
		if (result / 10 != prev_res && sign == -1)
			return (0);
		prev_res = result;
		i++;
	}
	return (result);
}
