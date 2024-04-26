/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 14:29:00 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/12/03 15:22:45 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

void	putnbr_base(int nbr, char *base, int *total_length);
void	putnbr_base_u(unsigned int n, char *base, int *total_length);
void	putnbr_base_u_l(long unsigned int n, char *base, int *total_length);
void	putchar_ret(char c, int *total_length);
int		ft_strlen_print(char *s);
void	putstr_ret(char *s, int *total_length);
int		ft_printf(const char *format, ...);

#endif