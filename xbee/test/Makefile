CC = gcc
CFLAGS = -g -Wall -Werror -I..
TARGET = test_reader_writer
SRCS = ../xbee_writer.c ../xbee_reader.c test_xbee_reader_writer.c
OBJS = $(SRCS:.c=.o)

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm *.o $(TARGET)
