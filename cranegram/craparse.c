#include <stdio.h>
#include "craparse.h"
#include "../tool/xokmalloc/xokmalloc.h"
#include "cgm_parse.h"
#include "cgm_gramma.h"


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
pav_init_make_num(XokMalloc *xokMalloc , ParseFactor *pf 
, String *curr , String *num_p1 , craastdesc **root)
{ 
  if(!compare(curr , " ")){ /* init is number : int a = 6; --> 
     6  --> make_num(6); */
    copy_string(curr , num_p1);
    int *num_p1_int = str_convert_int(curr);
    *root = make_num(xokMalloc , *num_p1_int);
    pf->curr++;
    accept_empty(pf);
    free(num_p1_int);
  }
}

/* 
** parse_assign_value() reach low priority clacsign.
** collect next number : 'num1' , and next calcsign.
 */
static void
pav_lowpriority_init(ParseFactor *pf , String *curr , String *calcsign
, String *calcsign2 , String *num_p1 , int *return_index)
{
  copy_string(curr , calcsign); /* record first calcsign */
  pf->curr++;
  accept_empty(pf); /* reach low priority num1 */
  *return_index = pf->curr;  /* record num1 place */
  copy_string(pf->tokens->tokens[pf->curr++] , num_p1);
  accept_empty(pf); /* second calcsign */
  copy_string(pf->tokens->tokens[pf->curr] , calcsign2);
}


static void
pav_lowpriority_nextislowpriority(XokMalloc *xokMalloc 
, craastdesc **root ,ParseFactor *pf, String *num_p1 
, String *calcsign , String *calcsign2 , int *return_index)
{
  int *num_p1_int = str_convert_int(num_p1);
  craastdesc *right = make_num(xokMalloc , *num_p1_int);
  *root = make_binop(xokMalloc , calcsign->str , *root , right);
  pf->curr = *return_index;  /* pf.curr is num2 place unuse, return num1 place */
  pf->curr++;   /* reach next calcsign */
  accept_empty(pf);
  *return_index = -1;
  free(num_p1_int);
}


static void
pav_priority(XokMalloc *xokMalloc , ParseFactor *pf 
, craastdesc **root , String *curr , String *num_p1 
, String *num_p2 , String *calcsign)
{
  copy_string(curr , calcsign);
  pf->curr++;
  accept_empty(pf);
  copy_string(pf->tokens->tokens[pf->curr] , num_p2);
  int *num_p2_int = str_convert_int(num_p2);
  craastdesc *right = make_num(xokMalloc , *num_p2_int);
  printf("pav_priority: op=%s, root=%p\n", calcsign->str, *root);
  *root = make_binop(xokMalloc , calcsign->str , *root , right);
  printf("pav_priority: new root=%p\n", *root);
  pf->curr++;
  accept_empty(pf);
}


static void
pav_lowpriority_nextispriority(XokMalloc *xokMalloc 
, ParseFactor *pf , craastdesc **root , craastdesc *temp_node 
,  String *num_p1 , String *num_p2 , String *calcsign ,  String *calcsign2)
{
  pf->curr++;
  accept_empty(pf); //num2
  copy_string(pf->tokens->tokens[pf->curr] , num_p2);
  int *num_p1_int = str_convert_int(num_p1);
  int *num_p2_int = str_convert_int(num_p2);
  craastdesc *left = make_num(xokMalloc , *num_p1_int);
  craastdesc *right = make_num(xokMalloc , *num_p2_int);
  temp_node = make_binop(xokMalloc , calcsign2->str , left , right);
  *root = make_binop(xokMalloc , calcsign->str , *root , temp_node);
  pf->curr++;
  accept_empty(pf);
  free(num_p1_int);
  free(num_p2_int);
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


// int salary = 91 * 78 + 9 + 69 + 96 / 2;
static craastdesc*
pav_main_exe(XokMalloc *xokMalloc , craastdesc *temp_node 
,  ParseFactor *pf , craastdesc *root , String *num_p1 
, String *num_p2 , String *calcsign , String *calcsign2 , int *return_index)
{
  while(pf->curr < pf->tokens->count){
    pav_clear_string(num_p1 , num_p2 , calcsign , calcsign2);
    accept_empty(pf);
    String *curr = pf->tokens->tokens[pf->curr];
    printf("curr=%s, root=%p\n", curr->str , root);
    if(compare(curr , "*") || compare(curr , "/")){
    pav_priority(xokMalloc , pf , &root , curr 
    , num_p1 , num_p2 , calcsign);
    continue;
    }
    if(compare(curr , "+") ||
    compare(curr , "-")){
    pav_lowpriority_init(pf , curr , calcsign 
    , calcsign2 , num_p1 , return_index);
    if(compare(calcsign2 , "*") ||
    compare(calcsign2 , "/")){
    pav_lowpriority_nextispriority(xokMalloc , pf 
    , &root , temp_node , num_p1 , num_p2 , calcsign , calcsign2);
    continue;
    }
    pav_lowpriority_nextislowpriority(xokMalloc , &root , pf 
    , num_p1 , calcsign , calcsign2 , return_index);
    /* current pf.curr is '+' or '-', can goto continue. */
    continue;
  }
  if(compare(curr , ";"))
   return root;
   pav_init_make_num(xokMalloc , pf , curr , num_p1 , &root);
  }
  return root;
}


static craastdesc*
parse_assign_value(XokMalloc *xokMalloc , ParseFactor *pf)
{
  craastdesc *root = NULL;
  craastdesc *curr_node = NULL;
  craastdesc *temp_node = NULL;
  String *num_p1 = create_string("");
  String *num_p2 = create_string("");
  String *calcsign = create_string("");
  String *calcsign2 = create_string("");
  int return_index = -1;
  root = pav_main_exe(xokMalloc , temp_node , pf , root 
  , num_p1 , num_p2 , calcsign , calcsign2 , &return_index);
  return root;
}


static void
print_ast(craastdesc* node, int depth) {
  printf("print_ast called: node=%p, kind=%d\n", node, node ? node->kind : -1);
  if (node == NULL) return;
  for (int i = 0; i < depth; i++) printf("  ");
  switch (node->kind){
  case AST_NUM:{
   printf("NUM(%d)\n", node->as.num);
   break;
  }
  case AST_VAR:{
   printf("VAR(%s)\n", node->as.varname);
   break;
  }
  case AST_BINOP:{
   printf("BINOP(%s)\n", node->as.binop.calcsign);
   print_ast(node->as.binop.left, depth + 1);
   print_ast(node->as.binop.right, depth + 1);
   break;
  }
  case AST_ASSIGN:{
   printf("ASSIGN(%s)\n", node->as.assign.name);
   print_ast(node->as.assign.value, depth + 1);
   break;
  }
  }
}

/* 
** Crane 'int' grammar fule , parse to craastdesc tree.
** return single tree ,collect to tree arrays;
 */
static craastdesc*
int_desc(XokMalloc *xokmalloc , ParseFactor *pf)
{
  accept(pf , "int");   /* int type var */
  accept_empty(pf);   /* throught empty to variable name */
  String *name = pf->tokens->tokens[pf->curr++];
  craastdesc* var = make_var(xokmalloc , name->str); /* variable's name */
  LOG_C("make_var" , var->as.varname);
  accept_empty(pf); /* throught empty to '=' */
  int isEqu = accept(pf , "="); /* judge '=' */
  LOG_I("[1]" , pf->curr);
  if(!isEqu){ /* if isEqu not's true , declare is 'int a;' */
    //craastdesc *undefinevardesc push_var_undefine();  /* undefine variable tackle */
    //return undefinevardesc;
  }
  craastdesc *value = parse_assign_value(xokmalloc , pf); /* tackle after '=' value tree */
  //print_ast(value , 0); /* check parse_assign_value() func */
  LOG_C("check[1]" , value->as.binop.calcsign);
  LOG_I("check[2]" , value->as.binop.right->as.num);
  craastdesc *endvardesc = make_assign(xokmalloc , name->str , value);
  return endvardesc;
}


static int
t_convert_asttype(String *s)
{
  if(compare(s , "int"))return GRAM_INT;
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

