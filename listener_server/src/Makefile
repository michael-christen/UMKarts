CC = gcc
CFLAGS = -Wall -g

SRCS = log.c serial.c xbee_packet.c main.c xbee.c xbee_reader.c xbee_tx_rx.c
OBJS = $(SRCS:.c=.o)

MAIN = mario_listener

.PHONY = clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(MAIN)
