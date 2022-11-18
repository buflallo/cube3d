#include <mlx.h>
#include <stdio.h>

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_vars {
    void	*mlx;
    void	*win;
    t_data  pl_img;
}				t_vars;

float   px, py;

void    initPlayer(t_data  pl_img)
{
    pl_img.img = NULL;
    px = 400;
    py = 300;
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;
    int    offset;

    offset = (y * data->line_length + x * (data->bits_per_pixel / 8));

	dst = data->addr + offset;
	*(unsigned int*)dst = color;
}


void    drawPlayer(void *mlx, void *win, t_data pl_img)
{
    int    x;
    int    y;
    

    x = 0;
    
    if (pl_img.img)
        mlx_destroy_image(mlx, pl_img.img);
    pl_img.img = mlx_new_image(mlx, 1920, 1080);
    pl_img.addr = mlx_get_data_addr(pl_img.img, &pl_img.bits_per_pixel, &pl_img.line_length, &pl_img.endian);
    while (x < 20)
    {
        y = 0;
        while (y < 20)
        {
            my_mlx_pixel_put(&pl_img, px + x, py + y, 0x00FF0000);
            y++;
        }
        x++;
    }
    mlx_put_image_to_window(mlx, win, pl_img.img, 0, 0);
}

int    buttons(int keycode, t_vars *vars)
{
    if (keycode == 126)
        py -= 20;
    if (keycode == 125)
        py += 20;
    if (keycode == 123)
        px -= 20;
    if (keycode == 124)
        px += 20;
    drawPlayer(vars->mlx, vars->win, vars->pl_img);
    return (0);
}

int main(int ac, char *av[])
{
    t_vars  vars;
    

    (void)ac;
    (void)av;

    // Create a new window

	vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 1920, 1080, "Hello world!");
    // vars.pl_img.img = mlx_new_image(vars.mlx, 1920, 1080);
    // vars.pl_img.addr = mlx_get_data_addr(vars.pl_img.img, &vars.pl_img.bits_per_pixel, &vars.pl_img.line_length, &vars.pl_img.endian);
    initPlayer(vars.pl_img);
    // drawPlayer(vars.mlx, vars.win, &vars.pl_img);
    drawPlayer(vars.mlx, vars.win, vars.pl_img);
    mlx_key_hook(vars.win, buttons, &vars);
    mlx_loop(vars.mlx);
    return (0);
}