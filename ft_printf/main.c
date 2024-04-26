/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 14:28:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/30 10:24:11 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	main(void)
{
	int		real_ret;
	int		my_ret;
	char	str[] = "test";
	int		integer;

	integer = 1;
	my_ret = ft_printf("This is a %s of the %dst version completed to 10%% with address = %p\n", str, integer, NULL);
	real_ret = printf("This is a %s of the %dst version completed to 10%% with address = %p\n", str, integer, NULL);
	printf("real ret = %d\nmy ret = %d\n", real_ret, my_ret);
	return (0);
}
