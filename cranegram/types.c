#include <stdio.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/map/hashmap_int.h"
#include "../tool/xokmalloc/xokmalloc.h"
#include "types.h"

PlaceHold *
create_PlaceHold(void)
{
    PlaceHold *ph = (PlaceHold*)malloc(sizeof(PlaceHold));
    if (ph == NULL) return NULL;

    ph->size = 10;
    ph->type = (String**)malloc(ph->size * sizeof(String*));
    ph->origin = (String**)malloc(ph->size * sizeof(String*));
    ph->in_format_indexs = (int*)malloc(ph->size * sizeof(int));

    for (int i = 0; i < ph->size; i++) {
        ph->type[i] = create_string("");
    }
    for (int i = 0; i < ph->size; i++) {
        ph->origin[i] = create_string("");
    }
    for (int i = 0; i < ph->size; i++) {
        ph->in_format_indexs[i] = -1;
    }

    ph->type_count = 0;
    ph->indexs_count = 0;
    ph->origin_count = 0;

    return ph;
}


PlaceHold *
extend_PlaceHold(PlaceHold *old)
{
    if (old == NULL) return NULL;

    int new_size = old->size * 2;

    // 1. 扩容 type 数组
    String **new_type = (String**)malloc(new_size * sizeof(String*));
    for (int i = 0; i < old->size; i++) {
        new_type[i] = old->type[i];   // 复制旧指针
    }
    for (int i = old->size; i < new_size; i++) {
        new_type[i] = create_string("");
    }
    free(old->type);
    old->type = new_type;

    // 2. 扩容 origin 数组
    String **new_origin = (String**)malloc(new_size * sizeof(String*));
    for (int i = 0; i < old->size; i++) {
        new_origin[i] = old->origin[i];
    }
    for (int i = old->size; i < new_size; i++) {
        new_origin[i] = create_string("");
    }
    free(old->origin);
    old->origin = new_origin;

    // 3. 扩容 in_format_indexs 数组
    int *new_indexs = (int*)malloc(new_size * sizeof(int));
    for (int i = 0; i < old->size; i++) {
        new_indexs[i] = old->in_format_indexs[i];
    }
    for (int i = old->size; i < new_size; i++) {
        new_indexs[i] = -1;
    }
    free(old->in_format_indexs);
    old->in_format_indexs = new_indexs;

    // 4. 更新 size
    old->size = new_size;

    return old;
}

void
free_PlaceHold(PlaceHold *ph)
{
    if (ph == NULL) return;

    // 1. 释放 type 数组里的每个 String
    if (ph->type != NULL) {
        for (int i = 0; i < ph->size; i++) {
            if (ph->type[i] != NULL) {
                string_free(ph->type[i]);
            }
        }
        free(ph->type);
    }

    // 2. 释放 origin 数组里的每个 String
    if (ph->origin != NULL) {
        for (int i = 0; i < ph->size; i++) {
            if (ph->origin[i] != NULL) {
                string_free(ph->origin[i]);
            }
        }
        free(ph->origin);
    }

    // 3. 释放 in_format_indexs 数组
    if (ph->in_format_indexs != NULL) {
        free(ph->in_format_indexs);
    }

    // 4. 释放结构体本身
    free(ph);
}

void 
PlaceHold_is_outsize(PlaceHold *placeHold)
{
    if(placeHold->origin_count == placeHold->size || placeHold->type_count == placeHold->size)
        extend_PlaceHold(placeHold);
}


PrintfVarArg *
create_PrintfVarArg(void)
{
    PrintfVarArg *pva = (PrintfVarArg*)malloc(sizeof(PrintfVarArg));
    if (pva == NULL) return NULL;

    pva->size = 10;
    pva->args = (String**)malloc(pva->size * sizeof(String*));
    if (pva->args == NULL) {
        free(pva);
        return NULL;
    }

    for (int i = 0; i < pva->size; i++) {
        pva->args[i] = create_string("");
    }

    pva->args_count = 0;

    return pva;
}

PrintfVarArg *
extend_PrintfVarArg(PrintfVarArg *old)
{
    if (old == NULL) return NULL;

    int new_size = old->size * 2;

    // 1. 分配新数组
    String **new_args = (String**)malloc(new_size * sizeof(String*));
    if (new_args == NULL) return old;

    // 2. 复制旧指针
    for (int i = 0; i < old->size; i++) {
        new_args[i] = old->args[i];
    }

    // 3. 新位置创建空 String
    for (int i = old->size; i < new_size; i++) {
        new_args[i] = create_string("");
    }

    // 4. 释放旧数组
    free(old->args);

    // 5. 更新
    old->args = new_args;
    old->size = new_size;

    return old;
}

void
free_PrintfVarArg(PrintfVarArg *pva)
{
    if (pva == NULL) return;

    // 1. 释放每个 String
    if (pva->args != NULL) {
        for (int i = 0; i < pva->size; i++) {
            if (pva->args[i] != NULL) {
                string_free(pva->args[i]);
            }
        }
        free(pva->args);
    }

    // 2. 释放结构体本身
    free(pva);
}

void
judge_is_extend(PrintfVarArg *printfVarArg)
{
    if(printfVarArg->args_count >= printfVarArg->size)
        extend_PrintfVarArg(printfVarArg);
}

TypeVar*
create_TypeVar()
{
    TypeVar *typeVar = (TypeVar*)malloc(sizeof(TypeVar));
    typeVar->var_name = create_string("");
    typeVar->type = create_string("");
    return typeVar;
}

void
free_TypeVar(TypeVar *typeVar)
{
    string_free(typeVar->var_name);
    string_free(typeVar->type);
    free(typeVar);
}

AssignStack *
create_AssignStack(XokMalloc *xokmalloc)
{
    int capacity = 10;

    AssignStack *assignStack = (AssignStack *)xalloc_XokMalloc(
        xokmalloc, sizeof(AssignStack));

    assignStack->var_name = (String **)xalloc_XokMalloc(
        xokmalloc, sizeof(String *) * capacity);

    assignStack->calc_sign = (String **)xalloc_XokMalloc(
        xokmalloc, sizeof(String *) * capacity);

    for (int i = 0; i < capacity; i++) {
        assignStack->var_name[i] = create_string("");
        assignStack->calc_sign[i] = create_string("");
    }

    assignStack->var_name_count = 0;
    assignStack->calc_sign_count = 0;
    assignStack->size = capacity;

    return assignStack;
}


AssignStack *
extend_AssignStack(XokMalloc *xokmalloc, AssignStack *old)
{
    int new_capacity = old->size * 2;

    AssignStack *assignStack = (AssignStack *)xalloc_XokMalloc(
        xokmalloc, sizeof(AssignStack));

    assignStack->var_name = (String **)xalloc_XokMalloc(
        xokmalloc, sizeof(String *) * new_capacity);

    assignStack->calc_sign = (String **)xalloc_XokMalloc(
        xokmalloc, sizeof(String *) * new_capacity);

    for (int i = 0; i < old->var_name_count; i++) {
        assignStack->var_name[i] = create_string("");
        copy_string(old->var_name[i], assignStack->var_name[i]);
    }
    for (int i = old->var_name_count; i < new_capacity; i++) {
        assignStack->var_name[i] = create_string("");
    }

    for (int i = 0; i < old->calc_sign_count; i++) {
        assignStack->calc_sign[i] = create_string("");
        copy_string(old->calc_sign[i], assignStack->calc_sign[i]);
    }
    for (int i = old->calc_sign_count; i < new_capacity; i++) {
        assignStack->calc_sign[i] = create_string("");
    }

    assignStack->var_name_count = old->var_name_count;
    assignStack->calc_sign_count = old->calc_sign_count;
    assignStack->size = new_capacity;

    return assignStack;
}