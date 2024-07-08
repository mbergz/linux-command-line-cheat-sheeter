CC = gcc

CFLAGS  = -g -Wall
LDFLAGS = -lreadline

SRCS=$(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = linux_cmd_cheat_sheeter
INSTALL_DIR := $(HOME)/.local/bin

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

install:
	@./install.sh