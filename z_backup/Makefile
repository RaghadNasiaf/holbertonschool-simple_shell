CC      = gcc
CFLAGS  = -Wall -Werror -Wextra -pedantic -std=gnu89
SRC     = shell.c exec.c path.c utils.c
OBJ     = $(SRC:.c=.o)
NAME    = hsh

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

