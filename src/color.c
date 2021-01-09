#include "wolf3D.h"

SDL_Color     convert_color(int hexa_value)
{
  SDL_Color   color;

  color.r = ((hexa_value >> 16) & 0xFF00FF);
  color.g = ((hexa_value >> 8) & 0xFF00FF);
  color.b = ((hexa_value) & 0xFF00FF);
  return (color); 
}

SDL_Color     select_wall_color(int map_x, int map_y, t_raycaster *rc)
{
  SDL_Color   color;
  
  if (rc->map[map_x][map_y] == 1)
    color = convert_color(RED);
  else if (rc->map[map_x][map_y] == 2)
    color = convert_color(GREEN);
  else if (rc->map[map_x][map_y] == 3)
    color = convert_color(BLUE);
  else if (rc->map[map_x][map_y] == 4)
    color = convert_color(WHITE);
  else
    color = convert_color(BLACK);
  return (color);
}
