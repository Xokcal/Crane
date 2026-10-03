//
// Created by 26432 on 2026/5/28.
//

#ifndef XOKC_HASHMAP_H
#define XOKC_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../cranegram/types.h"

// = (int *) v;
#define MAP_GET(type, var)  ((type*)((var)->value))

typedef enum VAR_CLASS{
    INT,
    CHAR,
    STRING,
    LONG,
    FLOAT,
    DOUBLE
}VAR_CLASS;

typedef struct VAR{
    String *var_type;
    void* value;
}VAR;

typedef struct Node{
    char* K;
    VAR *V;
    struct Node* next;
}Node;

typedef struct Hashmap{
    Node** arr;
    int len;
    float factor;
}Hashmap;



Hashmap* create_hashmap();

void free_hashmap(Hashmap* hashmap);

int hashcode(char *K , Hashmap* map);

void map_put(Hashmap *map, char *K,String *type , void *v);

VAR *map_get(Hashmap *map, char *K);

#endif //XOKC_HASHMAP_H
