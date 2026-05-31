#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <libsdlrender/forward.h>

/// @brief Représente un objet texte pour l'affichage
struct text_object {
  /// @brief La texture associée à l'objet
  SDL_Texture *texture;
  /// @brief La taille de la police
  float font_size;
  /// @brief La couleur du texte
  SDL_Color color;
  /// @brief Le texte affiché
  char *content;
  /// @brief La position du texte
  float w, h;
};

/// @brief Permet de créer une instance d'objet texte
void text_object_create(appstate_t *appstate, text_object_t *text,
                        const char *content, SDL_Color color, float font_size);
/// @brief Permet de mettre à jour l'affichage d'un objet texte
void text_object_update(appstate_t *appstate, text_object_t *to);
/// @brief Permet de désallouer un objet texte
void text_object_free(text_object_t *text);
