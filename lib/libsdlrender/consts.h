#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <libcarcassonne/libcarcassonne.h>

/// @brief La taille d'une tuile en px
#define MAP_TILE_SIZE  64
#define MAP_TABLE_SIZE ((LIBCARCASSONNE_TILES_COUNT * 2) - 1)
/// @brief La taille de la fenêtre en largeur au lancement en px
#define DEFAULT_WINDOW_WIDTH 1000
/// @brief La taille de la fenêtre en hauteur au lancement en px
#define DEFAULT_WINDOW_HEIGHT 700
/// @brief Le paramètre pour redimmensionner la fenêtre
/// @details 1 pour avoir une fenêtre redimmensionnable, 0 sinon
#define WINDOW_RESIZABLE 1
/// @brief Le niveau de zoom maximum
#define ZOOM_MAX 5
/// @brief Le niveau de zoom minimum
#define ZOOM_MIN 0.5
/// @brief Le nombre d'images par seconde
#define STEP_RATE_IN_MILLISECONDS 16
/// @brief L'affichage de la grille au démarrage
#define DISPLAY_GRID_BY_DEFAULT 0

/// @brief Les couleurs par défaut des joueurs
static SDL_Color players_colors[] = {{255, 0, 255, 255},
                                     {0, 200, 255, 255},
                                     {255, 100, 0, 255},
                                     {0, 0, 0, 255},
                                     {150, 0, 255, 255}};

extern SDL_Texture* DEFAULT_TEXTURE;
