NAME = ft_nm

CC = gcc -Wall -Werror -Wextra 

OPTI = -Ofast -march=native -ffast-math

#DEBUG = -fsanitize=address -g3 

SRC =	main.c

SRC_FILES = $(addprefix src/, $(SRC))

OBJS = $(SRC:.c=.o)

OBJS_FILES = $(addprefix objs/, $(OBJS))

TEST = wiki_test.c

TEST_FILES = $(addprefix test/, $(TEST))

OBJS_TEST = $(TEST:.c=.o)

OBJS_TEST_FILES = $(addprefix test/objs/, $(OBJS_TEST))

HEADERS = includes/ft_nm.h

LIB = libft/libft.a

all : libft $(NAME) test

test : $(OBJS_TEST_FILES)
			$(CC) $(OBJS_TEST_FILES) -o test/wiki_test

$(NAME)	:	$(OBJS_FILES) $(LIB)
			$(CC) $(DEBUG) $(OBJS_FILES) -o $(NAME) libft/libft.a

libft	:	
			make -C libft


objs/%.o:	src/%.c Makefile $(HEADERS)
			@mkdir -p objs
			$(CC) $(DEBUG) $(OPTI) -c $< -o $@ 

test/objs/%.o:	test/%.c Makefile $(HEADERS)
			@mkdir -p test/objs
			$(CC) $(DEBUG) $(OPTI) -c $< -o $@
			
clean	:
			rm -rf objs/
			rm -rf test/objs/
			make clean -C libft

fclean	:	clean
			rm -f $(NAME)
			rm -f test/wiki_test
			make fclean -C libft

re		:	fclean all
	
.PHONY	:	libft all clean re fclean