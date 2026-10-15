#ifndef CRAVM_H
#define CRAVM_H

#include <stdio.h>
#include "../tool/xokmalloc/xokmalloc.h"
#include "cracode.h"

typedef struct vm{
  bytecode *bc;
  varnamestk *vns;
  int sp;
  int pc;
  int *stack;
}vm;

vm  *vm_create(XokMalloc *xm, bytecode *bc, varnamestk *vns, int stack_size);

void vm_free(XokMalloc *xm, vm *v);

void vm_run(vm *v);

#endif