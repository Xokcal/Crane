#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hashmap_int.h"
#include "../../cranegram/types.h"


Hashmap *create_hashmap() {
    Hashmap *map = (Hashmap *) malloc(sizeof(Hashmap));
    map->len = 10;
    map->factor = 0.8;
    map->arr = (Node **) malloc(map->len * sizeof(Node *));
    for (int i = 0; i < map->len; ++i)
        map->arr[i] = NULL;
    return map;
}

void free_hashmap(Hashmap *hashmap) {
    for (int i = 0; i < hashmap->len; ++i) {
        //...
    }
    free(hashmap->arr);
    free(hashmap);
}

Node *create_node(char *K, String *type , String *var_name) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->K = K;
    node->V = create_TypeVar();
    copy_string(var_name , node->V->var_name);
    copy_string(type , node->V->type);
    node->next = NULL;
    return node;
}

int hashcode(char *K, Hashmap *map) {
    int hashcode = 0;
    for (int i = 0; i < strlen(K); ++i) {
        hashcode += K[i];
    }
    int r = hashcode % map->len;
    return r;
}

void map_put(Hashmap *map, char *K,String *type , String *var_name) {
    Node *new_node = create_node(K, type , var_name);
    int hash_index = hashcode(K, map);
    Node *h = map->arr[hash_index];
    if (h == NULL) {
        map->arr[hash_index] = new_node;
        return;
    }
    while (h->next != NULL) {
        if (strcmp(h->K, new_node->K) == 0) {
            h->V = new_node->V;
            return;
        }
        h = h->next;
    }
    if (strcmp(h->K , K) == 0){
        h->V = new_node->V;
        return;
    }
    h->next = new_node;
}

TypeVar *map_get(Hashmap *map, char *K) {
    int hash_index = hashcode(K, map);
    Node *h = map->arr[hash_index];
    if (h == NULL){
        TypeVar *null = (TypeVar*)malloc(sizeof(TypeVar));
        null->type = NULL;
        null->var_name = NULL;
        return null;
    }
    while (h != NULL) {
        if (strcmp(h->K, K) == 0) {
            return h->V;
        }
        h = h->next;
    }
    TypeVar *null = (TypeVar*)malloc(sizeof(TypeVar));
    null->type = NULL;
    null->var_name = NULL;
    return null;
}














