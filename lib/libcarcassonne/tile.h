#pragma once

#include <libutils/vector.h>
#include <stdbool.h>

/// @brief Représente un emplacement pour meeple sur une tuile
struct tile_slot {
  double            x, y;
  tile_part_group_t group;
  // mask
};

/// @brief Une tile est un élément de jeu de carcassonne
struct tile {
  /// @brief Famille de la tile
  char family[5];
  /// @brief Parties de la tile
  tile_part_type_t parts[9];
  /// @brief Spécifie si la tile est un blason
  bool blason;
  /// @brief Spécifie le nombre de tiles présentes
  unsigned int amount;
  /// @brief Définition des groupes a l'intérieur d'une tile
  tile_part_group_t parts_groups[9];
  /// @brief nom de l'image correspondante
  char* texture;
  /// @brief Emplacements possibles pour les meeples
  tile_slot_t slots[9];
  /// @brief Nombre effectif de slots
  unsigned int nb_slots;
};

/// @brief Récupère une famille de la tuile en fonction de l'orientation et de
/// la face de connexion
/// @param tile Le modèle de tuile à utiliser
/// @param orientation L'orientation de la tuile
/// @param connexion_face La face de connexion
/// @return La famille de la face de connexion
/// @related tile_t
tile_part_type_t tile_get_family_face(const tile_t*      tile,
                                      tile_orientation_t orientation,
                                      tile_orientation_t connexion_face);

/// @brief Retourne l'inverse d'une orientation
/// Ex: NORTH <=> SOUTH, EAST <=> WEST
/// @param orientation L'orientation à inverser
/// @return L'orientation inversée
/// @related tile_t
tile_orientation_t tile_orientation_invert(tile_orientation_t orientation);
