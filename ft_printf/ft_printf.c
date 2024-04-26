/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 09:25:07 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/30 13:58:55 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	put_address(unsigned long int address, int *total_length)
{
	putstr_ret("0x", total_length);
	putnbr_base_u_l(address, "0123456789abcdef", total_length);
}

void	ft_put(char c, int *total_length, va_list argptr)
{
	if (c == 'i' || c == 'd')
		putnbr_base(va_arg(argptr, int), "0123456789", total_length);
	else if (c == 'c')
		putchar_ret(va_arg(argptr, int), total_length);
	else if (c == 'u')
		putnbr_base_u_l(va_arg(argptr, unsigned int), \
		"0123456789", total_length);
	else if (c == 'x')
		putnbr_base_u_l(va_arg(argptr, unsigned int), \
		"0123456789abcdef", total_length);
	else if (c == 'X')
		putnbr_base_u_l(va_arg(argptr, unsigned int), \
		"0123456789ABCDEF", total_length);
	else if (c == 'p')
		put_address(va_arg(argptr, unsigned long int), total_length);
	else if (c == 's')
		putstr_ret(va_arg(argptr, char *), total_length);
	else if (c == '%')
		putchar_ret('%', total_length);
	else
		putchar_ret(c, total_length);
}

int	ft_printf(const char *format, ...)
{
	va_list	argptr;
	int		total_length;
	int		i;

	va_start(argptr, format);
	i = 0;
	total_length = 0;
	while (format[i])
	{
		if (format[i] != '%')
			putchar_ret(format[i++], &total_length);
		else
		{
			ft_put(format[++i], &total_length, argptr);
			if (format[i] == 0)
				break ;
			i++;
		}
	}
	va_end(argptr);
	return (total_length);
}
