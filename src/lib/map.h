#pragma once

#include <stdlib.h>
#include <string.h>

#include "hash.h"

typedef struct map_node_t {
    unsigned long size;
    char **keys;
    void **values;
} map_node_t;

typedef struct map_t {
    map_node_t **table;
    unsigned short int table_size;
} map_t;

inline map_t *map_create(unsigned short int table_size) {
    map_t *map = (map_t *)malloc(sizeof(map_t));
    map->table_size = table_size;
    map->table = (map_node_t **)calloc(sizeof(map_node_t *), table_size);

    for (unsigned short int i = 0; i < table_size; i++) {
        map->table[i] = (map_node_t *)malloc(sizeof(map_node_t));
        map->table[i]->size = 0;
        map->table[i]->keys = (char **)malloc(0);
        map->table[i]->values = (void **)malloc(0);
    }

    return map;
}

inline void *map_get(map_t *map, char *key) {
    unsigned short int hash = hash_string(key) % map->table_size;

    unsigned short int i = 0;
    while (i < map->table[hash]->size) {
        if (strcmp(map->table[hash]->keys[i], key) == 0) {
            return map->table[hash]->values[i];
        }

        i += 1;
    }

    return 0;
}

inline void map_set(map_t *map, char *key, void *value) {
    unsigned short int hash = hash_string(key) % map->table_size;

    unsigned short int i = 0;
    while (i < map->table[hash]->size) {
        if (strcmp(map->table[hash]->keys[i], key) == 0) {
            map->table[hash]->values[i] = value;
            return;
        }

        i += 1;
    }

    unsigned long size = (map->table[hash]->size += 1);
    map->table[hash]->keys = (char **)realloc(map->table[hash]->keys, size * sizeof(char *));
    map->table[hash]->values = (void **)realloc(map->table[hash]->values, size * sizeof(void *));

    map->table[hash]->keys[size - 1] = key;
    map->table[hash]->values[size - 1] = value;
}
