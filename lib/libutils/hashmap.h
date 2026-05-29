#pragma once

#include <libutils/lc.h>
#include <libutils/vector.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define LIBUTILS_HASHMAP_SEED 67

/// @brief Représente un couple (clé, valeur)
typedef struct hashmap_node {
  /// @brief La clé
  void *key;
  /// @brief La valeur
  void *value;
  /// @brief La taille de la clé
  size_t key_size;
} hashmap_node_t;

typedef Vector(list_t) bucket_vector_t;

/// @brief Représente une hashmap
typedef struct hashmap {
  /// @brief Vecteur de bucket
  bucket_vector_t buckets;
  /// @brief Le nombre de buckets
  unsigned int size;
  /// @brief Le nombre d'éléments dans la hashmap
  unsigned int element_count;
} hashmap_t;

/// @brief Macro définissant une hashmap du type (clé, valeur) spécifié
#define HashMap(key_type, value_type) \
  union {                             \
    hashmap_t   meta;                 \
    key_type   *key_ghost;            \
    value_type *value_ghost;          \
  }  // NOLINT(bugprone-macro-parentheses)

/// @brief Hash une clé pour donner un index
/// @param key LA clé à hasher
/// @param key_size La taille de la clé
/// @return L'index de la clé
unsigned int hash(const void *key, size_t key_size);

void utils_hashmap_create(hashmap_t *map, size_t number_of_buckets);

void  utils_hashmap_set(hashmap_t *map, const void *key, size_t key_size,
                        const void *data, size_t data_size);
void *utils_hashmap_get(const hashmap_t *map, const void *key, size_t key_size);
void  utils_hashmap_free(hashmap_t *map);
void  utils_hashmap_remove(hashmap_t *map, const void *key, size_t key_size);

/// @brief Alloue la map passé avec le nombre de buckets spécifié
/// @param map La map à allouer
/// @param number_of_buckets Le nombre de buckets à allouer dans la map
#define hashmap_create(map, number_of_buckets) \
  utils_hashmap_create(&((map)->meta), number_of_buckets)

/// @brief Insère le couple (clé, valeur) dans la map
/// @param map La map dans laquelle ajouter l'élément
/// @param key La clé du couple (clé, valeur)
/// @param key_size la taille en octets de la clé
/// @param value La valeur du couple (clé, valeur)
/// @param value_size La taille en octets de la valeur
#define hashmap_set(map, key, key_size, value, value_size)                   \
  utils_hashmap_set(                                                         \
      &((map)->meta), (const void *)(1 ? (void *)(key) : *(map)->key_ghost), \
      (key_size), (const void *)(1 ? (void *)(value) : *(map)->value_ghost), \
      (value_size))

/// @brief Récupère une valeur selon une clé dans la map
/// @param map La map dans laquelle récupérer l'élément
/// @param key La clé du couple (clé, valeur)
/// @param key_size la taille en octets de la clé
/// @details Renvoie NULL si l'élément n'est pas trouvé
/// @return La valeur du couple (clé, valeur)
#define hashmap_get(map, key, key_size)                                      \
  (__typeof__(*(map)->value_ghost))utils_hashmap_get(                        \
      &((map)->meta), (const void *)(1 ? (void *)(key) : *(map)->key_ghost), \
      (key_size))

/// @brief Retire un couple (clé, valeur) de la map
/// @param map La map dans laquelle retirer le couple (clé, valeur)
/// @param key La clé du couple (clé, valeur)
/// @param key_size la taille en octets de la clé
#define hashmap_remove(map, key, key_size) \
  utils_hashmap_remove(&((map)->meta),     \
                       (1 ? (void *)(key) : *(map)->key_ghost), (key_size))

/// @brief Libère l'espace mémoire d'une instance de map
/// @param map La map à désallouer
#define hashmap_free(map) utils_hashmap_free(&((map)->meta))
