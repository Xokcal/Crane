#include <stdio.h>
#include "../tool/xokmalloc/xokmalloc.h"
#include "cracode.h"
#include "cravm.h"

#define VM_DEFAULT_STACK_SIZE 1024

vm *
vm_create(XokMalloc *xm, bytecode *bc, varnamestk *vns, int stack_size)
{
 if (stack_size <= 0) stack_size = VM_DEFAULT_STACK_SIZE;
 vm *v = (vm *)xalloc_XokMalloc(xm, sizeof(vm));
 if (!v) return NULL;
 v->stack = (int *)xalloc_XokMalloc(xm, sizeof(int) * stack_size);
 if (!v->stack) return NULL;
 v->bc         = bc;
 v->vns        = vns;
 v->sp         = 0;
 v->pc         = 0;
 v->stack_size = stack_size;
 return v;
}

void
vm_free(XokMalloc *xm, vm *v)
{
  if (!v) return;
  if (v->stack)v->stack = NULL;
}


