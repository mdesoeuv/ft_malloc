# Determine the host type if not provided
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -fPIC
OPTIMIZATION_FLAGS := -Ofast -march=native -ffast-math
DEBUG_FLAGS := #-fsanitize=address -g3

# Source files
SRC_DIR := src
SRC := main.c
SRC_FILES := $(addprefix $(SRC_DIR)/, $(SRC))

# Object files
OBJ_DIR := objs
OBJS := $(SRC:.c=.o)
OBJS_FILES := $(addprefix $(OBJ_DIR)/, $(OBJS))

# Test files
TEST_DIR := test
TEST := test.c
TEST_FILES := $(addprefix $(TEST_DIR)/, $(TEST))
TEST_OBJ_DIR := $(TEST_DIR)/objs
OBJS_TEST := $(TEST:.c=.o)
OBJS_TEST_FILES := $(addprefix $(TEST_OBJ_DIR)/, $(OBJS_TEST))

# Header files
HEADERS := includes/ft_malloc.h

# Library
LIB_DIR := libft
LIB := $(LIB_DIR)/libft.a

# FT_PRINTF
FT_PRINTF_DIR := ft_printf
FT_PRINTF := $(FT_PRINTF_DIR)/libftprintf.a

# Target
TARGET := libft_malloc_$(HOSTTYPE).so

all: libft libftprintf $(TARGET) test

test: $(OBJS_TEST_FILES) $(TARGET) $(LIB) $(FT_PRINTF)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OBJS_TEST_FILES) -o $(TEST_DIR)/test_exec  -L . -l ft_malloc_$(HOSTTYPE) -L $(FT_PRINTF_DIR) -l ftprintf

$(TARGET): $(OBJ_DIR) $(OBJS_FILES) $(LIB) $(FT_PRINTF)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -shared $(OBJS_FILES) -o $(TARGET) $(LIB)

libft:
	make -C $(LIB_DIR)

libftprintf:
	make -C $(FT_PRINTF_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADERS) | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) -c $< -o $@

$(OBJ_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR)
	make clean -C $(LIB_DIR)

fclean: clean
	rm -f $(TARGET) $(TEST_DIR)/test_exec
	make fclean -C $(LIB_DIR)

re: fclean all

.PHONY: all test libft clean fclean re
