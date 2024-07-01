CC = gcc

CFLAGS  = -g -Wall
LDFLAGS = -lreadline

SRCS=$(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = cs

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
	cp $(TARGET) /usr/local/bin