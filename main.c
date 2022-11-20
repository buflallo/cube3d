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
#define map_x 1024
#define map_y 768

float   px, py, pa, pdx, pdy;

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
    py = map_y / 12 * 1;
    px = map_x / 16 * 1;
    pa = PI;
    pdx = cos(pa) * 20;
    pdy = sin(pa) * 20;
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;
    int    offset;

    offset = (y * data->line_length + x * (data->bits_per_pixel / 8));

	dst = data->addr + offset;
	*(unsigned int*)dst = color;
}

void    draw_rays(t_vars *vars, float *rx, float *ry)
{
    // draw rays
    int   r,mx,my,mp,dof;
    float ra, xo = 0,yo;
    float aTan;

    ra = pa;
    for (r=0; r<1 ; r++)
    {
        //check horizontal lines
        dof = 0;
        aTan = -1 / tan(ra);
        if (ra > PI) 
        {
            printf("looking up\n");
            *ry = (((int)py >> 6) << 6) - 0.0001;
            *rx = (py - *ry) * aTan + px;
            yo = -64;
            xo = -yo * aTan;
        } 
        if (ra < PI) 
        {
            printf("looking down\n");
            *ry = (((int)py >> 6) << 6) + 64;
            *rx = (py - *ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
        } 
        if (ra == 0 || ra == PI) 
        {
            printf("looking straight\n");
            *rx = px;
            *ry = py;
            dof = 12;
        }
        while (dof < 12) 
        {
            mx = (int)(*rx) >> 6;
            my = (int)(*ry) >> 6;
            mp = my * 16 + mx;
            printf("mx = %d, my = %d", mx, my);
            printf("\n");
            if (mp < 192 && abs(my) < 12 && abs(mx) < 16 && vars->map[abs(my)][abs(mx)] == 1) 
            {
                dof = 12;
            } 
            else 
            {
                *rx += xo;
                *ry += yo;
                dof += 1;
            }
        }
    }
}


void    draw_line(t_vars *vars)
{
    float rx;
    float ry;
    float dx = 0;
    float i = 0;
    draw_rays(vars, &rx, &ry);
    printf("angle %f\n",pa);
    printf ("rx = %f ry = %f i = %f\n", px - rx, py - ry, fabs(py - ry)/sin(pa));
    if (pa < PI)
        i = fabs(py - ry)/sin(pa);
    else if (i == 0)
    {
        i = fabs(py - ry)/sin(pa/2);
    }
    if (i > 600)
    {
        printf("out of range \n");
        i = 100;
    }
    while (dx < i)
    {
        my_mlx_pixel_put(&vars->pl_img,  px + dx * cos(pa), py + dx * sin(pa), 0x00000000);
        dx++;
    }
}

void    drawPlayer(t_vars *vars)
{
    int    x;
    int    y;
    

    y = 0;
    
    
    while (y < 20)
    {
        x = 0;
        while (x < 20)
        {
            my_mlx_pixel_put(&vars->pl_img, px + x, py + y, 0x00FF0000);
            x++;
        }
        y++;
    }
    draw_line(vars);
}


void    drawmap(void *mlx, void *win, t_vars *vars)
{
    int    x;
    int    y;
    int    i;
    int    j;
    int    offset[2];
    
    if (vars->pl_img.img)
    {
        mlx_destroy_image(mlx, vars->pl_img.img);
    }
    vars->pl_img.img = mlx_new_image(mlx, map_x, map_y);
    vars->pl_img.addr = mlx_get_data_addr(vars->pl_img.img, &vars->pl_img.bits_per_pixel, &vars->pl_img.line_length, &vars->pl_img.endian);
    i = 0;
    offset[1] = 0;
    while(i < 12)
    {
        j = 0;
        offset[0] = 0;
        while (j < 16)
        {
            y = 0;
            if (vars->map[i][j] == 1)
            {
                while (y < map_y / 12)
                {
                    x = 0;
                    while (x < map_x / 16)
                    {
                        my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x0033FFFF);
                        x++;
                    }
                    y++;
                }
            }
            if (vars->map[i][j] == 0)
            {
                while (y < map_y / 12)
                {
                    x = 0;
                    while (x < map_x / 16)
                    {
                        if (y == 0 || y == map_y / 12 - 1 || x == 0 || x == map_x / 16 - 1)
                            my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x00000000);
                        else
                            my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x00FFFFFF);
                        x++;
                    }
                    y++;
                }
            }
            offset[0] += map_x / 16;
            j++;
        }
        offset[1] += map_y / 12;
        i++;
    }
    drawPlayer(vars);
    mlx_put_image_to_window(mlx, win, vars->pl_img.img, 0, 0);
}

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
            pa += PI;
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
    drawmap(vars->mlx, vars->win, vars);
    return (0);
}

int main(int ac, char *av[])
{
    t_vars  vars;
    int    y;
    int    x;

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

    y = 0;
    while (y < 12)
    {
        x = 0;
        while (x < 16)
        {
            vars.map[y][x] = map[y][x];
            x++;
        }
        y++;
    }
	vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, map_x, map_y, "Hello world!");
    // vars.pl_img.img = mlx_new_image(vars.mlx, 1920, 1080);
    // // vars.pl_img.addr = mlx_get_data_addr(vars.pl_img.img, &vars.pl_img.bits_per_pixel, &vars.pl_img.line_length, &vars.pl_img.endian);
    initPlayer(&vars);
    drawmap(vars.mlx, vars.win, &vars);
    // drawPlayer(vars.mlx, vars.win, &vars);
    mlx_key_hook(vars.win, buttons, &vars);
    mlx_loop(vars.mlx);
    return (0);
}