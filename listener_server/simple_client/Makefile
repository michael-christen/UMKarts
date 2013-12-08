CC = gcc
CFLAGS = -Wall -g
LIBS = -lzmq

SRCS = client.c
OBJS = $(SRCS:.c=.o)

MAIN = client

.PHONY = clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(MAIN)
