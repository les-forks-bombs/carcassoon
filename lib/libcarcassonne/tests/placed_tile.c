
#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>

/* Vérifie l'instanciation d'un deck */
void placed_tile_open_slots_works(void** state) {
  (void)state;

  placed_tile_t tile = {0};
  return_code_t code =
      placed_tile_create(&tile, &LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[12],
                         LIBCARCASSONNE_TILE_ORIENTATION_NORTH, 0, 1);

  assert_int_equal(code, SUCCESS);
  assert_int_equal(tile.groups[LIBCARCASSONNE_TILE_PART_A]->open_slots, 1);
  assert_int_equal(tile.groups[LIBCARCASSONNE_TILE_PART_B]->open_slots, 2);
  assert_int_equal(tile.groups[LIBCARCASSONNE_TILE_PART_C]->open_slots, 1);

  placed_tile_destroy(&tile);
}

/* Tests pour la propagation des groupes et leur complétude */

/// @brief Test qu'une route isolée (RRRR) a 1 open_slot par groupe et n'est pas
/// complète
void placed_tile_road_single_not_complete(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "RRRR", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  placed_tile_t** placed = game_tile_at(&game, 0, 0);
  assert_non_null(placed);
  assert_non_null(*placed);

  // RRRR: groupes A,B,C,D sont les routes sur les bords (positions 1,3,5,7)
  // Chaque a exactement 1 open_slot
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_A]->open_slots,
                   1);
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_B]->open_slots,
                   1);
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_C]->open_slots,
                   1);
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_D]->open_slots,
                   1);

  // Aucun groupe n'est complet
  assert_false(placed_tile_group_complete(
      &game, (*placed)->groups[LIBCARCASSONNE_TILE_PART_A]));
  assert_false(placed_tile_group_complete(
      &game, (*placed)->groups[LIBCARCASSONNE_TILE_PART_B]));

  destroy_game(&game);
}

/// @brief Test qu'une ville CCCC (avec blason) a 4 open_slots et n'est pas
/// complète
void placed_tile_town_cccc_not_complete(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // CCCC existe UNIQUEMENT avec blason=true
  const tile_t* tile = deck_find_tile(&game.deck, "CCCC", true);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  placed_tile_t** placed = game_tile_at(&game, 0, 0);
  assert_non_null(placed);
  assert_non_null(*placed);

  // CCCC: groupe A = ville sur les bords (positions 1,3,5,7) = 4 open_slots
  // groupe B = centre (position 4) = 0 open_slots
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_A]->open_slots,
                   4);
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_B]->open_slots,
                   0);

  // Le groupe A (ville) n'est PAS complet car il a 4 faces ouvertes
  assert_false(placed_tile_group_complete(
      &game, (*placed)->groups[LIBCARCASSONNE_TILE_PART_A]));
  // Le groupe B (centre) est complet car il n'a pas de faces
  assert_true(placed_tile_group_complete(
      &game, (*placed)->groups[LIBCARCASSONNE_TILE_PART_B]));

  destroy_game(&game);
}

/// @brief Test qu'un champ FCFC a 1 open_slot et n'est jamais complet
void placed_tile_field_fcfc_not_complete(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // FCFC sans blason
  const tile_t* tile = deck_find_tile(&game.deck, "FCFC", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  placed_tile_t** placed = game_tile_at(&game, 0, 0);
  assert_non_null(placed);
  assert_non_null(*placed);

  // FCFC sans blason: parts_groups = {A,A,A,B,D,B,C,C,C}
  // Groupe A (champ): position 1 (bord) = 1 open_slot
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_A]->open_slots,
                   1);
  assert_false(placed_tile_group_complete(
      &game, (*placed)->groups[LIBCARCASSONNE_TILE_PART_A]));

  // Groupe B (ville): positions 3,5 (bords) = 2 open_slots
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_B]->open_slots,
                   2);
  assert_false(placed_tile_group_complete(
      &game, (*placed)->groups[LIBCARCASSONNE_TILE_PART_B]));

  // Groupe C (champ): position 7 (bord) = 1 open_slot
  assert_int_equal((*placed)->groups[LIBCARCASSONNE_TILE_PART_C]->open_slots,
                   1);
  assert_false(placed_tile_group_complete(
      &game, (*placed)->groups[LIBCARCASSONNE_TILE_PART_C]));

  destroy_game(&game);
}

/// @brief Test propagation: lier deux routes RRRR pour former un groupe complet
void placed_tile_road_link_propagates_completeness(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile1 = deck_find_tile(&game.deck, "RRRR", false);
  const tile_t* tile2 = deck_find_tile(&game.deck, "CRRR", false);
  assert_ptr_not_equal(tile1, NULL);
  assert_ptr_not_equal(tile2, NULL);

  assert_int_equal(game_place_tile(&game, tile1, 0, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);
  assert_int_equal(game_place_tile(&game, tile2, 0, 1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);

  placed_tile_t** p1 = game_tile_at(&game, 0, 0);
  placed_tile_t** p2 = game_tile_at(&game, 0, 1);
  assert_non_null(p1);
  assert_non_null(*p1);
  assert_non_null(p2);
  assert_non_null(*p2);

  // Lier les groupes de route EST (B) de p1 et OUEST (D) de p2
  // RRRR: NORD=A, EST=B, SUD=C, OUEST=D
  placed_tile_group_t* road1 =
      (*p1)->groups[LIBCARCASSONNE_TILE_PART_C];  // route EST de la première
                                                  // tuile
  placed_tile_group_t* road2 =
      (*p2)->groups[LIBCARCASSONNE_TILE_PART_B];  // route OUEST de la deuxième
                                                  // tuile

  // Avant lien: chaque a 1 open_slot
  assert_int_equal(road1->open_slots, 0);
  assert_int_equal(road2->open_slots, 0);

  // Les open_slots sont mis à jour: 1+1-2 = 0
  // Le groupe combiné a 0 open_slots, donc il EST complet
  assert_true(placed_tile_group_complete(&game, road1));
  assert_true(placed_tile_group_complete(&game, road2));

  destroy_game(&game);
}

/// @brief Test propagation: lier deux routes pour former un groupe non complet
/// (L)
void placed_tile_road_l_shape_not_complete(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // Utiliser CRFR qui a une route en forme de L
  // CRFR: parts_groups = {A,D,A,B,D,B,C,C,C} (sans blason)
  // ROAD: position 3 (EST), 4 (centre), 5 (SUD)
  // Position 3 (EST, bord) -> groupe B -> 1 open_slot
  // Position 5 (SUD, bord) -> groupe B -> 1 open_slot
  // Position 4 (centre) -> groupe D -> 0 open_slots
  // Total pour groupe B: 2 open_slots

  const tile_t* tile1 = deck_find_tile(&game.deck, "CRFR", false);
  const tile_t* tile2 = deck_find_tile(&game.deck, "CRFR", false);
  assert_ptr_not_equal(tile1, NULL);
  assert_ptr_not_equal(tile2, NULL);

  // Placer la première tuile
  assert_int_equal(game_place_tile(&game, tile1, 0, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);
  // Placer la deuxième tuile à l'EST
  assert_int_equal(game_place_tile(&game, tile2, 0, 1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);

  placed_tile_t** p1 = game_tile_at(&game, 0, 0);
  placed_tile_t** p2 = game_tile_at(&game, 0, 1);
  assert_non_null(p1);
  assert_non_null(*p1);
  assert_non_null(p2);
  assert_non_null(*p2);

  // Groupe B de p1 (routes EST et SUD)
  placed_tile_group_t* road1 = (*p1)->groups[LIBCARCASSONNE_TILE_PART_B];
  // Groupe B de p2 (routes... dépend de l'orientation)
  placed_tile_group_t* road2 = (*p2)->groups[LIBCARCASSONNE_TILE_PART_B];

  // Avant lien: p1->B a 2 open_slots, p2->B a 2 open_slots
  assert_int_equal(road1->open_slots, 1);
  assert_int_equal(road2->open_slots, 1);

  destroy_game(&game);
}
