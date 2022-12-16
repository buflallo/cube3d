#include "CUBE3d.h"

void    initPlayer(t_vars  *vars)
{
    vars->pl_img.img = NULL;
    vars->player.py = 64 * vars->parse.y;
    vars->player.px = 64 * vars->parse.x;
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
    int    offset;

    offset = (y * data->line_length + x * (data->bits_per_pixel / 8));

	dst = data->addr + offset;
	*(unsigned int*)dst = color;
}

void    draw_rays(t_vars *vars, float *rx, float *ry)
{
    // draw rays
    int   r,mx,my,dof;
    float ra, xo = 0,yo = 0, Tx = 0, Ty = 0, Dist = 0;
    float aTan;

    ra = vars->player.pa - (30 * DR);
    if (ra < 0)
        ra += 2 * PI;
    if (ra > 2 * PI)
        ra -= 2 * PI;
    for (r=0; r < map_x ; r++)
    {
        //check horizontal lines
        dof = 0;
        aTan = -1 / tan(ra);
        if (ra > PI) 
        {
            // looking up
            *ry = ((int)vars->player.py - ((int)vars->player.py % 64)) - 0.0001;
            *rx = (vars->player.py - *ry) * aTan + vars->player.px;
            yo = -64;
            xo = -yo * aTan;
        } 
        if (ra < PI)
        {
            // looking down
            *ry = ((int)vars->player.py - ((int)vars->player.py % 64)) + 64;
            *rx = (vars->player.py - *ry) * aTan + vars->player.px;
            yo = 64;
            xo = -yo * aTan;
        } 
        if (ra == 0 || ra == PI) 
        {
            // looking straight
            *rx = vars->player.px;
            *ry = vars->player.py;
            dof = vars->parse.height;
        }
        while (dof < vars->parse.height) 
        {
            mx = (int)(*rx) / 64;
            my = (int)(*ry) / 64;
            if (abs(my) < vars->parse.height && abs(mx) < vars->parse.width && vars->parse.map[abs(my)][abs(mx)] == 1) 
                dof = vars->parse.height;
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
            *rx = ((int)vars->player.px - ((int)vars->player.px % 64)) - 0.0001;
            *ry = (vars->player.px - *rx) * aTan + vars->player.py;
            xo = -64;
            yo = -xo * aTan;
        }
        if (ra < P2 || ra > P3) 
        {
            // looking right
            //n9s l modulo d 64
            *rx = ((int)vars->player.px - ((int)vars->player.px % 64)) + 64;
            *ry = (vars->player.px - *rx) * aTan + vars->player.py;
            xo = 64;
            yo = -xo * aTan;
        } 
        if (ra == 0 || ra == PI) 
        {
            // looking up or down
            *rx = vars->player.px;
            *ry = vars->player.py;
            dof = vars->parse.width;
        }
        while (dof < vars->parse.width) 
        {
            mx = (int)(*rx) / 64;
            my = (int)(*ry) / 64;
            if (abs(my) < vars->parse.height && abs(mx) < vars->parse.width && vars->parse.map[abs(my)][abs(mx)] == 1)
                dof = vars->parse.width;
            else
            {
                *rx += xo;
                *ry += yo;
                dof += 1;
            }
        }
        if (fabs(fabs(vars->player.py - *ry)/sin(ra)) > fabs(fabs(vars->player.py - Ty)/sin(ra)))
        {
            //take the closet intersection
            Dist = fabs(fabs(vars->player.py - Ty)/sin(ra));
            *ry = Ty;
            *rx = Tx ;
        }
        else
            Dist = fabs(fabs(vars->player.py - *ry)/sin(ra));
        

        float     dy;
        float   lineH;
        Dist = Dist * cos(vars->player.pa - ra);
        lineH = (64 * map_y * tan(45 * DR)) / Dist;
        // if (lineH > map_y)
        //     lineH = map_y;
        dy = 0;
        float ofsset;
        ofsset = (map_y - lineH) / 2;
        if (lineH > map_y)
            ofsset = 0;
        while (dy < ofsset)
        {
                my_mlx_pixel_put(&vars->pl_img, r,dy, 546511);
            dy++;
        }
        dy = 0;
        int dx;
        dx = 0;
        int offsetx;
        if (*ry != Ty)
        {
            offsetx = (int)(*ry) % 64;
        }   
        else
        {
            offsetx = (int)(*rx) % 64;
        }
        while (dy < lineH && dy < map_y)
        {
            // float distance = (ofsset + dy + ((int)lineH / 2)
				// - (map_y / 2));
		    // float offsety = distance * ((float)64 / (int)lineH);
            if (*ry != Ty)
            {
                if (ra > P2 && ra < P3)
                {
                    // my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, vars->textures[WE].data[(int)(((int)offsety) * 64 + offsetx)]);
                    my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, 0xFF5733);
                }
                else
                {
                    my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, 0xA569BD);
                    // my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, vars->textures[EA].data[(int)(((int)offsety) * 64 + offsetx)]);
                }
            }
            else
            {
                if (ra > PI)
                    my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, 0xDAF7A6);
                    // my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, vars->textures[SO].data[(int)(((int)offsety) * 64 + offsetx)]);
                else
                    my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, 0xFFC300);
                    // my_mlx_pixel_put(&vars->pl_img, r,ofsset + dy, vars->textures[NO].data[(int)(((int)offsety) * 64 + offsetx)]);
            }
            dy++;
        }
        // draw floor
        dy = 0;
        while ((ofsset + lineH + dy) < map_y)
        {
            my_mlx_pixel_put(&vars->pl_img, r, ofsset + lineH + dy, 0x00FF00);
            dy++;
        }
        // int dx;
        // dx = 0;
        // int i = 0;
        // if (ra < PI)
        //     i = fabs(vars->player.py - *ry)/sin(ra);
        // else
        //     i = -fabs(vars->player.py - *ry)/sin(ra);
        // // printf ("rx = %f *ry = %f i = %f\n", vars->player.px - *rx, vars->player.py - *ry, fabs(vars->player.py - *ry)/sin(vars->player.pa));
        // while (dx < i)
        // {
        //     my_mlx_pixel_put(&vars->pl_img,  vars->player.px + dx * cos(ra), vars->player.py + dx * sin(ra), 0x00000000);
        //     dx++;
        // }
        ra += DR/(map_x / 60);
        if (ra < 0)
            ra += 2 * PI;
        if (ra > 2 * PI)
            ra -= 2 * PI;
    }
}


void    draw_line(t_vars *vars)
{
    float rx = 0;
    float ry = 0;
    // float dx = 0;
    // float i = 0;
    draw_rays(vars, &rx, &ry);
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
    while(i < vars->parse.height)
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

    // draw_line(vars);
    mlx_put_image_to_window(mlx, win, vars->pl_img.img, 0, 0);
}

void    ft_swap(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

int    key_pressed(int keycode, t_vars *vars)
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

int key_release(int keycode, t_vars *vars)
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

void    move_up_down(t_player *player, t_parse parse, int xo, int yo)
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

void    move_up_down(t_player *player, int xo, int yo)
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
    int xo, yo;
    xo = 0;
    yo = 0;
    if (vars->player.pdx < 0)
        xo = -5;
    else
        xo = 5;
    if (vars->player.pdy < 0)
        yo = -5;
    else
        yo = 5;
    move_up_down(&vars->player, vars->parse, xo, yo);
    move_left_right(&vars->player, xo, yo);
}

int render_next_frame(t_vars *vars)
{
    drawmap(vars->mlx, vars->win, vars);
    return (0);
}

int update(t_vars *vars)
{
    mlx_hook(vars->win, 2, 1L << 0, key_pressed, vars);
	mlx_hook(vars->win, 3, 1L << 1, key_release, vars);
    move(vars);
    render_next_frame(vars);
    return (1);
}

int     init_textures(t_vars *vars, int Dir, char *name)
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
    t_vars  vars;
    t_parse  parse;
    // int     c;

    (void)ac;
	vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, map_x, map_y, "Hello world!");
    // vars.pl_img.img = mlx_new_image(vars.mlx, 1920, 1080);
    // // vars.pl_img.addr = mlx_get_data_addr(vars.pl_img.img, &vars.pl_img.bits_per_pixel, &vars.pl_img.line_length, &vars.pl_img.endian);
    parse_map(av, &parse);
    vars.parse = parse;
    initPlayer(&vars);
    // init_textures(&vars, NO, parse.no);
    // init_textures(&vars, SO, parse.so);
    // init_textures(&vars, WE, parse.we);
    // init_textures(&vars, EA, parse.ea);
    drawmap(vars.mlx, vars.win, &vars);
    // drawPlayer(vars.mlx, vars.win, &vars);
    // mlx_loop_hook(vars.mlx, render_next_frame(), &vars);
    mlx_loop_hook(vars.mlx, update, &vars);
    mlx_loop(vars.mlx);
    return (0);
}
