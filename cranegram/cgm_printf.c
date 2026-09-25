#include <stdio.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/map/hashmap_int.h"
#include "../tool/xokmalloc/xokmalloc.h"
#include "cgm_printf.h"
#include "types.h"

#define CRA_PRINTF_MODULE(content , param) printf((content) , (param));
#define CRA_PRINTF_MODULE_NO_PARAM(content) printf("%s" , (content));
#define PRINTF_KEY_LEN(keys) sizeof(keys) / sizeof(keys[0])
#define PRINTF_LN printf("\n")
#define PRINTF_SINGLE_PLACEHOLD(placeHold , content) \
        printf((placeHold) , (content))

static int
contain_printf_PlaceHold(String *str)
{
    for(int i = 0 ; i < PRINTF_KEY_LEN(printf_PlaceHolds) ; i++)
        if(compare(str , printf_PlaceHolds[i]))return 1;
    return 0;
}

static String*
printf_varArgs_string(TOKENSB *t ,int *i)
{
    String *temp = create_string("");
    for(int j = ((*i) + 1); j < t->count ; j++){
        String *token = t->tokens[j];
        if(compare(token , "\"")){
            *i = j;
            return temp;
        }
        combine_tail(temp , token->str);
    }
}

static int
printf_varArgs_collect(TOKENSB *t , PrintfVarArg *printfVarArg 
    , int curr , int *is_entre_varArgs)
{
    *is_entre_varArgs = 1;
    int is_over_dot = 0;
    String *str_type = create_string("");
    for(int i = curr + 1; i < t->count ; i++){
        String *token = t->tokens[i];
        if(compare(token , ")"))return i;
        if(!compare(token , " ")&&!compare(token , ",")&&!compare(token , "\"")){
            judge_is_extend(printfVarArg);
            copy_string(token , printfVarArg->args[printfVarArg->args_count++]);
            continue;
        }
        if(compare(token , "\"")){
            copy_string(printf_varArgs_string(t , &i) , str_type);
            copy_string(str_type , printfVarArg->args[printfVarArg->args_count++]);
            delete_all(str_type);
            continue;
        }
    }
}

static int
printf_format_content(TOKENSB *t , PlaceHold *placeHold 
    , int curr , int *is_over_doubleQueto)
{
    *is_over_doubleQueto = 1;
    for(int i = curr; i < t->count ; i++){
        String *token = t->tokens[i];
        if(compare(token  ,"\""))return i;
        if(contain_printf_PlaceHold(token)){ // is placeHold
            PlaceHold_is_outsize(placeHold);
            String *type = placeHold->type[placeHold->type_count++];
            copy_string(token , type);
        }
        PlaceHold_is_outsize(placeHold);
        String *origin = placeHold->origin[placeHold->origin_count++];
        copy_string(token , origin);
    }
}

static void
placeHold_type_match_index(PlaceHold *placeHold)
{
    for(int i = 0 ; i < placeHold->origin_count ; i++){
        String *origin = placeHold->origin[i];
        if(contain_printf_PlaceHold(origin))
            placeHold->in_format_indexs[placeHold->indexs_count++] = i;
    }
}

static int
is_reach_format_type(PlaceHold *placeHold , int curr)
{
    for(int i = 0 ; i < placeHold->indexs_count; i++){
        if(placeHold->in_format_indexs[i] == curr)return 1;
    }return 0;
}

static void
printf_output_varArgs_empty(PlaceHold *placeHold)
{
    for(int i =0; i < placeHold->origin_count ; i++){
        if(compare(placeHold->origin[i] , "\\n")){
            PRINTF_LN;continue;
        }
        PRINTF_SINGLE_PLACEHOLD("%s" , placeHold->origin[i]->str);
    }
}

static void
printf_output(PlaceHold *placeHold ,PrintfVarArg *printfVarArg)
{
    placeHold_type_match_index(placeHold);
    int args_index = 0;
    if(printfVarArg->args_count == 0){
        printf_output_varArgs_empty(placeHold);return;
    }
    for(int i = 0 ; i < placeHold->origin_count; i++){
        if(compare(placeHold->origin[i] , "\\n")){
            PRINTF_LN;continue;
        }
        if(is_reach_format_type(placeHold , i)){
            PRINTF_SINGLE_PLACEHOLD("%s" , printfVarArg->args[args_index++]->str);
            continue;
        }
        PRINTF_SINGLE_PLACEHOLD("%s" , placeHold->origin[i]->str);
    }
}

int
CraPrintf_fule(XokMalloc *xokmalloc ,TOKENSB *t , Hashmap *hashmap , int curr)
{
    int is_over_printfBracket = 0;
    int is_over_doubleQueto = 0;
    int is_storage_content = 0;
    int is_entre_varArgs = 0;
    int is_over_format_conent = 0;
    int is_over_varArgs_collect = 0;
    String *content = (String *)xalloc_XokMalloc(xokmalloc , sizeof(String));
    String *tempParam = (String *)xalloc_XokMalloc(xokmalloc , sizeof(String));
    PlaceHold *placeHold = create_PlaceHold();
    PrintfVarArg *printfVarArg = create_PrintfVarArg();
    for(int i = curr ; i < t->count ; i++){
        String *token = t->tokens[i];
        if(!is_over_doubleQueto&&compare(token , "\"")){
            i = printf_format_content(t , placeHold , i + 1 , &is_over_doubleQueto ); // return \" index
            is_over_format_conent = 1;
            continue;
        }
        if(is_over_format_conent&&compare(token , ")")&&!is_over_varArgs_collect){
            printf_output(placeHold ,printfVarArg);
            return i + 1;
        }
        if(!is_entre_varArgs&&compare(token , ",")){
            i = printf_varArgs_collect(t , printfVarArg , i , &is_entre_varArgs);
            is_over_varArgs_collect = 1;
            content;
        }
        if(is_entre_varArgs&&compare(token , ";")){
            printf_output(placeHold ,printfVarArg);
            return i;
        }
    }
}

