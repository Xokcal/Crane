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
  OP_HALT
}opcode;

typedef struct bytecode{
  int *codes;
  int count;
  int size;
}bytecode;

void
craC_bytecode_parse(XokMalloc *xm ,  desclist *dl);

#endif