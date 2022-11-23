#include <mlx.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define arrow_up 65362
#define arrow_down 65364
#define arrow_left 65363
#define arrow_right 65361
#define map_x 1024
#define map_y 768
#define DR PI / 180
#define P2 PI / 2
#define P3 3 * PI / 2

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
    py = map_y / 12 * 3;
    px = map_x / 16 * 1.3;
    pa = 0;
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
    float ra, xo = 0,yo = 0, Tx = 0, Ty = 0, Dist = 0;
    float aTan;

    ra = pa - (30 * DR);
    if (ra < 0)
        ra += 2 * PI;
    if (ra > 2 * PI)
        ra -= 2 * PI;
    for (r=0; r<1020 ; r++)
    {
        //check horizontal lines
        dof = 0;
        aTan = -1 / tan(ra);
        if (ra > PI) 
        {
            // looking up
            *ry = (((int)py >> 6) << 6) - 0.0001;
            *rx = (py - *ry) * aTan + px;
            yo = -64;
            xo = -yo * aTan;
        } 
        if (ra < PI) 
        {
            // looking down
            *ry = (((int)py >> 6) << 6) + 64;
            *rx = (py - *ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
        } 
        if (ra == 0 || ra == PI) 
        {
            // looking straight
            *rx = px;
            *ry = py;
            dof = 12;
        }
        while (dof < 12) 
        {
            mx = (int)(*rx) >> 6;
            my = (int)(*ry) >> 6;
            mp = my * 16 + mx;
            if (mp < 192 && abs(my) < 12 && abs(mx) < 16 && vars->map[abs(my)][abs(mx)] == 1) 
                dof = 12;
            else 
            {
                *rx += xo;
                *ry += yo;
                dof += 1;
            }
        }
        Tx = *rx;
        Ty = *ry;
        //check vertical lines
        dof = 0;
        aTan = -tan(ra);
        if (ra > P2 && ra < P3) 
        {
            //looking left
            *rx = (((int)px >> 6) << 6) - 0.0001;
            *ry = (px - *rx) * aTan + py;
            xo = -64;
            yo = -xo * aTan;
        } 
        if (ra < P2 || ra > P3) 
        {
            // looking right
            *rx = (((int)px >> 6) << 6) + 64;
            *ry = (px - *rx) * aTan + py;
            xo = 64;
            yo = -xo * aTan;
        } 
        if (ra == 0 || ra == PI) 
        {
            // looking up or down
            *rx = px;
            *ry = py;
            dof = 12;
        }
        while (dof < 16) 
        {
            mx = (int)(*rx) >> 6;
            my = (int)(*ry) >> 6;
            mp = my * 16 + mx;
            if (mp < 192 && abs(my) < 12 && abs(mx) < 16 && vars->map[abs(my)][abs(mx)] == 1) 
                dof = 16;
            else 
            {
                *rx += xo;
                *ry += yo;
                dof += 1;
            }
        }
        if (fabs(fabs(py - *ry)/sin(ra)) > fabs(fabs(py - Ty)/sin(ra)))
        {
            //take the closet intersection
            Dist = fabs(fabs(py - Ty)/sin(ra));
            *ry = Ty;
            *rx = Tx ;
        }
        else
            Dist = fabs(fabs(py - *ry)/sin(ra));
        float dx = 0;
        float     dy;
        float   lineH;
        Dist = Dist * cos(pa - ra);
        lineH = (64 * map_y * tan(30 * DR)) / Dist;
        if (lineH > 443.405006738)
            lineH = 443.405006738;
        dy = 0;
        float ofsset;
        while (dy < lineH)
        {
            dx = 0;
            ofsset = (map_y - 445.405006738 - lineH) / 2;
            while (dx <= (map_x / 1020))
            {
                if (*ry != Ty)
                    my_mlx_pixel_put(&vars->pl_img, ((r * (map_x / 1020)) + dx), (map_y - (430.405006738 - ofsset)) + dy, 0x00FF0000);
                else
                    my_mlx_pixel_put(&vars->pl_img, ((r * (map_x / 1020)) + dx), (map_y - (430.405006738 - ofsset)) + dy, 0x00CC0000);
                dx++;
            }
            dy++;
        }
        ra += DR/17;
        if (ra < 0)
            ra += 2 * PI;
        if (ra > 2 * PI)
            ra -= 2 * PI;
    }
}


void    draw_line(t_vars *vars)
{
    float rx;
    float ry;
    // float dx = 0;
    // float i = 0;
    draw_rays(vars, &rx, &ry);
    printf("angle %f\n",pa);
    // if (pa < PI)
    //     i = fabs(py - ry)/sin(pa);
    // else
    //     i = -fabs(py - ry)/sin(pa);
    // printf ("rx = %f ry = %f i = %f\n", px - rx, py - ry, fabs(py - ry)/sin(pa));
    // while (dx < i)
    // {
    //     my_mlx_pixel_put(&vars->pl_img,  px + dx * cos(pa), py + dx * sin(pa), 0x00000000);
    //     dx++;
    // }
}

void    drawPlayer(t_vars *vars)
{
    // int    x;
    // int    y;
    

    // y = 0;
    
    
    // while (y < 20)
    // {
    //     x = 0;
    //     while (x < 20)
    //     {
    //         my_mlx_pixel_put(&vars->pl_img, px + x, py + y, 0x00FF0000);
    //         x++;
    //     }
    //     y++;
    // }
    draw_line(vars);
}


void    drawmap(void *mlx, void *win, t_vars *vars)
{
    // int    x;
    // int    y;
    // int    i;
    // int    j;
    // int    offset[2];
    
    if (vars->pl_img.img)
    {
        mlx_destroy_image(mlx, vars->pl_img.img);
    }
    vars->pl_img.img = mlx_new_image(mlx, map_x, map_y);
    vars->pl_img.addr = mlx_get_data_addr(vars->pl_img.img, &vars->pl_img.bits_per_pixel, &vars->pl_img.line_length, &vars->pl_img.endian);
    // i = 0;
    // offset[1] = 0;
    // while(i < 12)
    // {
    //     j = 0;
    //     offset[0] = 0;
    //     while (j < 16)
    //     {
    //         y = 0;
    //         if (vars->map[i][j] == 1)
    //         {
    //             while (y < map_y / 12)
    //             {
    //                 x = 0;
    //                 while (x < map_x / 16)
    //                 {
    //                     my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x0033FFFF);
    //                     x++;
    //                 }
    //                 y++;
    //             }
    //         }
    //         if (vars->map[i][j] == 0)
    //         {
    //             while (y < map_y / 12)
    //             {
    //                 x = 0;
    //                 while (x < map_x / 16)
    //                 {
    //                     if (y == 0 || y == map_y / 12 - 1 || x == 0 || x == map_x / 16 - 1)
    //                         my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x00000000);
    //                     else
    //                         my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x00FFFFFF);
    //                     x++;
    //                 }
    //                 y++;
    //             }
    //         }
    //         offset[0] += map_x / 16;
    //         j++;
    //     }
    //     offset[1] += map_y / 12;
    //     i++;
    // }
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
            pa += PI * 2;
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
    return (keycode);
}

int render_next_frame(void *vars)
{
    (void)vars;
    return (0);
}

int main(int ac, char *av[])
{
    t_vars  vars;
    int    y;
    int    x;
    // int     c;

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
    // mlx_loop_hook(vars.mlx, render_next_frame(), &vars);
    mlx_key_hook(vars.win, buttons, &vars);
    mlx_loop(vars.mlx);
    return (0);
}