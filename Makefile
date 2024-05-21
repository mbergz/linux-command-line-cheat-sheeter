CC = gcc

CFLAGS  = -g -Wall

LIBS = -lncurses

SRCS = main.c csfind.c csgrep.c common.c
OBJS = $(SRCS:.c=.o)
TARGET = cs

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXEC)