NAME=cub3d
CC=cc
SRC=parse/errors_utils.c parse/main.c parse/parse.c parse/parse_utils.c parse/parse_utils2.c main.c ./parse/gnl/get_next_line.c ./parse/gnl/get_next_line_utils.c
OBJ=parse/errors_utils.o parse/main.o parse/parse.o parse/parse_utils.o parse/parse_utils2.o main.o ./parse/gnl/get_next_line.o ./parse/gnl/get_next_line_utils.o
SANITIZE=-fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -Lmlx -lmlx -framework OpenGL -O3 -framework AppKit -o $(NAME) -fsanitize=address

%.o:%.c
	$(CC) -Wall -Wextra -Werror -g -Imlx  -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all