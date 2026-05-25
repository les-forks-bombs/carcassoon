#ifndef EVENTS_H
#define EVENTS_H

#include <SDL3/SDL.h>
#include <sdl/appstate.h>

SDL_AppResult handle_mouse_event_(AppState *as, SDL_Event *event);
SDL_AppResult handle_key_event_(AppState *as, SDL_Keycode key_val);

#endif
