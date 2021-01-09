#include "wolf3D.h"

int           init(t_sdl *sdl, t_raycaster *rc)
{
  sdl->window = NULL;
  sdl->renderer = NULL;
  rc->player_pos_x = 22;
  rc->player_pos_y = 12;
  rc->player_dir_x = -1;
  rc->player_dir_y = 0;
  rc->player_plane_x = 0;
  rc->player_plane_y = 0.66;
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    fprintf(stderr,"SDL initialization failed (%s)\n",SDL_GetError());
    return (-1);
  }
  if (SDL_CreateWindowAndRenderer(WIN_X, WIN_Y, 0, &sdl->window, &sdl->renderer) != 0)
  {
    fprintf(stderr,"Window creation failed (%s)\n",SDL_GetError());
    return (-1);
  }
  return (0);
}

void          initial_calc(t_raycaster *rc, int x)
{
  double      camera_x;

  camera_x = 2 * x / (double)(WIN_X) - 1;
  rc->ray_dir_x = rc->player_dir_x + rc->player_plane_x * camera_x;
  rc->ray_dir_y = rc->player_dir_y + rc->player_plane_y * camera_x;
  rc->map_x = (int)(rc->player_pos_x);
  rc->map_y = (int)(rc->player_pos_y);
  rc->delta_dist_x = sqrt(1 + pow(rc->ray_dir_y, 2) / pow(rc->ray_dir_x, 2));
  rc->delta_dist_y = sqrt(1 + pow(rc->ray_dir_x, 2) / pow(rc->ray_dir_y, 2));
  if (rc->ray_dir_x < 0)
  {
    rc->step_x = -1;
    rc->side_dist_x = (rc->player_pos_x - rc->map_x) * rc->delta_dist_x;
  }
  else
  {
    rc->step_x = 1;
    rc->side_dist_x = (rc->map_x + 1.0 - rc->player_pos_x) * rc->delta_dist_x;
  }
  if (rc->ray_dir_y < 0)
  {
    rc->step_y = -1;
    rc->side_dist_y = (rc->player_pos_y - rc->map_y) * rc->delta_dist_y;
  }
  else
  {
    rc->step_y = 1;
    rc->side_dist_y = (rc->map_y + 1.0 - rc->player_pos_y) * rc->delta_dist_y;
  }
}

void          perform_dda(t_raycaster *rc)
{
  int         hit;

  hit = 0;
  while (hit == 0)
  {
    if (rc->side_dist_x < rc->side_dist_y)
    {
      rc->side_dist_x += rc->delta_dist_x;
      rc->map_x += rc->step_x;
      rc->side = 0;
    }
    else
    {
      rc->side_dist_y += rc->delta_dist_y;
      rc->map_y += rc->step_y;
      rc->side = 1;
    }
    if (rc->map[rc->map_x][rc->map_y] > 0)
      hit = 1;
  }
}

void          calc_wall_height(t_raycaster *rc)
{
  int         line_height;

  if (rc->side == 0)
    rc->perp_wall_dist = (rc->map_x - rc->player_pos_x + (1 - rc->step_x) / 2) / rc->ray_dir_x;
  else
    rc->perp_wall_dist = (rc->map_y - rc->player_pos_y + (1 - rc->step_y) / 2) / rc->ray_dir_y;
  line_height = (int)(WIN_Y / rc->perp_wall_dist);
  rc->draw_start = -line_height / 2 + WIN_Y / 2;
  if (rc->draw_start < 0)
    rc->draw_start = 0;
  rc->draw_end = line_height / 2 + WIN_Y / 2;
  if (rc->draw_end >= WIN_Y)
    rc->draw_end = WIN_Y - 1;
}

void draw(t_raycaster *rc, t_sdl *sdl) {
    int i;
    int j;
    SDL_Rect rect;
    SDL_Rect pla;
        pla.x = rc->player_pos_x * 6;
        pla.y = rc->player_pos_y * 6;
        pla.h = 6;
        pla.w = 6;
    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(sdl->renderer,pla.x + 3, pla.y + 3, rc->map_x*6,rc->map_y*6);
    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 255, 255);
    i = 0;
    while (i < 24)
        {
            j = 0;
            while (j < 24)
            {
                if (rc->map[j][i] > 0)
                {
                    rect.y = i * 6;
                    rect.x = j * 6 ;
                    rect.w = 6;
                    rect.h = 6;
                    SDL_RenderFillRect(sdl->renderer, &rect);
                }
                j++;     
            }
            i++;
        }
        SDL_SetRenderDrawColor(sdl->renderer, 255, 33, 255, 255);
        SDL_RenderFillRect(sdl->renderer, &pla);
}

void          draw_vert_line(t_sdl *sdl, t_raycaster *rc, int x)
{
  SDL_Color   color;
  
  color = select_wall_color(rc->map_x, rc->map_y, rc);
  SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(sdl->renderer, x, rc->draw_start, x, rc->draw_end);

}

void          render_frame(t_sdl *sdl)
{
  SDL_RenderPresent(sdl->renderer);
  SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(sdl->renderer);
}

int           handle_events(t_raycaster *rc)
{
  SDL_Event   event;
  double      oldDirX;
  double      oldPlaneX;

  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
      return (-1);
    if (event.type == SDL_KEYDOWN)
    {
      if (event.key.keysym.sym == SDLK_UP)
      {
        if(rc->map[(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0) rc->player_pos_x += rc->player_dir_x * MV_SPEED;
        if(rc->map[(int)(rc->player_pos_x)][(int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)] == 0) rc->player_pos_y += rc->player_dir_y * MV_SPEED;
      }
      if (event.key.keysym.sym == SDLK_DOWN)
      {
        if(rc->map[(int)(rc->player_pos_x - rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0) rc->player_pos_x -= rc->player_dir_x * MV_SPEED;
        if(rc->map[(int)(rc->player_pos_x)][(int)(rc->player_pos_y - rc->player_dir_y * MV_SPEED)] == 0) rc->player_pos_y -= rc->player_dir_y * MV_SPEED;
      }
      if (event.key.keysym.sym == SDLK_RIGHT)
      {
        oldDirX = rc->player_dir_x;
        rc->player_dir_x = rc->player_dir_x * cos(-ROT_SPEED) - rc->player_dir_y * sin(-ROT_SPEED);
        rc->player_dir_y = oldDirX * sin(-ROT_SPEED) + rc->player_dir_y * cos(-ROT_SPEED);
        oldPlaneX = rc->player_plane_x;
        rc->player_plane_x = rc->player_plane_x * cos(-ROT_SPEED) - rc->player_plane_y * sin(-ROT_SPEED);
        rc->player_plane_y = oldPlaneX * sin(-ROT_SPEED) + rc->player_plane_y * cos(-ROT_SPEED);
      }
      if (event.key.keysym.sym == SDLK_LEFT)
      {
        oldDirX = rc->player_dir_x;
        rc->player_dir_x = rc->player_dir_x * cos(ROT_SPEED) - rc->player_dir_y * sin(ROT_SPEED);
        rc->player_dir_y = oldDirX * sin(ROT_SPEED) + rc->player_dir_y * cos(ROT_SPEED);
        oldPlaneX = rc->player_plane_x;
        rc->player_plane_x = rc->player_plane_x * cos(ROT_SPEED) - rc->player_plane_y * sin(ROT_SPEED);
        rc->player_plane_y = oldPlaneX * sin(ROT_SPEED) + rc->player_plane_y * cos(ROT_SPEED);
      }
      
    }
  }
  return (0);
}


void          raycaster(t_sdl *sdl, t_raycaster *rc)
{
  SDL_bool    done;

  done = SDL_FALSE;
  while(!done)
  {
    int x;

    x = -1;
    while(++x < WIN_X)
    {
      initial_calc(rc, x);
      perform_dda(rc);
      calc_wall_height(rc);
      draw(rc, sdl);
      draw_vert_line(sdl, rc, x);
    }
    render_frame(sdl);
    if (handle_events(rc) != 0)
      done = SDL_TRUE;
  }
}

int           main(int ac, char **av)
{
  t_sdl       sdl;
  t_raycaster rc;

  if(ac == 2)
  {
    if(!(rc.map = read_map(av[1])))
      {
        puts("Error, in map // -> map[24][24]!!!!!!");
        return (-1);
      }
    if (init(&sdl, &rc) != 0)
      return (-1);
    raycaster(&sdl, &rc);
    if (sdl.renderer)
      SDL_DestroyRenderer(sdl.renderer);
    if (sdl.window)
      SDL_DestroyWindow(sdl.window);
    SDL_Quit();
  }
  else
    puts("Error, not find the map!!!!!!");
  return (0);
}