# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/05 15:49:39 by mdesoeuv          #+#    #+#              #
#    Updated: 2022/03/01 10:00:02 by mdesoeuv         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

CC:= gcc

CFLAGS:= -Wall -Wextra -Werror

NAME:= libft.a

SRCS:=	ft_isblank.c ft_isspace.c ft_calloc.c ft_strlcpy.c ft_toupper.c \
		ft_tolower.c ft_strtrim.c ft_substr.c ft_strstr.c ft_split.c \
		ft_strrchr.c ft_strnstr.c ft_strnew.c ft_strnequ.c ft_strncpy.c \
		ft_strncmp.c ft_strncat.c ft_strmapi.c ft_strmap.c ft_strlen.c \
		ft_strlcat.c ft_strjoin.c ft_striteri.c ft_striter.c \
		ft_strequ.c ft_strdup.c ft_strdel.c ft_strcpy.c ft_strcmp.c \
		ft_strclr.c ft_strchr.c ft_strcat.c ft_putstr.c ft_putstr_fd.c \
		ft_putnbr.c ft_putnbr_fd.c ft_putendl.c ft_putendl_fd.c ft_putchar.c \
		ft_putchar_fd.c ft_memset.c ft_memmove.c ft_memdel.c ft_memcpy.c \
		ft_memcmp.c ft_memchr.c ft_memccpy.c ft_memalloc.c ft_itoa.c \
		ft_isprint.c ft_isdigit.c ft_isascii.c ft_isalpha.c ft_isalnum.c \
		ft_bzero.c ft_atoi.c ft_swap.c ft_putnbr_base.c \
		ft_strjoin_free_s1.c ft_strndup.c ft_strjoin_free_s2.c ft_is_duplicate.c \
		ft_min.c ft_max.c ft_abs.c ft_if.c ft_strjoin_free.c \
		ft_lstlast.c ft_lstsize.c ft_lstadd_back.c ft_lstadd_front.c \
		ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstnew.c ft_lstmap.c \
		ft_is_charset.c ft_atol.c ft_free_split.c

OBJS:= $(SRCS:.c=.o)

OBJS_DIR:= objs/


all: $(NAME)

$(NAME):	$(addprefix $(OBJS_DIR), $(OBJS)) Makefile
			ar rcs $(NAME) $(addprefix $(OBJS_DIR), $(OBJS))

objs/%.o:	%.c libft.h
			@mkdir -p $(OBJS_DIR)
			$(CC) $(CFLAGS) -c $< -o $@

clean:		
			rm -rf $(OBJS_DIR)

fclean:		clean
			rm -f $(NAME)

re:			
	$(MAKE) fclean
	$(MAKE) all
			
.PHONY: 	all re clean fclean
