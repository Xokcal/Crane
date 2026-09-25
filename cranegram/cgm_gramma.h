//
// Created by 26432 on 2026/8/26.
//

#ifndef CRANEC_CGM_GRAMMA_H
#define CRANEC_CGM_GRAMMA_H

#include <stdio.h>
#include <stdlib.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/log/log.h"
#include "../tool/map/hashmap_int.h"

int print_k_exe(String **tokens , int len , int curr_i , Hashmap *HASHMAP_VOID);
int int_k_exe(String **tokens , int len , int curr_i , Hashmap *HASHMAP_VOID);
int str_convert_int(String *value_str);
int int_k_exe(String **tokens , int len , int curr_i , Hashmap *HASHMAP_VOID);
void print_var(Hashmap *HASHMAP_VOID , VAR *var , char *var_name);
#endif //CRANEC_CGM_GRAMMA_H
