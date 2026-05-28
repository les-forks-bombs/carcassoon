#ifndef EVENTS_H
#define EVENTS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <libsdlrender/appstate.h>
#include <libsdlrender/camera.h>

SDL_AppResult handle_app_event(appstate_t *as, SDL_Event *event);

#endif
