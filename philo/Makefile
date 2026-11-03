CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

NAME = philo
SRC = philo.c \
      utiles.c \
      clean_setup.c \
      routin.c \
      simulation.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re