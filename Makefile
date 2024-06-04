CC = gcc

CFLAGS  = -g -Wall

SRCS = main.c find.c grep.c common.c
OBJS = $(SRCS:.c=.o)
TARGET = cs

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)