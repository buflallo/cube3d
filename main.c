/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 02:42:07 by hlachkar          #+#    #+#             */
/*   Updated: 2022/12/20 02:42:13 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CUBE3d.h"

void	initplayer(t_vars *vars)
{
	vars->pl_img.img = NULL;
	vars->player.py = 64 * vars->parse.y + 32;
	vars->player.px = 64 * vars->parse.x + 32;
	vars->player.pa = 0;
	vars->player.down = 0;
	vars->player.up = 0;
	vars->player.left = 0;
	vars->player.right = 0;
	vars->player.pdx = cos(vars->player.pa) * 3;
	vars->player.pdy = sin(vars->player.pa) * 3;
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;
	int		offset;

	offset = (y * data->line_length + x * (data->bits_per_pixel / 8));
	dst = data->addr + offset;
	*(unsigned int *)dst = color;
}

void	search_h_inter(t_rays *rays, t_vars *vars, int x)
{
	int		mx;
	int		my;
	int		dof;

	dof = x;
	while (dof < vars->parse.height) 
	{
		mx = (int)(rays->h_rx) / 64;
		my = (int)(rays->h_ry) / 64;
		if (abs(my) < vars->parse.height && abs(mx) < vars->parse.width && vars->parse.map[abs(my)][abs(mx)] == 1) 
			dof = vars->parse.height;
		else 
		{
			rays->h_rx += rays->r_xo;
			rays->h_ry += rays->r_yo;
			dof += 1;
		}
	}
	rays->Dist_H = fabs(fabs(vars->player.py - rays->h_ry)/sin(rays->ra));
}

void	search_v_inter(t_rays *rays, t_vars *vars, int x)
{
	int		mx;
	int		my;
	int		dof;

	dof = x;
	while (dof < vars->parse.width)
	{
		mx = (int)(rays->v_rx) / 64;
		my = (int)(rays->v_ry) / 64;
		if (abs(my) < vars->parse.height && abs(mx) < vars->parse.width && vars->parse.map[abs(my)][abs(mx)] == 1) 
			dof = vars->parse.width;
		else
		{
			rays->v_rx += rays->r_xo;
			rays->v_ry += rays->r_yo;
			dof += 1;
		}
	}
	rays->Dist_V = fabs(fabs(vars->player.py - rays->v_ry)/sin(rays->ra));
}

void	ray_up_or_down(t_rays *rays, char dir, t_vars *vars)
{
	float	atan;

	atan = -1 / tan(rays->ra);
	if (dir == 'u')
	{
		rays->h_ry = ((int)vars->player.py - ((int)vars->player.py % 64)) - 0.01;
		rays->r_yo = -64;
	}
	else
	{
		rays->h_ry = ((int)vars->player.py - ((int)vars->player.py % 64)) + 64;
		rays->r_yo = 64;
	}
	rays->h_rx = (vars->player.py - rays->h_ry) * atan + vars->player.px;
	rays->r_xo = -rays->r_yo * atan;
	if (dir == 's')
	{
		rays->h_rx = vars->player.px;
		rays->h_ry = vars->player.py;
		search_h_inter(rays, vars, vars->parse.height);
		return ;
	}
	search_h_inter(rays, vars, 0);
}

void	ray_lef_or_right(t_rays *rays, char dir, t_vars *vars)
{
	float	atan;

	atan = -tan(rays->ra);
	if (dir == 'l')
	{
		rays->v_rx = ((int)vars->player.px - ((int)vars->player.px % 64)) - 0.01;
		rays->r_xo = -64;
	}
	else
	{
		rays->v_rx = ((int)vars->player.px - ((int)vars->player.px % 64)) + 64;
		rays->r_xo = 64;
	}
	rays->v_ry = (vars->player.px - rays->v_rx) * atan + vars->player.py;
	rays->r_yo = -rays->r_xo * atan;
	if (dir == 's')
	{
		rays->v_rx = vars->player.px;
		rays->v_ry = vars->player.py;
		search_v_inter(rays, vars, vars->parse.width);
		return ;
	}
	search_v_inter(rays, vars, 0);
}

void	init_ray_values(t_rays *rays, char dir, t_vars *vars)
{
	if (dir == 'u' || dir == 'd' || dir == 's')
		ray_up_or_down(rays, dir, vars);
	if (dir == 'l' || dir == 'r' || dir == 's')
		ray_lef_or_right(rays, dir, vars);
}

t_frame	*init_frame(t_vars *vars, t_rays *rays, float dist)
{
	t_frame	*frame;

	frame = (t_frame *)malloc(sizeof(t_frame));
	frame->Dist = dist;
	frame->Dist = frame->Dist * cos(vars->player.pa - rays->ra);
	frame->lineH = (64 * map_y * tan(45 * DR)) / frame->Dist;
	frame->ofsset = (map_y - frame->lineH) / 2;
	if (frame->lineH > map_y)
		frame->ofsset = 0;
	if (rays->Dist_V > rays->Dist_H)
		frame->offsetx = (int)(rays->h_rx) % 64;
	else
		frame->offsetx = (int)(rays->v_ry) % 64;
	return (frame);
}

void	draw_celling(t_data *img, int x, int y)
{
	int	dy;

	dy = 0;
	while (dy < y)
	{
		my_mlx_pixel_put(img, x, dy, 546511);
		dy++;
	}
}

void	draw_floor(t_data *img, int x, int y)
{
	int	dy;

	dy = y;
	while (dy > 0 && dy < map_y)
	{
		my_mlx_pixel_put(img, x, dy, 0x00ff00);
		dy++;
	}
}

void	draw_wall(t_vars *vars, t_rays *rays, t_frame *frame)
{
	int	dy;

	dy = 0;
	while (dy < frame->lineH && dy < map_y)
	{
		frame->distance = (frame->ofsset + dy + ((int)frame->lineH / 2) - (map_y / 2));
		frame->offsety = frame->distance * ((float)64 / (int)frame->lineH);
		if (rays->Dist_V < rays->Dist_H)
		{
			if (rays->ra > P2 && rays->ra < P3)
				my_mlx_pixel_put(&vars->pl_img, rays->num_r,frame->ofsset + dy, vars->textures[WE].data[(int)(((int)frame->offsety) * 64 + frame->offsetx)]);
			else
				my_mlx_pixel_put(&vars->pl_img, rays->num_r,frame->ofsset + dy, vars->textures[EA].data[(int)(((int)frame->offsety) * 64 + frame->offsetx)]);
		}
		else
		{
			if (rays->ra > PI)
				my_mlx_pixel_put(&vars->pl_img, rays->num_r,frame->ofsset + dy, vars->textures[SO].data[(int)(((int)frame->offsety) * 64 + frame->offsetx)]);
			else
				my_mlx_pixel_put(&vars->pl_img, rays->num_r,frame->ofsset + dy, vars->textures[NO].data[(int)(((int)frame->offsety) * 64 + frame->offsetx)]);
		}
		dy++;
	}
}

void	draw_frame(t_rays *rays, t_vars *vars, float dist)
{
	int			dx;
	t_frame		*frame;

	frame = init_frame(vars, rays, dist);
	draw_celling(&vars->pl_img, rays->num_r, frame->ofsset);
	draw_wall(vars, rays, frame);
	draw_floor(&vars->pl_img, rays->num_r, frame->ofsset + frame->lineH);
}

void	check_angle(t_rays *rays)
{
	if (rays->ra < 0)
		rays->ra += 2 * PI;
	if (rays->ra > 2 * PI)
		rays->ra -= 2 * PI;
}

void	draw_rays(t_vars *vars)
{
	t_rays	*rays;

	rays = (t_rays *)malloc(sizeof(t_rays));
	rays->ra = vars->player.pa - (30 * DR);
	check_angle(rays);
	for (rays->num_r=0; rays->num_r < map_x ; rays->num_r++)
	{
		if (rays->ra > PI) 
			init_ray_values(rays, 'u', vars);
		if (rays->ra < PI)
			init_ray_values(rays, 'd', vars);
		if (rays->ra == 0 || rays->ra == PI) 
			init_ray_values(rays, 's', vars);
		if (rays->ra > P2 && rays->ra < P3) 
			init_ray_values(rays, 'l', vars);
		if (rays->ra < P2 || rays->ra > P3) 
			init_ray_values(rays, 'r', vars);
		if (rays->ra == 0 || rays->ra == PI) 
			init_ray_values(rays, 's', vars);
		if (rays->Dist_H < rays->Dist_V)
			draw_frame(rays, vars, rays->Dist_H);
		else
			draw_frame(rays, vars, rays->Dist_V);
		rays->ra += DR/(map_x / 60);
		check_angle(rays);
	}
}


void    draw_line(t_vars *vars)
{
	draw_rays(vars);
	// if (vars->player.pa < PI)
	//     i = fabs(vars->player.py - ry)/sin(vars->player.pa);
	// else
	//     i = -fabs(vars->player.py - ry)/sin(vars->player.pa);
	// printf ("rx = %f ry = %f i = %f\n", vars->player.px - rx, vars->player.py - ry, fabs(vars->player.py - ry)/sin(vars->player.pa));
	// while (dx < i)
	// {
	//     my_mlx_pixel_put(&vars->pl_img,  vars->player.px + dx * cos(vars->player.pa), vars->player.py + dx * sin(vars->player.pa), 0x00000000);
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
	//         my_mlx_pixel_put(&vars->pl_img, vars->player.px + x, vars->player.py + y, 0x00FF0000);
	//         x++;
	//     }
	//     y++;
	// }
	draw_line(vars);
}


void    drawmap(void *mlx, void *win, t_vars *vars)
{
	int    x;
	int    y;
	int    i;
	int    j;
	int    minimapx;
	int    minimapy;
	int    offset[2];

	if (vars->pl_img.img)
	{
		mlx_destroy_image(mlx, vars->pl_img.img);
	}
	vars->pl_img.img = mlx_new_image(mlx, map_x, map_y);
	vars->pl_img.addr = mlx_get_data_addr(vars->pl_img.img, &vars->pl_img.bits_per_pixel, &vars->pl_img.line_length, &vars->pl_img.endian);
	drawPlayer(vars);
	i = 0;
	offset[1] = 0;
	minimapx = 200;
	minimapy = 200;
	while (i < vars->parse.height)
	{
		j = 0;
		offset[0] = 0;
		while (j < vars->parse.width)
		{
			y = 0;
			if (vars->parse.map[i][j] == 1)
			{
				while (y < 200 / 64)
				{
					x = 0;
					while (x < 200 / 64)
					{
						my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x0033FFFF);
						x++;
					}
					y++;
				}
			}
			if (vars->parse.map[i][j] == 0)
			{
				while (y < 200 / 64)
				{
					x = 0;
					while (x < 200 / 64)
					{
						my_mlx_pixel_put(&vars->pl_img, offset[0] + x, offset[1] + y, 0x00FFFFFF);
						x++;
					}
					y++;
				}
			}
			offset[0] += 200 / 64;
			j++;
		}
		offset[1] += 200 / 64;
		i++;
	}

	y = 0;


	// while (y < 20)
	// {
	//     x = 0;
	//     while (x < 20)
	//     {
	//         my_mlx_pixel_put(&vars->pl_img, vars->player.px + x, vars->player.py + y, 0x00FF0000);
	//         x++;
	//     }
	//     y++;
	// }

	mlx_put_image_to_window(mlx, win, vars->pl_img.img, 0, 0);
}

void	ft_swap(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	key_pressed(int keycode, t_vars *vars)
{
	if (keycode == arrow_down)
		vars->player.down = 1;
	if (keycode == arrow_up)
		vars->player.up = 1;
	if (keycode == arrow_left)
		vars->player.left = 1;
	if (keycode == arrow_right)
		vars->player.right = 1;
	return (keycode);
}

int	key_release(int keycode, t_vars *vars)
{
	if (keycode == arrow_down)
		vars->player.down = 0;
	if (keycode == arrow_up)
		vars->player.up = 0;
	if (keycode == arrow_left)
		vars->player.left = 0;
	if (keycode == arrow_right)
		vars->player.right = 0;
	if (keycode == 53)
		exit(0);
	return (keycode);
}

void	move_up_down(t_player *player, t_parse parse, int xo, int yo)
{
	if (player->down)
	{
		if (parse.map[(int)(player->py - yo) / 64][(int)player->px / 64] == 0)
			player->py -= player->pdy;
		if (parse.map[(int)player->py / 64][(int)(player->px - xo) / 64] == 0)
			player->px -= player->pdx;
	}
	if (player->up)
	{
		if (parse.map[(int)(player->py + yo) / 64][(int)player->px / 64] == 0)
			player->py += player->pdy;
		if (parse.map[(int)player->py / 64][(int)(player->px + xo) / 64] == 0)
			player->px += player->pdx;
	}
}

void	move_left_right(t_player *player, int xo, int yo)
{
	if (player->left)
	{
		player->pa -= 0.05;
		if (player->pa < 0)
			player->pa += PI * 2;
		player->pdx = cos(player->pa) * 3;
		player->pdy = sin(player->pa) * 3;
	}
	if (player->right)
	{
		player->pa += 0.05;
		if (player->pa > 2 * PI)
			player->pa -= 2 * PI;
		player->pdx = cos(player->pa) * 3;
		player->pdy = sin(player->pa) * 3;
	}
}

void    move(t_vars *vars)
{
	int	xo;
	int	yo;

	xo = 0;
	yo = 0;
	if (vars->player.pdx < 0)
		xo = -10;
	else
		xo = 10;
	if (vars->player.pdy < 0)
		yo = -10;
	else
		yo = 10;
	move_up_down(&vars->player, vars->parse, xo, yo);
	move_left_right(&vars->player, xo, yo);
}

int render_next_frame(t_vars *vars)
{
	drawmap(vars->mlx, vars->win, vars);
	return (0);
}

int	update(t_vars *vars)
{
	mlx_hook(vars->win, 2, 1L << 0, key_pressed, vars);
	mlx_hook(vars->win, 3, 1L << 1, key_release, vars);
	move(vars);
	render_next_frame(vars);
	return (1);
}

int	init_textures(t_vars *vars, int Dir, char *name)
{
	vars->textures[Dir].file = name;
	vars->textures[Dir].tex_img.img = mlx_xpm_file_to_image(vars->mlx, vars->textures[Dir].file, &vars->textures[Dir].img_width, &vars->textures[Dir].img_height);
	if (!vars->textures[Dir].tex_img.img)
		errors(11);
	vars->textures[Dir].data = (int *)mlx_get_data_addr(vars->textures[Dir].tex_img.img, &vars->textures[Dir].tex_img.bits_per_pixel, &vars->textures[Dir].tex_img.line_length, &vars->textures[Dir].tex_img.endian);
	return (1);
}

int main(int ac, char *av[])
{
	t_vars	vars;
	t_parse	*parse;

	(void)ac;
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, map_x, map_y, "Hello world!");
	vars.pl_img.img = mlx_new_image(vars.mlx, 1920, 1080);
	parse = (t_parse *)malloc(sizeof(t_parse));
	parse_map(ac, av, parse);
	vars.parse = *parse;
	initplayer(&vars);
	init_textures(&vars, NO, parse->no);
	init_textures(&vars, SO, parse->so);
	init_textures(&vars, WE, parse->we);
	init_textures(&vars, EA, parse->ea);
	mlx_loop_hook(vars.mlx, update, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
