/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 14:19:29 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/12/03 15:23:16 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_strlen_print(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	putchar_ret(char c, int *total_length)
{
	write(1, &c, 1);
	*total_length += 1;
}

void	putstr_ret(char *s, int *total_length)
{
	if (!s)
	{
		write(1, "(null)", 6);
		*total_length += 6;
	}
	else
	{
		write(1, s, ft_strlen_print(s));
		*total_length += ft_strlen_print(s);
	}
}
