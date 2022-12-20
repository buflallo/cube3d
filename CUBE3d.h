#ifndef CUB3D_H
# define CUB3D_H

#include <mlx.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "parse/cub3d_parse.h"
#include "parse/gnl/get_next_line.h"

#define PI 3.14159265358979323846
#define arrow_up 126
#define arrow_down 125
#define arrow_left 123
#define arrow_right 124
#define map_x 1200
#define map_y 800
#define DR PI / 180
#define P2 PI / 2
#define P3 3 * PI / 2
#define SO 0
#define NO 1
#define EA 2
#define WE 3


typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_rays
{
	int		num_r;
    float   v_rx;
    float   v_ry;
    float   h_ry;
    float   h_rx;
    float   ra;
    float   r_xo;
    float   r_yo;
    float   Dist_H;
    float   Dist_V;
}   t_rays;

typedef struct s_frame
{
    float	lineH;
    float	Dist;
    float	distance;
    float	ofsset;
    int		offsetx;
    float   offsety;
} t_frame;


typedef struct	s_player {
	int	    up;
	int		down;
	int		left;
	int		right;
    float   px;
    float   py;
    float   pa;
    float   pdx;
    float   pdy;
}				t_player;

typedef struct s_texture
{
    char	*file;
    t_data  tex_img;
    int 	*data;
	int		*dir;
	int		img_width;
	int		img_height;
}   t_texture;


typedef struct	s_vars {
    void	*mlx;
    void	*win;
    t_data  pl_img;
    int     map[12][16];
    t_texture   textures[4];
    t_player    player;
    t_parse     parse;
}				t_vars;


void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void    drawPlayer(t_vars *vars);
void    drawmap(void *mlx, void *win, t_vars *vars);
void    draw_rays(t_vars *vars);
int    key_pressed(int keycode, t_vars *vars);
void    move(t_vars *vars);
int render_next_frame(t_vars *vars);
int update(t_vars *vars);
int     init_textures(t_vars *vars, int Dir, char *name);
int key_release(int keycode, t_vars *vars);
void    ft_swap(int *a, int *b);
int key_release(int keycode, t_vars *vars);
void    initplayer(t_vars  *vars);
#endif