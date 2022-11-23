NAME=cub3d
CC=cc
SRC=main.c
OBJ=main.o
SANITIZE=-fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME) $(SANITIZE)

%.o:%.c
	$(CC) -I/usr/include -Imlx_linux -O3 -g3 -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all