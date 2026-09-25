#include <stdio.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/map/hashmap_int.h"
#include "../tool/xokmalloc/xokmalloc.h"

typedef struct PlaceHold{
    int indexs_count;
    int size;
    int type_count;
    int origin_count;
    int *in_format_indexs;
    String **type;
    String **origin;
}PlaceHold;

typedef struct PrintfVarArg{
    String **args;
    int args_count;
    int size;
}PrintfVarArg;

/* typevar.h struct!*/
typedef struct TypeVar{
    String *var_name;
    String *type;
}TypeVar;

typedef struct AssignStack{
    String **var_name;
    String **calc_sign;
    int var_name_count;
    int calc_sign_count;
    int size;
}AssignStack;


void
free_PlaceHold(PlaceHold *ph);

PlaceHold *
extend_PlaceHold(PlaceHold *old);

PlaceHold *
create_PlaceHold(void);

void 
PlaceHold_is_outsize(PlaceHold *placeHold);


/* Create a new PrintfVarArg with initial capacity 10.
   Returns NULL on failure. */
PrintfVarArg *create_PrintfVarArg(void);

/* Double the capacity of the PrintfVarArg.
   Returns the (possibly new) pointer. */
PrintfVarArg *extend_PrintfVarArg(PrintfVarArg *old);

/* Free the PrintfVarArg and all its Strings. */
void free_PrintfVarArg(PrintfVarArg *pva);

void
judge_is_extend(PrintfVarArg *printfVarArg);


TypeVar*
create_TypeVar();

void
free_TypeVar(TypeVar *typeVar);

AssignStack *
create_AssignStack(XokMalloc *xokmalloc);


AssignStack *
extend_AssignStack(XokMalloc *xokmalloc, AssignStack *old);


