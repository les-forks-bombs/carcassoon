#pragma once

#include <stdlib.h>
#include <string.h>

/// @brief Réprésente un noeud de la liste chaînée
typedef struct list_node {
  /// @brief Noeud suivant
  struct list_node *next;
  /// @brief Noeud précédent
  struct list_node *prev;
  /// @brief Données stockés
  char value[];
} list_node_t;

/// @brief Représente une liste chaînée
typedef struct {
  /// @brief La tête de la liste
  list_node_t *head;
  /// @brief La queue de la liste
  list_node_t *tail;
  /// @brief La taille de la liste (nombre d'élements)
  unsigned int size;
} list_t;

/// @brief Macro définissant une liste chaînée du type spécifié
#define List(type)     \
  union {              \
    list_t meta;       \
    type  *type_ghost; \
  }  // NOLINT(bugprone-macro-parentheses)

list_node_t *utils_list_append(list_t *list, const void *data, size_t size);
list_node_t *utils_list_prepend(list_t *list, const void *data, size_t size);
list_node_t *utils_list_insert(list_t *list, const void *data, size_t size,
                               unsigned int index);
list_node_t *utils_list_nth(list_t *list, unsigned int index);
void         utils_list_free(list_t *list);
void         utils_list_remove(list_t *list, list_node_t *el);
void utils_list_remove_value(list_t *list, const void *data, size_t size);

/// @brief Ajoute un noeud à la fin de la liste
/// @param list La liste dans laquelle ajouter le noeud
/// @param item Le noeud à ajouter
#define list_append(list, item)                                      \
  utils_list_append(&((list)->meta),                                 \
                    (const void *)(1 ? (item) : (list)->type_ghost), \
                    sizeof(*(list)->type_ghost))

/// @brief Ajoute un noeud au début de la liste
/// @param list La liste dans laquelle ajouter le noeud
/// @param item Le noeud à ajouter
#define list_prepend(list, item)                                      \
  utils_list_prepend(&((list)->meta),                                 \
                     (const void *)(1 ? (item) : (list)->type_ghost), \
                     sizeof(*(list)->type_ghost))

/// @brief Ajoute un noeud dans la liste à l'index spécifié
/// @param list La liste dans laquelle ajouter le noeud
/// @param item Le noeud à ajouter
/// @param index L'index auquel ajouter le noeud
#define list_insert(list, item, index)                               \
  utils_list_insert(&((list)->meta),                                 \
                    (const void *)(1 ? (item) : (list)->type_ghost), \
                    sizeof(*(list)->type_ghost), index)

/// @brief Récupère un noeud de liste dans la liste à l'index spécifié
/// @param list La liste dans laquelle récupérer le noeud
/// @param index L'index auquel récupérer le noeud
/// @return Le noeud de liste à l'index spécifié
#define list_nth(list, index)  utils_list_nth(&((list)->meta), index)

/// @brief Récupère la valeur d'un noeud de liste dans la liste à l'index spécifié
/// @param list La liste dans laquelle récupérer le noeud
/// @param index L'index auquel récupérer le noeud
/// @return La valeur du noeud à l'index spécifié
#define list_value(list, node) ((__typeof__((list)->type_ghost))((node)->value))

/// @brief Retire un noeud de la liste
/// @param list La liste dans laquelle retirer le noeud
/// @param item Le noeud à retirer de la liste
#define list_remove(list, item) utils_list_remove(&((list)->meta), item)

/// @brief Permet de récupérer la tête de la liste
/// @param list La liste dans laquelle récupérer la tête
/// @return Le noeud à la tête de la liste
#define list_head(list) ((list)->meta.head)

/// @brief Permet de récupérer la queue de la liste
/// @param list La liste dans laquelle récupérer la queue
/// @return Le noeud à la queue de la liste
#define list_tail(list) ((list)->meta.tail)

/// @brief Libère la mémoire d'une instance de liste
/// @param list La liste à désallouer
#define list_free(list) utils_list_free(&((list)->meta))

/// @brief Permet de récupérer la taille d'une liste
/// @param list La liste dans laquelle récupérer la taille
/// @return La taille (nombre d'éléments) de la liste
#define list_size(list) (*(const unsigned int *)&((list)->meta.size))

/// @brief Retire un noeud de la liste par valeur
/// @param list La liste dans laquelle retirer le noeud
/// @param item La valeur du noeud à retirer de la liste
#define list_remove_value(list, item)                                      \
  utils_list_remove_value(&((list)->meta),                                 \
                          (const void *)(1 ? (item) : (list)->type_ghost), \
                          sizeof(*(list)->type_ghost))
