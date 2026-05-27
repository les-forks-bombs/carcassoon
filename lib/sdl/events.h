#ifndef EVENTS_H
#define EVENTS_H

#include <SDL3/SDL.h>
#include <sdl/appstate.h>
#include <sdl/camera.h>

SDL_AppResult handle_mouse_event_(appstate_t *as, SDL_Event *event);
SDL_AppResult handle_key_event_(appstate_t *as, SDL_Keycode key_val);

#endif
