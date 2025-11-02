CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=gnu89
SRC=main.c utils.c
OBJ=$(SRC:.c=.o)
BIN=simple_shell

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(BIN)

re: fclean all
