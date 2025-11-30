CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude
SRC=src/tetris.c
BIN=tetrisstack
all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
clean:
	rm -f $(BIN)
