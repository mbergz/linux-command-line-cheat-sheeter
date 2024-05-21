CC = gcc

CFLAGS  = -g -Wall

LIBS = -lncurses

SRCS = main.c csfind.c csgrep.c common.c
OBJS = $(SRCS:.c=.o)
TARGET = cs

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)