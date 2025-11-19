CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2

server: server.c print_output.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f server

.PHONY: clean
