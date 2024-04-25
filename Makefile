# Determine the host type if not provided
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -Wextra
OPTIMIZATION_FLAGS := -Ofast -march=native -ffast-math
DEBUG_FLAGS := -fsanitize=address -g3

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

# Target
TARGET := libft_malloc_$(HOSTTYPE).so

all: libft $(TARGET) test

test: $(OBJS_TEST_FILES) $(TARGET)
	$(CC) $(CFLAGS) $(OBJS_TEST_FILES) -o $(TEST_DIR)/test  -L . -l ft_malloc_$(HOSTTYPE)

$(TARGET): $(OBJ_DIR) $(OBJS_FILES) $(LIB)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -shared $(OBJS_FILES) -o $(TARGET) $(LIB)

libft:
	make -C $(LIB_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADERS) | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(OPTIMIZATION_FLAGS) -c $< -o $@

$(OBJ_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR)
	make clean -C $(LIB_DIR)

fclean: clean
	rm -f $(TARGET) $(TEST_DIR)/wiki_test
	make fclean -C $(LIB_DIR)

re: fclean all

.PHONY: all test libft clean fclean re
