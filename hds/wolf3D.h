#ifndef RAYCASTER_H_
#define RAYCASTER_H_

// System includes


#include "../SDL/SDL2.framework/Headers/SDL.h"
#include "../SDL/SDL2_image.framework/Headers/SDL_image.h"
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

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

#define texWidth 64
#define texHeight 64

// Structures
typedef struct      s_sdl
{
  SDL_Window      *window;
  SDL_Renderer    *renderer;
  SDL_Texture     *texture;
  SDL_Surface     *image;
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
  double          wallx;
  int             texX;
  int             texY;
  int             *buffer;
  int             **tex;
  double          texPos;
  double          step;
  int             texNum;
  char            **wolf;
  } t_raycaster;

// function
int		**read_map(char *path);
char *ft_strdup(const char *s1);

#endif