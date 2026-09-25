//
// Created by 26432 on 2026/8/26.
//

#include <stdio.h>
#include <stdlib.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/log/log.h"
#include "../tool/map/hashmap_int.h"
#include "cgm_gramma.h"
#include "cgm_printf.h"
#include "../tool/map/hashmap_int.h"
#include "../tool/xokmalloc/xokmalloc.h"
#include "typevar.h"

int isContainKeys(String *string){
    for (int i = 0; i < 29; ++i) {
        if(compare(string , keys[i]) == 1)
            return 1;
    }
    return 0;
}

TOKENSB *tokens_parse(String *origin){
    String **tokens = (String**)malloc(2000000 * sizeof(String*));
    String *temp = create_string("");
    String *temp_test = create_string("");
    String *temp_longest_match = create_string("");
    int count = 0;
    for (int i = 0; i < origin->length; ++i) {
        //LOG_CHR("---i---" , origin->str[i]);
        combine_tail_char(temp , origin->str[i]);
        if(isContainKeys(temp) == 1){
            int longest_count = 1;
            combine_tail(temp_longest_match , temp->str);
            if(origin->str[i + 1] == ' ' ||  (i + 1) == origin->length){
                goto notAddBehind;
            }
            combine_tail_char(temp_longest_match , origin->str[i + longest_count]);
            //LOG_C("[1]" , temp_longest_match->str);
            if(isContainKeys(temp_longest_match)){
                //LOG("KEYS");
                while(!(origin->str[i + (++longest_count)] == ' ') || (1 + longest_count) != origin->length){
                    combine_tail_char(temp_longest_match , origin->str[i + longest_count]);
                    if(!isContainKeys(temp_longest_match)){
                        delete_indexof(temp_longest_match , temp_longest_match->length - 1);
                        break;
                    }
                }
                tokens[count] = create_string("");
                //LOG_C("---collect---/longest" , temp_longest_match->str);
                combine_tail(tokens[count++] , temp_longest_match->str);
                delete_all(temp);
                delete_all(temp_longest_match);
                i = i + 1;
                continue;
            }
            notAddBehind:
            tokens[count] = create_string("");
            //LOG_C("---collect---/keys" , temp->str);
            combine_tail(tokens[count++] , temp->str);
            delete_all(temp);
            delete_all(temp_longest_match);
            continue;
        } else {
            combine_tail_char(temp_test , origin->str[i + 1]);
            if(isContainKeys(temp_test)){
                tokens[count] = create_string("");
                //LOG_C("---collect---/common" , temp->str);
                combine_tail(tokens[count++] , temp->str);
                delete_all(temp);
            }
            delete_all(temp_test);
            continue;
        }
    }
    //printf("[count]%d\n" , count);
    string_free(temp);
    string_free(temp_test);
    string_free(temp_longest_match);
    TOKENSB *tokensb = (TOKENSB*)malloc(sizeof(TOKENSB));
    tokensb->tokens = tokens;
    tokensb->count = count;
    return tokensb;
}

KE token_to_ke(String *token){
    if(compare(token , "print") == 1)return PRINT_K;
    else if(compare(token , "int") == 1)return INT_K;
    else if(compare(token , "printf") == 1)return PRINTF_K;
    else return DEFAULT;
}

void __CRVM_RUN__(XokMalloc *xokmalloc , TOKENSB *tokens , int len , Hashmap *void_hashmap){
    for (int i = 0; i < len; ++i) {
        //LOG("entre CRVM!!");
        KE token_KE = token_to_ke(tokens->tokens[i]);
        switch (token_KE) {
            case PRINTF_K:
                i = CraPrintf_fule(xokmalloc , tokens , void_hashmap , i);
                continue;
            case INT_K:
                i = CraTypeVar_Parse(xokmalloc , t , void_hashmap , i);
                continue;
            case DEFAULT:
                break;
        }
    }

}
