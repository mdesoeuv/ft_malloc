# Determine the host type if not provided
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -fPIC   -g3 # -pedantic
OPTIMIZATION_FLAGS := # -O0
DEBUG_FLAGS := # -g3 # -fsanitize=address 

# Source files
SRC_DIR := src
SRC :=	malloc.c \
		realloc.c \
		free.c \
		utils.c \
		ft_printf.c

SRC_FILES := $(addprefix $(SRC_DIR)/, $(SRC))

# Object files
OBJ_DIR := objs
OBJS := $(SRC:.c=.o)
OBJS_FILES := $(addprefix $(OBJ_DIR)/, $(OBJS))

# Test files
TEST_DIR := test
TEST 	 := test.c \
			ft_printf.c
		
TEST_FILES := $(addprefix $(TEST_DIR)/, $(TEST))
TEST_OBJ_DIR := $(TEST_DIR)/objs
OBJS_TEST := $(TEST:.c=.o)
OBJS_TEST_FILES := $(addprefix $(TEST_OBJ_DIR)/, $(OBJS_TEST))

# Header files
HEADERS :=	includes/ft_malloc.h \
			includes/ft_printf.h

# Extra Mock Lib
MOCK_SRC_DIR := extra
MOCK_SRC := extra.c
MOCK_SRC_FILES := $(addprefix $(MOCK_SRC_DIR)/, $(MOCK_SRC))
MOCK_OBJ_DIR := $(MOCK_SRC_DIR)/objs
MOCK_OBJS := $(MOCK_SRC:.c=.o)
MOCK_OBJS_FILES := $(addprefix $(MOCK_OBJ_DIR)/, $(MOCK_OBJS))
MOCK_HEADERS := $(MOCK_SRC_DIR)/extra.h


# Target
TARGET := libft_malloc_$(HOSTTYPE).so

.PHONY: all clean fclean re

all: $(TARGET) $(TEST_DIR)/test_exec libmock.so

test: $(TEST_DIR)/test_exec libmock.so
	$(TEST_DIR)/test_exec

libmock.so: $(MOCK_OBJS_FILES) $(MOCK_HEADERS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -shared $(MOCK_OBJS_FILES) -o libmock.so

$(TEST_DIR)/test_exec: $(OBJS_TEST_FILES) libmock.so
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OBJS_TEST_FILES) -o $@  -L . -l mock

$(TARGET): $(OBJ_DIR) $(OBJS_FILES) $(HEADERS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -shared $(OBJS_FILES) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADERS) Makefile | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) -c $< -o $@

$(MOCK_OBJ_DIR)/%.o: $(MOCK_SRC_DIR)/%.c $(MOCK_HEADERS) Makefile | $(MOCK_OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) -c $< -o $@

$(MOCK_OBJ_DIR):
	mkdir -p $@

$(TEST_OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) $(MOCK_OBJ_DIR)

fclean: clean
	rm -f $(TARGET) $(TEST_DIR)/test_exec libmock.so

re: fclean all

