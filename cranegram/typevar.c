#include <stdio.h>
#include "typevar.h"
#include "../tool/String/String.h"
#include "types.h"
#include "../tool/map/hashmap_int.h"
#include "cgm_gramma.h"

#define TYPE_CONVERTOR(type) ((type))
#define TYPE_RECEIVE(type) (type)
#define DOUBLE_STACK_RES_INDEX 0

int isTypeVarToken(String *string){
    for (int i = 0; i < TYPE_VARS_LEN; ++i) {
        if(compare(string , typeVars[i]) == 1)
            return 1;
    }
    return 0;
}

static int
is_calc_sign(String *token)
{
    for(int i = 0; i < CALC_SIGN_LEN ; i++){
        if(compare(token , calcSigns[i])){
            return 1;
        }
    }
    return 0;
}

static String*
int_to_String(int p)
{
    char buf[20];
    snprintf(buf , sizeof(buf) , "%d" , p);
    String *strInt = create_string(buf);
}

static int
judge_calcSign_priority(String *calcSign)
{
    if(compare(calcSign , "*") || compare(calcSign , "/"))return 1;
    return 0;
}

static int
various_calcSign_calc(int *p1 , int *p2 , String *calcSign)
{
    if(compare(calcSign , "+"))return (*p1) + (*p2);if(compare(calcSign , "-"))return (*p1) - (*p2);
    if(compare(calcSign , "*"))return (*p1) * (*p2);if(compare(calcSign , "*"))return (*p1) / (*p2);
    return 0;
}

static void
prioritySign_calc_core(AssignStack *doubleStack , AssignStack *assignStack 
    , int *v_i , String *curr_calcSign)
{
    int* temp_stack_v = str_convert_int(doubleStack->var_name[--doubleStack->var_name_count]);
    int* next_stack_v = str_convert_int(assignStack->var_name[++(*v_i)]);
    int r = various_calcSign_calc(temp_stack_v , next_stack_v , curr_calcSign);
    String *r_str = int_to_String(r);
    copy_string(r_str ,doubleStack->var_name[doubleStack->var_name_count++]);
    string_free(r_str);
}

// [60 , 3 , 6]  [+ , -]
static String*
doubleStack_lowPriority_calc(AssignStack *doubleStack)
{
    int doubleStack_varName_count = 0 , doubleStack_calc_count = 0;
    while(doubleStack_varName_count < (doubleStack->calc_sign_count - 1)){
        String *currSign = doubleStack->calc_sign[doubleStack_calc_count++];
        int *p1 = str_convert_int(doubleStack->var_name[doubleStack_varName_count++]);
        int *p2 = str_convert_int(doubleStack->var_name[doubleStack_varName_count++]); // index = 2
        int temp_r = various_calcSign_calc(p1 , p2 , currSign);
        // storage to p2 position.  --   [16 , 2] [+]  ------------------->  [18]
        //                                |    \______________. p2             \____________. p2
        //                                \______________. p1
        copy_string(int_to_String(temp_r) , doubleStack->var_name[--doubleStack_varName_count]);
    }
    return doubleStack->var_name[doubleStack->var_name_count - 1];
}

static String*
AssignStack_DoubleStack_calc(XokMalloc *xokmalloc ,AssignStack *assignStack)
{
    int v_i = 0 , c_i = 0;
    AssignStack *doubleStack = create_AssignStack(xokmalloc);
    while(c_i < assignStack->calc_sign_count){
        copy_string(assignStack->var_name[v_i] ,doubleStack->var_name[doubleStack->var_name_count++]);
        String *curr_calcSign = assignStack->calc_sign[c_i];
        if(judge_calcSign_priority(curr_calcSign)){
            prioritySign_calc_core(doubleStack , assignStack , &v_i , curr_calcSign);
            v_i++;c_i++;continue;
        }
        copy_string(curr_calcSign , doubleStack->calc_sign[doubleStack->calc_sign_count++]);
        v_i++;c_i++;
    }
    return doubleStack_lowPriority_calc(doubleStack);
}

static int
typeVar_AssignStack_collect(XokMalloc *xokmalloc , TOKENSB *t 
    , int curr , AssignStack *assignStack)
{
    for(int i = 0; i < t->count ; i++){
        String *token = t->tokens[i];
        if(compare(token , ";"))return i - 1;
        if(!isContainKeys(token)&&!is_calc_sign(token)){ // varName
            copy_string(token , assignStack->var_name[assignStack->var_name_count++]);
            continue;
        }
        if(is_calc_sign(token)){ // "+" "-" "*" "/"
            copy_string(token , assignStack->calc_sign[assignStack->calc_sign_count++]);
            continue;
        }
    }
}

int
CraTypeVar_Parse(XokMalloc *xokmalloc ,TOKENSB *t , Hashmap *hashmap , int curr)
{
    int is_over_type = 0;
    int is_over_typeName = 0;
    int is_over_equal = 0;
    String *k = create_string("");
    String *name = create_string("");
    String *R;
    AssignStack *assignStack = create_AssignStack(xokmalloc);
    for(int i = 0 ; i < t->count ; i++){
        String *token = t->tokens[i];
        if(compare(token , ";")){
            R = AssignStack_DoubleStack_calc(xokmalloc , assignStack);
            map_put(hashmap , name->str , k , R);
            return i;
        }
        if(is_over_equal){
            i = typeVar_AssignStack_collect(t , i , assignStack);
            continue;
        }
        if(!is_over_type&&isTypeVarToken(token)){
            combine_tail(k , token->str);
            is_over_type = 1;
            continue;
        }
        if(is_over_type&&compare(token , " ")){
            combine_tail(name , token->str);
            is_over_typeName = 1;
            continue;
        }
        if(!is_over_equal && compare(token , "=")){
            is_over_equal = 1;
            continue;
        }
    }
}