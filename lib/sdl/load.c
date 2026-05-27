#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <dirent.h>
#include <sdl/appstate.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "libutils/hashmap.h"
#include "libutils/path.h"
#include "sdl/forward.h"

void load_texture(appstate_t *state, char *name, char *path) {
  SDL_Texture *texture = IMG_LoadTexture(state->renderer, path);

  hashmap_set(&state->textures, name, strlen(name) + 1, &texture,
              sizeof(SDL_Texture *));
}

void load_textures(appstate_t *appstate, char *directory, char *assets) {
  DIR *dossier = opendir(directory);
  if (dossier == NULL) {
    perror("Erreur lors de l'ouverture du dossier");
    return;
  }

  struct dirent *entree;
  char           chemin_complet[LIBUTILS_PATH_BUF];

  while ((entree = readdir(dossier)) != NULL) {
    if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0) {
      continue;
    }
    snprintf(chemin_complet, sizeof(chemin_complet), "%s/%s", directory,
             entree->d_name);

    struct stat info_chemin;
    if (stat(chemin_complet, &info_chemin) == 0) {
      if (S_ISDIR(info_chemin.st_mode)) {
        // printf("[DOSSIER] %s\n", chemin_complet);
        load_textures(appstate, chemin_complet, assets);
      } else if (S_ISREG(info_chemin.st_mode)) {
        // printf("[FICHIER] %s\n", chemin_complet);

        char *start = &chemin_complet[strlen(assets)];
        // printf("loading texture %s at %s\n", start, chemin_complet);

        load_texture(appstate, start, chemin_complet);
      }
    }
  }
  closedir(dossier);
}
