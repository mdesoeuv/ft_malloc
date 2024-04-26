/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putnbr_base.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 09:59:40 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/12/03 15:23:53 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	putnbr_base(int nbr, char *base, int *total_length)
{
	int			base_len;
	long int	n;

	base_len = ft_strlen_print(base);
	if (base_len < 2)
		return ;
	n = nbr;
	if (n < 0)
	{
		putchar_ret('-', total_length);
		n = -n;
	}
	if (n < base_len)
		putchar_ret(base[n], total_length);
	else
	{
		putnbr_base(n / base_len, base, total_length);
		putchar_ret(base[n % base_len], total_length);
	}
}

void	putnbr_base_u(unsigned int n, char *base, int *total_length)
{
	unsigned int		base_len;

	base_len = ft_strlen_print(base);
	if (base_len < 2)
		return ;
	if (n < base_len)
		putchar_ret(base[n], total_length);
	else
	{
		putnbr_base_u(n / base_len, base, total_length);
		putchar_ret(base[n % base_len], total_length);
	}
}

void	putnbr_base_u_l(long unsigned int n, char *base, int *total_length)
{
	unsigned int		base_len;

	base_len = ft_strlen_print(base);
	if (base_len < 2)
		return ;
	if (n < base_len)
		putchar_ret(base[n], total_length);
	else
	{
		putnbr_base_u_l(n / base_len, base, total_length);
		putchar_ret(base[n % base_len], total_length);
	}
}
