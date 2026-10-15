#ifndef CRACODE_H
#define CRACODE_H

#include <stdio.h>
#include "../tool/xokmalloc/xokmalloc.h"
#include "craparse.h"

typedef enum opcode{
  OP_PUSH,
  OP_ADD,
  OP_SUB,
  OP_DIV,
  OP_MUL,
  OP_HALT,
  OP_STORE
}opcode;

typedef struct bytecode{
  int *codes;
  int count;
  int size;
}bytecode;

typedef struct varnamestk {
    char **vn; 
    int *slots;
    int size;
    int count;
} varnamestk;

vm*
craC_bytecode_parse(XokMalloc *xm ,  desclist *dl);

#endif