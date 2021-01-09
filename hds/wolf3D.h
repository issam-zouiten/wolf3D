#ifndef RAYCASTER_H_
#define RAYCASTER_H_

// System includes

#include "../SDL2/SDL.h"
#include <stdio.h>
#include <math.h>

// Initial values
#define WIN_X           1280
#define WIN_Y           720
#define MV_SPEED        0.2
#define ROT_SPEED       0.174533

// Colors
#define WHITE           0xFFFFFF
#define RED             0xFF0000
#define GREEN           0x00FF00
#define BLUE            0x0000FF
#define BLACK           0x000000

// Structures
typedef struct      s_sdl
{
  SDL_Window      *window;
  SDL_Renderer    *renderer;
} t_sdl;

typedef struct      s_raycaster
{
  double          player_pos_x;
  double          player_pos_y;
  double          player_dir_x;
  double          player_dir_y;
  double          player_plane_x;
  double          player_plane_y;
  double          ray_dir_x;
  double          ray_dir_y;
  int             map_x;
  int             map_y;
  double          side_dist_x;
  double          side_dist_y;
  double          delta_dist_x;
  double          delta_dist_y;
  int             step_x;
  int             step_y;
  int             side;
  int             draw_start;
  int             draw_end;
  double          perp_wall_dist;
  int             **map;
} t_raycaster;

// function
SDL_Color convert_color(int hexa_value);
SDL_Color     select_wall_color(int map_x, int map_y, t_raycaster *rc);
SDL_Color apply_night_effect(SDL_Color color, double distance);
int		**read_map(char *path);
#endif