CC = gcc

CFLAGS  = -g -Wall
LDFLAGS = -lreadline

SRC_DIR := src
OBJ_DIR := obj
INCLUDE_DIR := ./include

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = linux_cmd_cheat_sheeter

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -I$(INCLUDE_DIR) $(CFLAGS) -c -o $@ $<

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

install:
	@./install.sh