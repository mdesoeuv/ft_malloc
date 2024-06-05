/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 09:25:07 by mdesoeuv          #+#    #+#             */
/*   Updated: 2024/06/05 17:14:34 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_strlen_print(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putchar_ret(char c, int *total_length)
{
	write(1, &c, 1);
	*total_length += 1;
}

void	ft_putstr_ret(char *s, int *total_length)
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

void	ft_putnbr_base(int nbr, char *base, int *total_length)
{
	int			base_len;
	long int	n;

	base_len = ft_strlen_print(base);
	if (base_len < 2)
		return ;
	n = nbr;
	if (n < 0)
	{
		ft_putchar_ret('-', total_length);
		n = -n;
	}
	if (n < base_len)
		ft_putchar_ret(base[n], total_length);
	else
	{
		ft_putnbr_base(n / base_len, base, total_length);
		ft_putchar_ret(base[n % base_len], total_length);
	}
}

void	ft_putnbr_base_u(unsigned int n, char *base, int *total_length)
{
	unsigned int		base_len;

	base_len = ft_strlen_print(base);
	if (base_len < 2)
		return ;
	if (n < base_len)
		ft_putchar_ret(base[n], total_length);
	else
	{
		ft_putnbr_base_u(n / base_len, base, total_length);
		ft_putchar_ret(base[n % base_len], total_length);
	}
}

void	ft_putnbr_base_u_l(long unsigned int n, char *base, int *total_length)
{
	unsigned int		base_len;

	base_len = ft_strlen_print(base);
	if (base_len < 2)
		return ;
	if (n < base_len)
		ft_putchar_ret(base[n], total_length);
	else
	{
		ft_putnbr_base_u_l(n / base_len, base, total_length);
		ft_putchar_ret(base[n % base_len], total_length);
	}
}



void	ft_put_address(unsigned long int address, int *total_length)
{
	ft_putstr_ret("0x", total_length);
	ft_putnbr_base_u_l(address, "0123456789abcdef", total_length);
}

void	ft_put(char c, int *total_length, va_list argptr)
{
	if (c == 'i' || c == 'd')
		ft_putnbr_base(va_arg(argptr, int), "0123456789", total_length);
	else if (c == 'c')
		ft_putchar_ret(va_arg(argptr, int), total_length);
	else if (c == 'u')
		ft_putnbr_base_u_l(va_arg(argptr, unsigned int), \
		"0123456789", total_length);
	else if (c == 'x')
		ft_putnbr_base_u_l(va_arg(argptr, unsigned int), \
		"0123456789abcdef", total_length);
	else if (c == 'X')
		ft_putnbr_base_u_l(va_arg(argptr, unsigned int), \
		"0123456789ABCDEF", total_length);
	else if (c == 'p')
		ft_put_address(va_arg(argptr, unsigned long int), total_length);
	else if (c == 's')
		ft_putstr_ret(va_arg(argptr, char *), total_length);
	else if (c == '%')
		ft_putchar_ret('%', total_length);
	else
		ft_putchar_ret(c, total_length);
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
			ft_putchar_ret(format[i++], &total_length);
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
