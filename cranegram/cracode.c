#include <stdio.h>
#include <string.h>
#include "cracode.h"
#include "../tool/xokmalloc/xokmalloc.h"
#include "craparse.h"
#include "cgm_parse.h"

#define BYTECODE_DEFAULT_SIZE 16
#define BYTECODE_GROW_FACTOR  2
#define craC_codes_log(bc) \
  for(int i = 0; i < (bc)->count; i++) LOG_I("BC" , (bc)->codes[i]);

static bytecode*
bc_create(XokMalloc *xm, int init_size)
{
  if (init_size <= 0) init_size = BYTECODE_DEFAULT_SIZE;
  bytecode *bc = (bytecode *)xalloc_XokMalloc(xm, sizeof(bytecode));
  if (!bc) return NULL;
  bc->codes = (int *)xalloc_XokMalloc(xm, sizeof(int) * init_size);
  if (!bc->codes) return NULL;
  bc->count = 0;
  bc->size  = init_size;
  return bc;
}


static void
bc_free(XokMalloc *xm, bytecode *bc)
{
  if (!bc) return;
  if (bc->codes) bc->codes = NULL;
}


static int
bc_expand(XokMalloc *xm, bytecode *bc)
{
  if (!bc) return 0;
  int new_size = bc->size * BYTECODE_GROW_FACTOR;
  int *new_codes = (int *)xalloc_XokMalloc(xm, sizeof(int) * new_size);
  if (!new_codes) return 0;
  memcpy(new_codes, bc->codes, sizeof(int) * bc->count);
  bc->codes = new_codes;
  bc->size  = new_size;
  return 1;
}


static int
emit(XokMalloc *xm, bytecode *bc, int instr)
{
  if (!bc) return 0;
  if (bc->count >= bc->size) {
    if (!bc_expand(xm, bc)) return 0;
  }
  bc->codes[bc->count++] = instr;
  return 1;
}


static int
sign_emit(XokMalloc *xm , char *s , bytecode *bc)
{
  if(strcmp(s , "+")==0)return emit(xm , bc , OP_ADD);
  if(strcmp(s , "-")==0)return emit(xm , bc , OP_SUB);
  if(strcmp(s , "*")==0)return emit(xm , bc , OP_MUL);
  if(strcmp(s , "/")==0)return emit(xm , bc , OP_DIV);
}



static void
assign_encode(XokMalloc *xm, craastdesc *node, bytecode *bc)
{
  if (node->kind == AST_NUM) { /* single tackle node
    is AST_NUM, if is binop node don't push*/
    emit(xm, bc, OP_PUSH);
    emit(xm, bc, node->as.num);
    return;
  }
  if (node->kind == AST_BINOP) { /* left-right-mid recursion，
    leftest is highest priority binop, priority tackle leftest binop
    left-num and right-num, rightest is low priority, can tabckle later */
    assign_encode(xm, node->as.binop.left, bc);
    assign_encode(xm, node->as.binop.right, bc);
    sign_emit(xm, node->as.binop.calcsign, bc);
    return;
  }
  return;
}


static bytecode*
assign_code(XokMalloc *xm , craastdesc *desc)
{
  bytecode *bc = bc_create(xm , BYTECODE_DEFAULT_SIZE);
  craastdesc *value = desc->as.assign.value;
  assign_encode(xm , value , bc);
  return bc;
}


void
craC_bytecode_parse(XokMalloc *xm ,  desclist *dl)
{
  int dl_c = 0;
  bytecode *bc = NULL;
  while(dl_c < dl->count){
    craastdesc *curdesc = dl->desclist[dl_c];
    switch(curdesc->kind){
      case AST_ASSIGN:{
        bc = assign_code(xm , curdesc);
        dl_c++;
        break;
      }
      case AST_EMPTY:{
        break;
      }
      default:{
        break;
      }
    }
  }
  emit(xm , bc , OP_HALT);
  craC_codes_log(bc);
}