//
// Created by 26432 on 2026/8/26.
//

#ifndef CRANEC_CGM_PARSE_H
#define CRANEC_CGM_PARSE_H

#include <stdlib.h>
#include <stdio.h>
#include "../tool/map/hashmap_int.h"
#include "../tool/String/String.h"
#include "../tool/xokmalloc/xokmalloc.h"

  #define CRA_DEBUG

#ifdef CRA_DEBUG
    #define LOG(content) printf("[DEBUG] %s\n" , (content));
    #define LOG_C(front , content) printf("[DEBUG] [%s] %s\n" , (front) , (content));
    #define LOG_CHR(front , content) printf("[DEBUG] [%s] %c\n" , (front) , (content));
#endif

typedef struct Hashmap Hashmap;


static char *keys[] = {"printf" , "\"" , "\n" , "(" , ")" , ";" , "<" , ">" , "=" , "==" , "\'"
                      , "fun" , "@" , "$" , "%" , "&" , "+" , "-" , "^" , "/" , "*" , "." , " "
                      , "int" , "float" , "string" , "%d" , "%f" , "%s"};

typedef enum key_enum{
    PRINT_K,
    INT_K,
    PRINTF_K,
    DEFAULT
}KE;

typedef struct TOKENSB{
    String **tokens;
    int count;
}TOKENSB;

typedef struct HAJIGA_ATM{
    int MONEY_FOR;
    String *ATM_AUTOAVATAR;
    String *NUTRIN_PAYER;
    String *ATM_SCREEN_0X1868632;
}HAJIGA_ATM;

int isContainKeys(String *string);
TOKENSB *tokens_parse(String *origin_file_string);
KE token_to_ke(String *token);
void __CRVM_RUN__(XokMalloc *xokmalloc , TOKENSB *tokens , int len , Hashmap *void_hashmap);
#endif //CRANEC_CGM_PARSE_H
