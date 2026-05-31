#ifndef TILE_H
#define TILE_H

#include <libsdlrender/appstate.h>

/// @brief Met la première action de la liste dans l'appstate en tant qu'action courante
/// @param as l'appstate du rendu
void put_first_action_in_appstate(appstate_t *as);

/// @brief Récupère la liste des prochaines actions fournies par engine
/// @param as l'appstate du rendu
void get_current_actions(appstate_t *as);

/// @brief Change l'action courante 
/// @param as l'appstate du rendu
/// @param increment le sens du changement
/// @details -1 pour passer à l'action précédente, 1 pour passer à la suivante
void pass_to_action(appstate_t *as, unsigned int increment);

/// @brief Dispatch l'action au moteur et met à jour l'état de jeu
/// @param as l'appstate du rendu
void send_action_to_engine(appstate_t *as);

#endif
