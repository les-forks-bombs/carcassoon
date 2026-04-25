#include <SDL3/SDL.h>
#include <sdl/map.h>

map_t *create_map(void) {
  map_t *map = SDL_calloc(1, sizeof(map_t));
  return map;
}

void render_map(map_t *map, SDL_Renderer *renderer, camera_t *cam) {
  for (int i = 0; i < MAP_TABLE_SIZE * MAP_TABLE_SIZE; i++) {
    if (map->tiles[i] == NULL) continue;

    float x_render    = (map->tiles[i]->world_x - cam->x) * cam->zoom;
    float y_render    = (map->tiles[i]->world_y - cam->y) * cam->zoom;
    float size_zoomed = MAP_TILE_SIZE * cam->zoom;

    if (x_render + size_zoomed > 0 && x_render < WINDOW_WIDTH &&
        y_render + size_zoomed > 0 && y_render < WINDOW_HEIGHT) {
      SDL_FRect dest = {x_render, y_render, size_zoomed, size_zoomed};
      SDL_RenderTexture(renderer, map->tiles[i]->texture, NULL, &dest);
    }
  }
}

void update_camera(camera_t *cam) {
  if (cam->x < 0) cam->x = 0;

  float max_x = (MAP_TABLE_SIZE * MAP_TILE_SIZE) - (WINDOW_WIDTH / cam->zoom);
  if (cam->x > max_x) cam->x = max_x;

  if (cam->y < 0) cam->y = 0;

  float max_y = (MAP_TABLE_SIZE * MAP_TILE_SIZE) - (WINDOW_HEIGHT / cam->zoom);
  if (cam->y > max_y) cam->y = max_y;
}
