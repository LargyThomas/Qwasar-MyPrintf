NAME = my_printf
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = my_printf.c
OBJS = $(SRC:.c=.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all