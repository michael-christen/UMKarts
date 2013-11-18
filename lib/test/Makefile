CC = gcc
CFLAGS = -g -Wall -Werror -I..
TARGETS = test_circle_buffer test_sa

all: $(TARGETS)

test_circle_buffer: circle_buffer.o ../circle_buffer.o
	$(CC) $(CFLAGS) -o $@ $^

test_sa: simple_allocator.o ../simple_allocator.o
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm *.o $(TARGETS)
