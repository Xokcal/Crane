//
// Created by 26432 on 2026/8/26.
//

#include <stdio.h>
#include <stdlib.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/log/log.h"
#include "../tool/map/hashmap_int.h"

void print_var(Hashmap *HASHMAP_VOID , VAR *var , char *var_name){
    int value;
    switch (var->class) {
        case INT:
            LOG_INFO("CLASS" , "CLASS");
            value = *(int*)var->value;
            printf("%d\n", value);
            return;
    }
}

int* str_convert_int(String *value_str){
    int v = 0;
    for (int i = 0; i < value_str->length; ++i) {
        switch (value_str->str[i]) {
            case '0':
                v += 0;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '1':
                v += 1;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '2':
                v += 2;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '3':
                v += 3;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '4':
                v += 4;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '5':
                v += 5;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '6':
                v += 6;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '7':
                v += 7;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '8':
                v += 8;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
            case '9':
                v += 9;
                if(i == value_str->length - 1) {
                    int *end_r = (int *)malloc(sizeof(int ));
                    end_r = &v;
                    return end_r;
                }
                v *= 10;
                break;
        }
    }
    int *end_r = (int *)malloc(sizeof(int ));
    return end_r;
}
