#pragma once

#include <libcarcassonne/forward.h>
#include <libutils/vector.h>

/// @brief Représente un événement de jeu
struct extension_process_hook {
  /// @brief Valeur de priorité de l'événement
  /// @details L'ensemble des hooks du jeu sont triés selon leur priorité.
  /// L'exécution se fait par ordre croissant de priorité
  const unsigned int priority;
  /// @brief Nom de l'événement
  const char label[50];
  /// @brief Fonction "forward" qui effectue l'événement de jeu
  const extension_forward_t fw;
  /// @brief Fonction "backward" qui annule les modifications du "forward"
  const extension_backward_t bw;
  /// @brief Fonction qui libère la mémoire utilisé par "forward" et "backward"
  const extension_free_t free;
  /// @brief Fonctin permettant de lister les actions possibles pour le joueur
  /// courant
  const extension_list_actions_t list_actions;
  /// @brief Type de l'action voulu par la fonction "forward"
  action_type_t needed_action;
};

/// @brief Représente une extension de jeu
struct extension {
  /// @brief Nom de l'extension de jeu
  const char *name;
  /// @brief Vecteur des prototypes de tuile présentes dans l'extension
  const tile_vector_t *tiles;
  /// @brief Vecteur des prototypes de tuile de départ présentes dans
  /// l'extension
  const tile_vector_t *start_tiles;
  /// @brief Valeur de priorité des tuiles de départ de cette extension
  /// @details Les tuiles de départ sont ordonnés dans l'ordre décroissant
  const unsigned int start_tiles_priority;
  /// @brief Vecteur des événements de jeu de l'extension
  const extension_process_hooks_vector_t *hooks;
  /// @brief Vecteur des dépendances de l'extension
  const extension_vector_t *required;
  /// @brief Vecteur du nombre de meeple par joueur par type de meeple de
  /// l'extension
  /// @details Les comptes de meeples de toutes les extensions sont additionnés
  const meeple_count_vector_t meeples_count;
};

return_code_t create_extension_list(extension_vector_t *);
void          destroy_extension_list(extension_vector_t *);
