#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <dirent.h>
#include <libsdlrender/appstate.h>
#include <libsdlrender/forward.h>
#include <libutils/hashmap.h>
#include <libutils/path.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static void load_texture(appstate_t *state, char *name, char *path) {
  SDL_Texture *texture = IMG_LoadTexture(state->renderer, path);
  if (texture == NULL) {
    printf("LOAD: Couldn't load the texture at %s\n", path);
  }
  printf("Loaded texture %s\n", name);

  hashmap_set(&state->textures, name, strlen(name) + 1, &texture,
              sizeof(SDL_Texture *));
}

void load_textures(appstate_t *appstate, char *directory, char *prefix) {
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
        load_textures(appstate, chemin_complet, prefix);
      } else if (S_ISREG(info_chemin.st_mode)) {
        char *start = &chemin_complet[strlen(prefix)];

        load_texture(appstate, start, chemin_complet);
      }
    }
  }
  closedir(dossier);
}
