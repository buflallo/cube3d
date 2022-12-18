NAME=cub3d
CC=cc
SRC=parse/errors_utils.c parse/main.c parse/parse.c parse/parse_utils.c parse/parse_utils2.c main.c ./parse/gnl/get_next_line.c ./parse/gnl/get_next_line_utils.c
OBJ=parse/errors_utils.o parse/main.o parse/parse.o parse/parse_utils.o parse/parse_utils2.o main.o ./parse/gnl/get_next_line.o ./parse/gnl/get_next_line_utils.o
SANITIZE=-fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)

%.o:%.c
	$(CC) -I/usr/include -Imlx_linux -O3 -g3 -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all