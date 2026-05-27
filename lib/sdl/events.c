#include <sdl/action.h>
#include <sdl/consts.h>
#include <sdl/events.h>

SDL_AppResult handle_mouse_event_(appstate_t *as, SDL_Event *event) {
  float mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  const SDL_FPoint mouse_pos = {mouseX, mouseY};

  switch (event->type) {
    case SDL_EVENT_MOUSE_WHEEL:
      if (SDL_PointInRectFloat(&mouse_pos, &as->map_viewport)) {
        float localX = mouseX - as->map_viewport.x;
        float localY = mouseY - as->map_viewport.y;

        float worldMouseX = (localX / as->camera->zoom) + as->camera->x;
        float worldMouseY = (localY / as->camera->zoom) + as->camera->y;

        if (event->wheel.y > 0) {
          if (as->camera->zoom < ZOOM_MAX) as->camera->zoom *= 1.1f;
        } else {
          if (as->camera->zoom > ZOOM_MIN) as->camera->zoom *= 0.9f;
        }

        as->camera->x = worldMouseX - (localX / as->camera->zoom);
        as->camera->y = worldMouseY - (localY / as->camera->zoom);
      }
      break;
    case SDL_EVENT_MOUSE_MOTION:
      if (event->motion.state & SDL_BUTTON_LMASK &&
          SDL_PointInRectFloat(&mouse_pos, &as->map_viewport)) {
        as->camera->x -= event->motion.xrel / as->camera->zoom;
        as->camera->y -= event->motion.yrel / as->camera->zoom;
      }
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult handle_key_event_(appstate_t *as, SDL_Keycode key_val) {
  switch (key_val) {
    case SDLK_ESCAPE:
      return SDL_APP_SUCCESS;
    case SDLK_LEFT:
      pass_to_action(as, -1);
      break;
    case SDLK_RIGHT:
      pass_to_action(as, 1);
      break;
    case SDLK_KP_PLUS:
      as->camera->zoom += 0.1f;
      break;
    case SDLK_KP_MINUS:
      as->camera->zoom -= 0.1f;
      break;
    case SDLK_Z:
      as->camera->y += 10.0f;
      break;
    case SDLK_Q:
      as->camera->x += 10.0f;
      break;
    case SDLK_S:
      as->camera->y -= 10.0f;
      break;
    case SDLK_D:
      as->camera->x -= 10.0f;
      break;
    case SDLK_RETURN:
      send_action_to_engine(as);
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}
