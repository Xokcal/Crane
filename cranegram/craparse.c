#include <stdio.h>
#include "craparse.h"
#include "../tool/xokmalloc/xokmalloc.h"
#include "cgm_parse.h"
#include "cgm_gramma.h"

#define craP_ps_currtoken(ps) (ps).pf->tokens->tokens[(ps).pf->curr]

#define craP_pf_currtoken(pf) (pf)->tokens->tokens[(pf)->curr]

static int
t_convert_asttype(String *s)
{
  if(compare(s , "int"))return GRAM_INT;
}


static int
accept(ParseFactor *pf , char *name)
{
  if(compare(pf->tokens->tokens[pf->curr] , name)){
    pf->curr++;
    return 1;
  }
  return 0;
}

static void
accept_empty(ParseFactor *pf)
{
  while(pf->curr < pf->tokens->count)
  {
    String *s = pf->tokens->tokens[pf->curr];
    if(compare(s , " "))pf->curr++;
    else return;
  }
}


static void
pav_clear_string(String *num_p1 , String *num_p2 
, String *calcsign , String *calcsign2)
{
  delete_all(num_p1);
  delete_all(num_p2);
  delete_all(calcsign);
  delete_all(calcsign2);
}


static pavstate
pavstate_init(XokMalloc *xm,ParseFactor *pf)
{
  pavstate          pavs;
  pavs.xm           = xm;
  pavs.pf           = pf;
  pavs.root         = NULL;
  pavs.curr_node    = NULL;
  pavs.temp_node    = NULL;
  pavs.num_p1       = create_string("");
  pavs.num_p2       = create_string("");
  pavs.calcsign     = create_string("");
  pavs.calcsign2    = create_string("");
  return pavs;
}


static void
print_ast(craastdesc *node, int depth)
{
  if (!node) {
      printf("%*s(null)\n", depth * 2, "");
      return;
  }
  printf("%*s", depth * 2, "");
  switch (node->kind) {
    case AST_NUM:
      printf("NUM: %d\n", node->as.num);
      break;
    case AST_VAR:
      printf("VAR: %s\n", node->as.varname);
      break;
    case AST_BINOP:
      printf("BINOP: %s\n", node->as.binop.calcsign);
      print_ast(node->as.binop.left,  depth + 1);
      print_ast(node->as.binop.right, depth + 1);
      break;
    case AST_ASSIGN:
      printf("ASSIGN: %s\n", node->as.assign.name);
      print_ast(node->as.assign.value, depth + 1);
      break;
    default:
      printf("UNKNOWN kind=%d\n", node->kind);
      break;
  }
}


craastdesc *make_var(XokMalloc *xokmalloc , char *name)
{
  craastdesc *node = (craastdesc *)
  xalloc_XokMalloc(xokmalloc , sizeof(craastdesc));
  node->kind = AST_VAR;
  node->as.varname = strdup(name);  /* remember free node name */
  return node;
}


craastdesc *make_num(XokMalloc *xokMalloc ,  int n)
{
  craastdesc *node = (craastdesc *)
  xalloc_XokMalloc(xokMalloc , sizeof(craastdesc));
  node->kind = AST_NUM;
  node->as.num = n;
  return node;
}


craastdesc *make_binop(XokMalloc *xokMalloc, char *calcsign 
    , craastdesc *left , craastdesc *right)
{
  craastdesc *node = (craastdesc *)
  xalloc_XokMalloc(xokMalloc , sizeof(craastdesc));
  node->kind = AST_BINOP;
  node->as.binop.calcsign = strdup(calcsign); /* remember free node.left! */
  node->as.binop.left = left; 
  node->as.binop.right = right;
  return node;
}


craastdesc *make_assign(XokMalloc *xokMalloc , char *name , craastdesc *value)
{
  craastdesc *node = (craastdesc *)
  xalloc_XokMalloc(xokMalloc , sizeof(craastdesc));
  node->kind = AST_ASSIGN;
  node->as.assign.name = strdup(name);
  node->as.assign.value = value;
  return node;
}


/*  */
static craastdesc*
push_var_undefine(XokMalloc *xm , String *name)
{return make_assign(xm , name , NULL);}


static craastdesc*
parse_factor(pavstate ps)
{
  /* tackle curr number to struct 'craastdesc*' */
  int *n = str_convert_int(craP_ps_currtoken(ps));
  ps.pf->curr++;
  ps.root = make_num(ps.xm , *n);
  /* pf's index step to next unempty token */
  accept_empty(ps.pf);
  free(n);  /* temp free n , will use memory pool later */
  return ps.root;
}


static craastdesc*
parse_mid(pavstate ps)
{
  accept_empty(ps.pf);
  /* low priority calculate sign ,tackle struct 'craastdesc*' */
  craastdesc *l = parse_factor(ps); /* is 'main node' in the funtion */
  String *cur = craP_ps_currtoken(ps);
  /* continuous tackle high priority condition */
  while(compare(cur , "*") || compare(cur , "/")){
    ps.pf->curr++;
    accept_empty(ps.pf);  /* current pf.token is calcsign */
    craastdesc *r = parse_factor(ps);
    l = make_binop(ps.xm , cur->str , l , r); /* every loop, l is 
    after refresh l and loop internal r(num) make_binop() overall root node  */
    cur = craP_ps_currtoken(ps); /* refresh current calcsign */
  }
  return l; /* external funtion need r node,
  l is has tackled all high priority node tree,
  and/or first entre the funtion's low priority node(tree) */
}


static craastdesc*
parse_assign_main(pavstate ps)
{
  accept_empty(ps.pf);
  ps.root = parse_mid(ps); /* tackle low priority or 
  continous tackle high priority,
  use ps.root to accept res , root is current all craastdesc node*/
  String *cur = craP_ps_currtoken(ps); /* drawer parse_mid() , current caculate sign */
  while (compare(cur , "+") || compare(cur , "-")){
    ps.pf->curr++;
    accept_empty(ps.pf); /* current is match right number */
    craastdesc *r = parse_mid(ps);
    ps.root = make_binop(ps.xm , cur->str , ps.root , r); 
    /* ps.root is previous ps.root and craastdesc *r binop node 
    (about parse_mid() internal high priority root accept overall node),
    and current pf.token is calcsign */
    cur = craP_ps_currtoken(ps); /* refresh current calcsign */
  }
  return ps.root; /* root is main node, 
  has collected all tackled node tree */
}


static craastdesc*
parse_assign_value(XokMalloc *xm , ParseFactor *pf)
{
  /* package init params to pavstate */
  pavstate ps = pavstate_init(xm , pf);
  ps.root = parse_assign_main(ps);
  print_ast(ps.root , 0);
  return ps.root;
}

/* 
** Crane 'int' grammar fule , parse to craastdesc tree.
** return single tree ,collect to tree arrays;
 */
static craastdesc*
int_desc(XokMalloc *xm , ParseFactor *pf)
{
  accept(pf , "int");   /* int type var */
  accept_empty(pf);   /* throught empty to variable name */
  String *name = craP_pf_currtoken(pf);
  pf->curr++;
  craastdesc* var = make_var(xm , name->str); /* variable's name */
  accept_empty(pf); /* throught empty to '=' */
  int isEqu = accept(pf , "="); /* judge '=' */
  if(!isEqu){ /* if isEqu not's true , declare is 'int a;' */
    craastdesc *undefinevar = push_var_undefine(xm , name);  /* undefine variable tackle */
    return undefinevar;
  }
  craastdesc *v = parse_assign_value(xm , pf); /* tackle after '=' value tree */
  craastdesc *desc = make_assign(xm , name->str , v); /* current 'desc' struction is:
  struct assign* = {var_name , struct binop* root} */
  return desc;
}


void
Cra_token_parse(XokMalloc *xokMalloc , TOKENSB *tokens)
{
  ParseFactor pf = {tokens , 0};
  while(pf.curr < tokens->count)
  {
   String *t = tokens->tokens[pf.curr];
   switch(t_convert_asttype(t))
   {
    case GRAM_INT:{
     LOG_C("GRAM_INT" , "entre!!");
     int_desc(xokMalloc , &pf);
     break;
    }
    case GRAM_PRINTF:{
     LOG_C("GRAM_PRINTF" , "entre!!");
     //prinft_desc();
     break;
    }
    default:{break;}
  }
   break;
  }
}
