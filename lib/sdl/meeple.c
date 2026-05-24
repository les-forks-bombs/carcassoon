#include <sdl/meeple.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tile.h>
#include <libcarcassonne/game.h>

void render_placed_meeple(placed_tile_t *tile, SDL_Renderer *renderer, float x_render, float y_render, float size_zoomed, double angle, SDL_Texture *meeple_tex){
  const tile_t *real_tile = tile->parent;
  if (real_tile == NULL) return;

  for (unsigned int s = 0; s < real_tile->nb_slots; s++) {
    tile_slot_t slot = real_tile->slots[s];
    tile_part_group_t slot_group = slot.group;
    
    placed_tile_group_t *ptg = tile->groups[slot_group];
    if (ptg != NULL && ptg->meeple != NULL) {
      float cx = (float)slot.x - 0.5f;
      float cy = (float)slot.y - 0.5f;

      double rad = angle * (SDL_PI_D / 180.0);
      float cos_a = (float)SDL_cos(rad);
      float sin_a = (float)SDL_sin(rad);

      float rot_x = cx * cos_a - cy * sin_a;
      float rot_y = cx * sin_a + cy * cos_a;

      float final_local_x = rot_x + 0.5f;
      float final_local_y = rot_y + 0.5f;

      float meeple_size = size_zoomed * 0.15f;

      float meeple_x = x_render + (final_local_x * size_zoomed) - (meeple_size / 2.0f);
      float meeple_y = y_render + (final_local_y * size_zoomed) - (meeple_size / 2.0f);

      SDL_FRect meeple_dest = {
        meeple_x, 
        meeple_y, 
        meeple_size, 
        meeple_size
      };

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
      SDL_RenderFillRect(renderer, &meeple_dest);
    }
  }
}