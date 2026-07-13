CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = my_morse
SRC = my_morse.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) output.txt
