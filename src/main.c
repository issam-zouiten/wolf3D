#include "../hds/wolf3D.h"

void imgs(t_raycaster *rc)
{
  rc->wolf = (char **)malloc(sizeof(char *) * 8 );
  rc->wolf[0] = ft_strdup("pics/wood.png") ;
  rc->wolf[1] = ft_strdup("pics/colorstone.png");
  rc->wolf[2] = ft_strdup("pics/eagle.png");
  rc->wolf[3] = ft_strdup("pics/bluestone.png");
  rc->wolf[4] = ft_strdup("pics/pillar.png");
  rc->wolf[5] = ft_strdup("pics/mossy.png");
  rc->wolf[6] = ft_strdup("pics/purplestone.png");
  rc->wolf[7] = ft_strdup("pics/redbrick.png");
}

void tex2(t_raycaster *rc, t_sdl *sdl)
{
  sdl->image = IMG_Load(rc->wolf[0]);
  rc->tex[0] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[0] = (int *)sdl->image->pixels;

  sdl->image = IMG_Load(rc->wolf[1]);
  rc->tex[1] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[1] = (int *)sdl->image->pixels;

  sdl->image = IMG_Load(rc->wolf[2]);
  rc->tex[2] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[2] = (int *)sdl->image->pixels;

  sdl->image = IMG_Load(rc->wolf[3]);
  rc->tex[3] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[3] = (int *)sdl->image->pixels;

  sdl->image = IMG_Load(rc->wolf[4]);
  sdl->texture = SDL_CreateTextureFromSurface(sdl->renderer, sdl->image);
  rc->tex[4] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[4] = (int *)sdl->image->pixels;

  sdl->image = IMG_Load(rc->wolf[5]);
  rc->tex[5] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[5] = (int *)sdl->image->pixels;

  sdl->image = IMG_Load(rc->wolf[6]);
  sdl->texture = SDL_CreateTextureFromSurface(sdl->renderer, sdl->image);
  rc->tex[6] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[6] = (int *)sdl->image->pixels;

  sdl->image = IMG_Load(rc->wolf[7]);
  sdl->texture = SDL_CreateTextureFromSurface(sdl->renderer, sdl->image);
  rc->tex[7] = (int *)malloc(sizeof(int) * texHeight * texWidth );
  rc->tex[7] = (int *)sdl->image->pixels;

}

int textureSurface(t_sdl *sdl, t_raycaster *rc)
{
  rc->tex = (int **)malloc(sizeof(int *) * 8);
  tex2(rc, sdl);
  
  rc->buffer = (int *)malloc(sizeof(int) * (int)WIN_X * (int)WIN_Y);
  sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING, WIN_X, WIN_Y);
  return (0);
}

int init(t_sdl *sdl, t_raycaster *rc)
{
  sdl->window = NULL;
  sdl->renderer = NULL;
  sdl->texture = NULL;
  sdl->image = NULL;
  rc->player_pos_x = 22;
  rc->player_pos_y = 12;
  rc->player_dir_x = -1;
  rc->player_dir_y = 0;
  rc->player_plane_x = 0;
  rc->player_plane_y = 0.6;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    fprintf(stderr, "SDL initialization failed (%s)\n", SDL_GetError());
    return (-1);
  }
  if (SDL_CreateWindowAndRenderer(WIN_X, WIN_Y, 0, &sdl->window, &sdl->renderer) != 0)
  {
    fprintf(stderr, "Window creation failed (%s)\n", SDL_GetError());
    return (-1);
  }
  textureSurface(sdl, rc);
  return (0);
}



void initial_calc(t_raycaster *rc, int x)
{

  double camera_x;

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

void perform_dda(t_raycaster *rc)
{
  int hit;

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

void calc_wall_height(t_raycaster *rc )
{

  int line_height;

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
  //texturing calculations
   rc->texNum = rc->map[rc->map_x][rc->map_y] - 1; //1 subtracted from it so that texture 0 can be used!
  //calculate value of wallX
  if (rc->side == 0)
    rc->wallx = rc->player_pos_y + rc->perp_wall_dist * rc->ray_dir_y;
  else
    rc->wallx = rc->player_pos_x + rc->perp_wall_dist * rc->ray_dir_x;
  rc->wallx -= floor((rc->wallx));
  //x coordinate on the texture
  rc->texX = (int)(rc->wallx * (double)texWidth);
  if (rc->side == 0 && rc->ray_dir_x > 0)
    rc->texX = texWidth - rc->texX - 1;
  if (rc->side == 1 && rc->ray_dir_y < 0)
    rc->texX = texWidth - rc->texX - 1;
  // How much to increase the texture coordinate per screen pixel
  rc->step = 1.0 * texHeight / line_height;
  // Starting texture coordinate
  rc->texPos = (rc->draw_start - WIN_Y / 2 + line_height / 2) * rc->step;

}

void drawmap(t_raycaster *rc, t_sdl *sdl)
{
  int i;
  int j;
  SDL_Rect rect;
  rect.w = 6;
  rect.h = 6;
  i = 0;
  while (i < 24)
  {
    j = 0;
    while (j < 24)
    {
      rect.x = i * 6 ;
      rect.y = j * 6 ;
      if (rc->map[j][i] > 0)
      {
        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(sdl->renderer, &rect);
      }
      else
      {
        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(sdl->renderer, &rect);
      }
      j++;
    }
    i++;
  }
}


void drawplayer(t_raycaster *rc, t_sdl *sdl)
{

  SDL_Rect player;
  player.y = rc->player_pos_x * 6;
  player.x = rc->player_pos_y * 6;
  player.w = 6;
  player.h = 6;
 
  SDL_SetRenderDrawColor(sdl->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

  SDL_RenderFillRect(sdl->renderer, &player);
int x;
x = 0;
  while (x < WIN_X)
  {
    initial_calc(rc, x);
    perform_dda(rc);
    SDL_RenderDrawLine(sdl->renderer, player.x + 3, player.y + 3, rc->map_y * 6 , rc->map_x * 6 );
    x++;
  }
}

void clearcolorbuffer(int *buffer, int color)
{
  int i;
  int j;

  i = 0;
  while (i < WIN_X)
  {
    j = 0;
    while (j < WIN_Y)
    {
      buffer[(WIN_Y * j) + i] = color;
      j++;
    }
    i++;
  }
}

void rendercolorbuffer(t_raycaster *rc, t_sdl *sdl)
{
  SDL_UpdateTexture(sdl->texture, NULL, rc->buffer,(int)WIN_X * sizeof(int));
  SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
}

void draw_vert_line(t_raycaster *rc, int x)
{
  int j;
  j = -1;
  while (++j < rc->draw_start)
    rc->buffer[WIN_X * j + x] = 0xff87cefa;
  j = rc->draw_start - 1;
  while (++j < rc->draw_end)
  {
    // rc->buffer[WIN_X * j + x] = 0xfff;
    rc->texY = (int)rc->texPos & (texHeight - 1);
    rc->texPos += rc->step;
    int color = rc->tex[rc->texNum][texHeight * rc->texY + rc->texX];
    rc->buffer[WIN_X * j + x] = color;
  }
  j = rc->draw_end;
  while (++j < WIN_Y)
    rc->buffer[WIN_X * j + x] = 0x000;
}

int handle_events(t_raycaster *rc)
{
  SDL_Event event;
  double oldDirX;
  double oldPlaneX;

  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
      return (-1);
    if (event.type == SDL_KEYDOWN)
    {
      if (event.key.keysym.sym == SDLK_UP)
      {
        if (rc->map[(int)(rc->player_pos_x + rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0)
          rc->player_pos_x += rc->player_dir_x * MV_SPEED;
        if (rc->map[(int)(rc->player_pos_x)][(int)(rc->player_pos_y + rc->player_dir_y * MV_SPEED)] == 0)
          rc->player_pos_y += rc->player_dir_y * MV_SPEED;
      }
      if (event.key.keysym.sym == SDLK_DOWN)
      {
        if (rc->map[(int)(rc->player_pos_x - rc->player_dir_x * MV_SPEED)][(int)(rc->player_pos_y)] == 0)
          rc->player_pos_x -= rc->player_dir_x * MV_SPEED;
        if (rc->map[(int)(rc->player_pos_x)][(int)(rc->player_pos_y - rc->player_dir_y * MV_SPEED)] == 0)
          rc->player_pos_y -= rc->player_dir_y * MV_SPEED;
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

void raycaster(t_sdl *sdl, t_raycaster *rc)
{
  SDL_bool done;
  int x;

  done = SDL_FALSE;
  while (!done)
  {
    if (handle_events(rc) != 0)
      done = SDL_TRUE;
    SDL_RenderClear(sdl->renderer);
    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
    x = -1;
    while (++x < WIN_X)
    {
       initial_calc(rc, x);
       perform_dda(rc);
       calc_wall_height(rc);
       draw_vert_line(rc, x);
    }
    rendercolorbuffer(rc, sdl);
    clearcolorbuffer(rc->buffer, 0x000);
    drawmap(rc, sdl);
    drawplayer(rc, sdl);
    SDL_RenderPresent(sdl->renderer);
  }
}

int main(int ac, char **av)
{
  t_sdl sdl;
  t_raycaster rc;

  if (ac == 2)
  {
    if (!(rc.map = read_map(av[1])))
    {
      puts("Error, in map // -> map[24][24]!!!!!!");
      return (-1);
    }
    imgs(&rc);
    if (init(&sdl, &rc) != 0)
      return (-1);
    raycaster(&sdl, &rc);
      free(rc.buffer);
      free(rc.wolf);
      if (sdl.texture)
        SDL_DestroyTexture(sdl.texture);
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
