#ifndef LOAD_H
#define LOAD_H

#include <libsdlrender/appstate.h>

/// @brief Charge toutes les textures d'un dossier dans le dictionnaire de l'affichage de la partie
/// @param appstate L'affichage de la partie
/// @param directory Le dossier qui contient les assets
void load_textures(appstate_t *appstate, char *directory, char *assets);

#endif
