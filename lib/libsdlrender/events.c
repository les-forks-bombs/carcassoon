#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <libsdlrender/action.h>
#include <libsdlrender/consts.h>
#include <libsdlrender/events.h>
#include <libsdlrender/forward.h>
#include <stdbool.h>

static SDL_AppResult handle_mouse_event(appstate_t *as, SDL_Event *event) {
  float mouseX;
  float mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  const SDL_FPoint mouse_pos = {mouseX, mouseY};

  switch (event->type) {
    case SDL_EVENT_MOUSE_WHEEL:
      if (SDL_PointInRectFloat(&mouse_pos, &as->map_viewport)) {
        float localX = mouseX - as->map_viewport.x;
        float localY = mouseY - as->map_viewport.y;

        float worldMouseX = (localX / as->camera.zoom) + as->camera.x;
        float worldMouseY = (localY / as->camera.zoom) + as->camera.y;

        if (event->wheel.y > 0) {
          if (as->camera.zoom < ZOOM_MAX) {
            as->camera.zoom *= 1.1F;
          }
        } else {
          if (as->camera.zoom > ZOOM_MIN) {
            as->camera.zoom *= 0.9F;
          }
        }

        as->camera.x = worldMouseX - (localX / as->camera.zoom);
        as->camera.y = worldMouseY - (localY / as->camera.zoom);
      }
      break;
    case SDL_EVENT_MOUSE_MOTION:
      if (event->motion.state & SDL_BUTTON_LMASK &&
          SDL_PointInRectFloat(&mouse_pos, &as->map_viewport)) {
        as->camera.x -= event->motion.xrel / as->camera.zoom;
        as->camera.y -= event->motion.yrel / as->camera.zoom;
      }
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

static SDL_AppResult handle_key_event(appstate_t *as, SDL_Keycode key_val) {
  switch (key_val) {
    case SDLK_G:
      as->display_grid = ((!as->display_grid) != 0);
      break;
    case SDLK_ESCAPE:
      as->playing = false;
      break;
    case SDLK_LEFT:
      pass_to_action(as, -1);
      break;
    case SDLK_RIGHT:
      pass_to_action(as, 1);
      break;
    case SDLK_KP_PLUS:
      as->camera.zoom += 0.1F;
      break;
    case SDLK_KP_MINUS:
      as->camera.zoom -= 0.1F;
      break;
    case SDLK_Z:
      as->camera.y += 10.0F;
      break;
    case SDLK_Q:
      as->camera.x += 10.0F;
      break;
    case SDLK_S:
      as->camera.y -= 10.0F;
      break;
    case SDLK_D:
      as->camera.x -= 10.0F;
      break;
    case SDLK_RETURN:
      send_action_to_engine(as);
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult handle_app_event(appstate_t *as, SDL_Event *event) {
  switch (event->type) {
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      as->window_width   = (float)event->window.data1;
      as->window_height  = (float)event->window.data2;
      as->map_viewport.w = as->window_width;
      as->map_viewport.h = as->window_height;
      center_camera_on_start(as);
      break;
    case SDL_EVENT_QUIT:
      as->playing = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      return handle_key_event(as, event->key.key);
    case SDL_EVENT_MOUSE_WHEEL:
    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      return handle_mouse_event(as, event);
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}
