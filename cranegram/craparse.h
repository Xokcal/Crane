#ifndef CRANEC_CRAPARSE
#define CRANEC_CRAPARSE

#include <stdio.h>
#include <string.h>
#include "cgm_parse.h"

typedef enum cragramtype{
  GRAM_INT,
  GRAM_STRING,
  GRAM_FLOAT,
  GRAM_PRINTF
}cragramtype;

typedef enum craasttype{
  /* key node */
  AST_VAR,
  /* child node */
  AST_NUM,
  AST_ASSIGN,
  AST_BINOP
}craasttype;

typedef struct craastdesc{
  craasttype kind;   /* ast's kind */
  union{
    char *varname;   /* ast's variable name */
    int num;         /* ast's node number */
    struct binop{ 
      struct craastdesc *right;   /* binop'sright */
      char *calcsign;      /* binop's calculate aign */
      struct craastdesc *left;    /* binop's left */
    } binop;
    struct assign{
      char *name;     /* assign about of variable name */
      struct craastdesc *value;   /* assign value */
    } assign;
  } as;
}craastdesc;

typedef struct desclist{
  craastdesc **desclist;
  int size;
  int count;
}desclist;

typedef struct ParseFactor{
  TOKENSB *tokens;
  int curr;
}ParseFactor;

typedef struct AstDescArr{
  craastdesc **descs;
  int count;
  int size;
}AstDescArr;

typedef struct pavstate{
  XokMalloc *xm;
  ParseFactor *pf;
  craastdesc *root;
}pavstate;

/* crane ast parse main funtion, xokMalloc is memory pool */
void Cra_token_parse(XokMalloc *xokMalloc , TOKENSB *tokens , desclist *dl);

desclist *Cra_desclist_create(XokMalloc *xm, int init_size);

void Cra_desclist_free(XokMalloc *xm, desclist *dl);

int Cra_desclist_push(XokMalloc *xm, desclist *dl, craastdesc *tree);

int Cra_desclist_expand(XokMalloc *xm, desclist *dl);
#endif