# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Output executable
TARGET = histext2fs

# Source and object files
SRCS = ext2fs_print.c histext2fs.c
OBJS = $(SRCS:.c=.o)

# Default build target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up
clean:
	rm -f $(TARGET) *.o
