#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

void	ft_putnbr_base(int nbr, char *base, int *total_length);
void	ft_putnbr_base_u(unsigned int n, char *base, int *total_length);
void	ft_putnbr_base_u_l(long unsigned int n, char *base, int *total_length);
void	ft_putchar_ret(char c, int *total_length);
int		ft_strlen_print(char *s);
void	ft_putstr_ret(char *s, int *total_length);
int		ft_printf(const char *format, ...);

#endif