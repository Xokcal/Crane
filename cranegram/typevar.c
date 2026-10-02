#include <stdio.h>
#include "typevar.h"
#include "../tool/String/String.h"
#include "types.h"
#include "../tool/map/hashmap_int.h"
#include "cgm_gramma.h"
#include "common.h"

#define TYPE_CONVERTOR(type) ((type))
#define TYPE_RECEIVE(type) (type)
#define DOUBLE_STACK_RES_INDEX 0

char *typeVars[TYPE_VARS_LEN] = {"int" , "float" , "string"};

char *calcSigns[CALC_SIGN_LEN] = {"+" , "-" , "*" , "/" , "^"};

int isTypeVarToken(String *string){
    for (int i = 0; i < TYPE_VARS_LEN; ++i) {
        if(compare(string , typeVars[i]) == 1)return 1;
    }
    return 0;
}

static int
is_calc_sign(String *token)
{
    for(int i = 0; i < CALC_SIGN_LEN ; i++){
        if(compare(token , calcSigns[i]))return 1;
    }
    return 0;
}

static String* 
int_to_String(int p){
    char buf[20];
    snprintf(buf , sizeof(buf) , "%d" , p);
    return create_string(buf);
}

static int
judge_calcSign_priority(String *calcSign){
    if(compare(calcSign , "*") || compare(calcSign , "/"))return 1;return 0;}

static int
various_calcSign_calc(int *p1 , int *p2 , String *calcSign)
{
    if(compare(calcSign , "+"))return CALCULATE_ADD(p1 , p2);
    if(compare(calcSign , "-"))return CALCULATE_SUB(p1 , p2);
    if(compare(calcSign , "*"))return CALCULATE_MUL(p1 , p2);
    if(compare(calcSign , "/")){
        if(*p2 == 0){CRA_ERROR("/ 0");return 0;}
        return CALCULATE_DIVI(p1 , p2);}
    return 0;
}

static void
priority_rescalc_free(int *p1 , int *p2){ free(p1) , free(p2);}

static int
prioritysign_rescalc(AssignStack *doubleStack , AssignStack *assignStack , String *curr_calcsign , int *v_i)
{
    int* calc_curr = str_convert_int(doubleStack->var_name[doubleStack->var_name_count - 1]);
    int* calc_next = str_convert_int(assignStack->var_name[++(*v_i)]);
    int r = various_calcSign_calc(calc_curr , calc_next , curr_calcsign);
    priority_rescalc_free(calc_curr , calc_next);
    return r;
}

static void
priority_res_replace(AssignStack *doubleStack , int res , int *previous_sign_priority , int *c_i)
{
    String *res_str = int_to_String(res);
    copy_string(res_str ,doubleStack->var_name[doubleStack->var_name_count - 1]);
    (*previous_sign_priority) = 1 , (*c_i)++;
    string_free(res_str);
}

static void
prioritySign_calc_core(AssignStack *doubleStack , AssignStack *assignStack 
    , int *v_i , String *curr_calcsign , int *previous_sign_priority , int *c_i , int *double_name_i)
{
    int res = prioritysign_rescalc(doubleStack , assignStack , curr_calcsign , v_i);
    priority_res_replace(doubleStack , res , previous_sign_priority , c_i);
}

static int
lowpriority_res_calc(AssignStack *doubleStack , int *doubleStack_calc_count , int *doubleStack_varName_count)
{
    String *currSign = doubleStack->calc_sign[(*doubleStack_calc_count)++];
    int *p1 = str_convert_int(doubleStack->var_name[(*doubleStack_varName_count)++]);
    int *p2 = str_convert_int(doubleStack->var_name[(*doubleStack_varName_count)++]);
    int res = various_calcSign_calc(p1 , p2 , currSign);
    priority_rescalc_free(p1 , p2);
    return res;
}

static void
lowpriority_res_replace(AssignStack *doubleStack , int temp_r , int *doubleStack_varName_count)
{
    String *lowPriority_res = int_to_String(temp_r);
    copy_string(lowPriority_res , doubleStack->var_name[--(*doubleStack_varName_count)]);
    string_free(lowPriority_res);
}

static String*
doubleStack_lowPriority_calc(AssignStack *assignStack , AssignStack *doubleStack , int c_i)
{
    if(c_i == 0)return assignStack->var_name[0];
    int doubleStack_varName_count = 0 , doubleStack_calc_count = 0;
    while(doubleStack_calc_count < (doubleStack->calc_sign_count)){
        int temp_r = lowpriority_res_calc(doubleStack , &doubleStack_calc_count , &doubleStack_varName_count);
        lowpriority_res_replace(doubleStack , temp_r , &doubleStack_varName_count);
    }
    return doubleStack->var_name[doubleStack->var_name_count - 1];
}

static void
doublestackpush_lowpriority_condition(AssignStack *doubleStack , String *curr_calcsign 
    , int *previous_sign_priority ,  int *v_i , int *c_i)
{
    copy_string(curr_calcsign , doubleStack->calc_sign[doubleStack->calc_sign_count++]);
    *previous_sign_priority = 0, (*v_i)++, (*c_i)++;
}

static void
doublestackpush_process_main(AssignStack *assignStack , AssignStack *doubleStack 
    , int *c_i , int *previous_sign_priority , int *v_i , int *double_name_i)
{
    while(*c_i <= assignStack->calc_sign_count){
        //when sign is "*" and after has calculated 
        //the result position replace is equal function'copy_string(insertPosition , assignStack);'.
        if((*previous_sign_priority) == 0){
            copy_string(assignStack->var_name[*v_i] ,doubleStack->var_name[doubleStack->var_name_count++]);
        }
        String *curr_calcsign = assignStack->calc_sign[*c_i];
        if(judge_calcSign_priority(curr_calcsign)){
            prioritySign_calc_core(doubleStack , assignStack , v_i 
                , curr_calcsign , previous_sign_priority , c_i , double_name_i);
            continue;
        }
        doublestackpush_lowpriority_condition(doubleStack , curr_calcsign 
            , previous_sign_priority , v_i , c_i);
    }
}

static int*
AssignStack_DoubleStack_calc(XokMalloc *xokmalloc ,AssignStack *assignStack)
{
    int v_i = 0 , c_i = 0 , double_name_i , previous_sign_priority = 0;
    AssignStack *doubleStack = create_AssignStack(xokmalloc);
    doublestackpush_process_main(assignStack , doubleStack , &c_i , &previous_sign_priority , &v_i , &double_name_i);
    String *str_res = doubleStack_lowPriority_calc(assignStack , doubleStack , c_i);
    string_free(str_res);
    return str_convert_int(str_res);
}

static int
typeVar_AssignStack_collect(XokMalloc *xokmalloc , TOKENSB *t, int curr , AssignStack *assignStack)
{
    for(int i = curr; i < t->count ; i++){
        String *token = t->tokens[i];
        if(compare(token , ";"))return i - 1;
        if(!isContainKeys(token)&&!is_calc_sign(token))
        copy_string(token , assignStack->var_name[assignStack->var_name_count++]);
        if(is_calc_sign(token))
        copy_string(token , assignStack->calc_sign[assignStack->calc_sign_count++]);
    }
}

// int a = 100 + 20
int
CraTypeVar_Parse(XokMalloc *xokmalloc ,TOKENSB *t , Hashmap *hashmap , int curr)
{
  int is_over_type = 0;
  int is_over_typeName = 0;
  int is_over_equal = 0;
  String *k = create_string("");
  String *name = create_string("");
  void *res;
  AssignStack *assignStack = create_AssignStack(xokmalloc);
  for(int i = curr ; i < t->count ; i++){
    String *token = t->tokens[i];
    if(!is_over_equal && compare(token , "=")){
        is_over_equal = 1;
        continue;
    }
    if(compare(token , ";")){
        res = AssignStack_DoubleStack_calc(xokmalloc , assignStack);
        map_put(hashmap , name->str , k , res);
        return i;
    }
    if(is_over_equal){
        i = typeVar_AssignStack_collect(xokmalloc , t , i , assignStack);
        continue;
    }
    if(!is_over_type&&isTypeVarToken(token)){
        combine_tail(k , token->str);
        is_over_type = 1;
        continue;
    }
    if(is_over_type&&!compare(token , " ")){
        combine_tail(name , token->str);
        is_over_typeName = 1;
        continue;
    }
  }
}