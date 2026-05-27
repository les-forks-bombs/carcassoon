#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/// @brief Représente un vecteur
typedef struct {
  /// @brief La taille du vecteur (nombre d'élements)
  unsigned int size;
  /// @brief La capacité totale du vecteur
  unsigned int caps;
  /// @brief Pointeur vers les données du vecteur
  void        *data;
} vector_t;

/// @brief Macro définissant un vecteur du type spécifié
#define Vector(type)     \
  union {                \
    vector_t meta;       \
    type    *type_ghost; \
  }  // NOLINT(bugprone-macro-parentheses)
  
unsigned int utils_vector_alloc(vector_t *vector, size_t capacity,
                                size_t element);
unsigned int utils_vector_grow(vector_t *vector, size_t size, size_t element);

unsigned int utils_vector_append(vector_t *vector, const void *data,
                                 size_t size);
void *utils_vector_nth(const vector_t *vector, unsigned int index, size_t size);
void  utils_vector_free(vector_t *vector);
void  utils_vector_remove(vector_t *vector, unsigned int index, size_t size);
void utils_vector_remove_value(vector_t *vector, const void *data, size_t size);

bool utils_vector_contains(vector_t *vector, const void *data, size_t size);

/// @brief Ajoute un élément dans un vecteur
/// @param vector Le vecteur dans lequel ajouter l'élément
/// @param item L'élément à ajouter
#define vector_append(vector, item)                                      \
  utils_vector_append(&((vector)->meta),                                 \
                      (const void *)(1 ? (item) : (vector)->type_ghost), \
                      sizeof(*(vector)->type_ghost))

/// @brief Récupère un élément du vecteur à l'index spécifié
/// @param vector Le vecteur dans lequel récupérer un élément
/// @param index L'index auquel récupérer l'élément
#define vector_nth(vector, index)                     \
  (__typeof__((vector)->type_ghost))utils_vector_nth( \
      &((vector)->meta), index, sizeof(*(vector)->type_ghost))

/// @brief Retirer l'élément à l'index spécifié dans un vecteur
/// @param vector Le vecteur dans lequel ajouter l'élément
/// @param index L'index de l'élément à retirer
#define vector_remove(vector, index) \
  utils_vector_remove(&((vector)->meta), index, sizeof(*(vector)->type_ghost))

/// @brief Libère la mémoire d'une instance de vecteur
/// @param vector Le vecteur à désallouer
#define vector_free(vector) utils_vector_free(&((vector)->meta))

/// @brief Alloue la mémoire d'une instance de vecteur
/// @param vector Le vecteur à allouer
#define vector_alloc(vector, size) \
  utils_vector_alloc(&((vector)->meta), size, sizeof(*(vector)->type_ghost))

/// @brief Augmente la taille d'une instance de vecteur
/// @param vector Le vecteur à faire grandir
#define vector_grow(vector, size) \
  utils_vector_grow(&((vector)->meta), size, sizeof(*(vector)->type_ghost))

/// @brief Permet de récupérer la taille d'un vecteur
/// @param vector Le vecteur
#define vector_size(vector) (*(const unsigned int *)&((vector)->meta.size))

/// @brief Permet de récupérer la capcité d'un vecteur
/// @param vector Le vecteur
#define vector_caps(vector) (*(const unsigned int *)&((vector)->meta.caps))

/// @brief Retirer l'élément spécifié dans un vecteur
/// @param vector Le vecteur dans lequel retirer l'élément
/// @param item L'élément à retirer
#define vector_remove_value(vector, item)                                      \
  utils_vector_remove_value(&((vector)->meta),                                 \
                            (const void *)(1 ? (item) : (vector)->type_ghost), \
                            sizeof(*(vector)->type_ghost))

/// @brief Recherche l'élément spécifié dans un vecteur
/// @param vector Le vecteur dans lequel chercher l'élément
/// @param item L'élément à rechercher
#define vector_contains(vector, item)                                      \
  utils_vector_contains(&((vector)->meta),                                 \
                        (const void *)(1 ? (item) : (vector)->type_ghost), \
                        sizeof(*(vector)->type_ghost))
