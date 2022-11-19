#include <mlx.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define arrow_up 65362
#define arrow_down 65364
#define arrow_left 65361
#define arrow_right 65363


float   px = 600, py = 400, pa = 0, pdx = 20, pdy = 0;

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
    t_data  mp_img;
    int     p_x;
    int     p_y;
    int     map[12][16];
}				t_vars;


void    initPlayer(t_vars  *vars)
{
    vars->pl_img.img = NULL;
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;
    int    offset;

    offset = (y * data->line_length + x * (data->bits_per_pixel / 8));

	dst = data->addr + offset;
	*(unsigned int*)dst = color;
}


void    drawPlayer(void *mlx, void *win, t_vars *vars)
{
    int    x;
    int    y;
    

    x = 0;
    
    if (vars->pl_img.img)
    {
        mlx_destroy_image(mlx, vars->pl_img.img);
    }
    vars->pl_img.img = mlx_new_image(mlx, 900, 800);
    vars->pl_img.addr = mlx_get_data_addr(vars->pl_img.img, &vars->pl_img.bits_per_pixel, &vars->pl_img.line_length, &vars->pl_img.endian);
    while (x < 20)
    {
        y = 0;
        while (y < 20)
        {
            my_mlx_pixel_put(&vars->pl_img, px + x, py + y, 0x00FF0000); // 600 400 
            y++;
        }
        x++;
    }
    // put line from 600 400 to 620 420
    //                                                                                 .
    //                                                                                .
    //                                                                               .
    //                                                                              .
    //                                                                             .
    //                                                                            .
    //                                                                  ..........
    //                                                                  ..........
    //                                                                  ..........
    //                                                                  ..........
    //                                                                  ..........
    x = 0;
    while (x < 100)
    {
        printf ("pdx = %d, pdy = %d\n", (int)fabs(pdx), (int)fabs(pdy));
        my_mlx_pixel_put(&vars->pl_img,  px + x * cos(pa) + 10, py + x * sin(pa) + 10, 0x00FFFFFF);
        x++;
    }
    mlx_put_image_to_window(mlx, win, vars->pl_img.img, 0, 0);
}

// void    drawmap(void *mlx, void *win, t_vars *vars)
// {
//     int    x;
//     int    y;
//     int    i;
//     int    j;
//     int    offset[2];
    
//     vars->mp_img.img = mlx_new_image(mlx, 1920, 1080);
//     vars->mp_img.addr = mlx_get_data_addr(vars->mp_img.img, &vars->mp_img.bits_per_pixel, &vars->mp_img.line_length, &vars->mp_img.endian);
//     i = 0;
//     offset[1] = 0;
//     while(i < 12)
//     {
//         j = 0;
//         offset[0] = 0;
//         while (j < 16)
//         {
//             x = 0;
//             if (vars->map[i][j] == 1)
//             {
//                 printf("1");
//                 while (x < 1920 / 16)
//                 {
//                     y = 0;
//                     while (y < 1080 / 12)
//                     {
//                         my_mlx_pixel_put(&vars->mp_img, offset[0] + x, offset[1] + y, 0x0033FFFF);
//                         y++;
//                     }
//                     x++;
//                 }
//             }
//             if (vars->map[i][j] == 0)
//             {
//                 printf("0");
//                 while (x < 1920 / 16)
//                 {
//                     y = 0;
//                     while (y < 1080 / 12)
//                     {
//                         if (y == 0 || y == 1080 / 12 - 1 || x == 0 || x == 1920 / 16 - 1)
//                             my_mlx_pixel_put(&vars->mp_img, offset[0] + x, offset[1] + y, 0x00000000);
//                         else
//                             my_mlx_pixel_put(&vars->mp_img, offset[0] + x, offset[1] + y, 0x00FFFFFF);
//                         y++;
//                     }
//                     x++;
//                 }
//             }
//             offset[0] += 1920 / 16;
//             j++;
//         }
//         printf("\n");
//         offset[1] += 1080 / 12;
//         i++;
//     }
//     mlx_put_image_to_window(mlx, win, vars->mp_img.img, 0, 0);
//     drawPlayer(vars->mlx, vars->win, vars);
// }

void    ft_swap(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

int    buttons(int keycode, t_vars *vars)
{
    printf("keycode = %d\n", keycode);
    // if (keycode == 126 && vars->map[(int)((py / (1080 / 12)) - (20 / (float)(1080 / 12)) )][(int)(px / (1920 / 16))] == 0)
    if (keycode == arrow_down)
    {
        px -= pdx;
        py -= pdy;
    }
    // if (keycode == 125 && vars->map[(int)((py / (1080 / 12)) + (20 / (float)(1080 / 12)) )][(int)(px / (1920 / 16))] == 0)
    if (keycode == arrow_up)
    {
        px += pdx;
        py += pdy;
    }
    // if (keycode == 123 && vars->map[(int)(py / (1080 / 12))][(int)((px / (1920 / 16)) - (20 / (float)(1920 / 16)))] == 0)
    if (keycode == arrow_left)
    {
        pa -= 0.1;
        if (pa < 0)
            pa += 2 * PI;
        pdx = cos(pa) * 20;
        pdy = sin(pa) * 20;
    }
    // if (keycode == 124 && vars->map[(int)(py / (1080 / 12))][(int)((px / (1920 / 16)) + (20 / (float)(1920 / 16)))] == 0)
    if (keycode == arrow_right)
    {
        pa += 0.1;
        if (pa > 2 * PI)
            pa -= 2 * PI;
        pdx = cos(pa) * 20;
        pdy = sin(pa) * 20;
    }
    drawPlayer(vars->mlx, vars->win, vars);
    return (0);
}

int main(int ac, char *av[])
{
    t_vars  vars;
    int    i;
    int    j;

    (void)ac;
    (void)av;
    int map[12][16]=
    {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,1,1,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    i = 0;
    while (i < 12)
    {
        j = 0;
        while (j < 16)
        {
            vars.map[i][j] = map[i][j];
            j++;
        }
        i++;
    }
	vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 900, 800, "Hello world!");
    // vars.pl_img.img = mlx_new_image(vars.mlx, 1920, 1080);
    // // vars.pl_img.addr = mlx_get_data_addr(vars.pl_img.img, &vars.pl_img.bits_per_pixel, &vars.pl_img.line_length, &vars.pl_img.endian);
    initPlayer(&vars);
    // drawmap(vars.mlx, vars.win, &vars);
    drawPlayer(vars.mlx, vars.win, &vars);
    mlx_key_hook(vars.win, buttons, &vars);
    mlx_loop(vars.mlx);
    return (0);
}