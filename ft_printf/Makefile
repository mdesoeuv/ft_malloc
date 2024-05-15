# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/30 10:25:46 by mdesoeuv          #+#    #+#              #
#    Updated: 2024/05/15 18:23:16 by mdesoeuv         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

SRCS=	ft_printf.c \
		lib_functions.c \
		putnbr_base.c

OBJS= $(SRCS:.c=.o)

CC= gcc -Wall -Werror -Wextra -fPIC

NAME= libftprintf.a

all: $(NAME)

$(NAME):	$(OBJS)
			ar rc $(NAME) $(OBJS)

%.o:	%.c ft_printf.h Makefile
		$(CC) -c $< -o $@

clean:
		rm -f $(OBJS)

fclean:	clean
		rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
