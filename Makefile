NAME=cub3d
CC=cc
SRC=main.c
OBJ=main.o
SANITIZE=-fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -Lmlx -lmlx -framework OpenGL -O3 -framework AppKit -o $(NAME)

%.o:%.c
	$(CC) -Wall -Wextra -Werror -Imlx -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all