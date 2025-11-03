CC      = gcc
CFLAGS  = -Wall -Werror -Wextra -pedantic -std=gnu89
SRC     = shell.c path.c exec.c utils.c
OBJ     = $(SRC:.c=.o)
BIN     = hsh

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(BIN)

re: fclean all

.PHONY: all clean fclean re

