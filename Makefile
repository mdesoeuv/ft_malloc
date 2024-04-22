NAME = ft_nm

CC = gcc -Wall -Werror -Wextra 

OPTI = -Ofast -march=native -ffast-math

#DEBUG = -fsanitize=address -g3 

SRC =	main.c

SRC_FILES = $(addprefix src/, $(SRC))

OBJS = $(SRC:.c=.o)

OBJS_FILES = $(addprefix objs/, $(OBJS))

HEADERS = includes/ft_nm.h

LIB = libft/libft.a

all : libft $(NAME)

$(NAME)	:	$(OBJS_FILES) $(LIB)
			$(CC) $(DEBUG) $(OBJS_FILES) -o $(NAME) libft/libft.a

libft	:	
			make -C libft


objs/%.o:	src/%.c Makefile $(HEADERS)
			@mkdir -p objs
			$(CC) $(DEBUG) $(OPTI) -c $< -o $@ 
			
clean	:
			rm -rf objs/
			make clean -C libft

fclean	:	clean
			rm -f $(NAME)
			make fclean -C libft

re		:	fclean all
	
.PHONY	:	libft all clean re fclean