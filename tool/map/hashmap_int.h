//
// Created by 26432 on 2026/5/28.
//

#ifndef XOKC_HASHMAP_H
#define XOKC_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../cranegram/types.h"

typedef enum VAR_CLASS{
    INT,
    CHAR,
    STRING,
    FLOAT,
    DOUBLE
}VAR_CLASS;

typedef struct VAR{
    VAR_CLASS class;
    void* value;
}VAR;

typedef struct Node{
    char* K;
    TypeVar *V;
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

void map_put(Hashmap *map, char *K,String *type , String *var_name);

TypeVar *map_get(Hashmap *map, char *K);

#endif //XOKC_HASHMAP_H
