#include <stdio.h>
#include "craparse.h"
#include "../tool/xokmalloc/xokmalloc.h"
#include "cgm_parse.h"
#include "cgm_gramma.h"

static int
t_convert_asttype(String *s)
{
  if(compare(s , "int"))return GRAM_INT;
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
pavstate_init(XokMalloc *xokMalloc,ParseFactor *pf, craastdesc *root
,craastdesc *curr_node , craastdesc *temp_node,String *num_p1 
,String *num_p2,String *calcsign,String *calcsign2,int *return_index)
{
  pavstate          pavs;
  pavs.xm           = xokMalloc;
  pavs.pf           = pf;
  pavs.root         = root;
  pavs.curr_node    = curr_node;
  pavs.temp_node    = temp_node;
  pavs.num_p1       = num_p1;
  pavs.num_p2       = num_p2;
  pavs.calcsign     = calcsign;
  pavs.calcsign2    = calcsign2;
  pavs.return_index = return_index;
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
pav_init_make_num(pavstate ps , String *curr , craastdesc **root)
{ 
  if(!compare(curr , " ")){ /* init is number : int a = 6; --> 
     6  --> make_num(6); */
    copy_string(curr , ps.num_p1);
    int *num_p1_int = str_convert_int(curr);
    *root = make_num(ps.xm , *num_p1_int);
    ps.pf->curr++;
    accept_empty(ps.pf);
    free(num_p1_int);
  }
}

/* 
** parse_assign_value() reach low priority clacsign.
** collect next number : 'num1' , and next calcsign.
 */
static void
pav_lowpriority_init(pavstate ps , String *curr)
{
  copy_string(curr , ps.calcsign); /* record first calcsign */
  ps.pf->curr++;
  accept_empty(ps.pf); /* reach low priority num1 */
  *ps.return_index = ps.pf->curr;  /* record num1 place */
  copy_string(ps.pf->tokens->tokens[ps.pf->curr++] , ps.num_p1);
  accept_empty(ps.pf); /* second calcsign */
  copy_string(ps.pf->tokens->tokens[ps.pf->curr] , ps.calcsign2);
}


static void
pav_lowpriority_nextislowpriority(pavstate ps , craastdesc **root)
{
  int *num_p1_int = str_convert_int(ps.num_p1);
  craastdesc *r = make_num(ps.xm , *num_p1_int);
  *root = make_binop(ps.xm , ps.calcsign->str , *root , r);
  ps.pf->curr = *ps.return_index;  /* pf.curr is num2 place unuse, return num1 place */
  ps.pf->curr++;   /* reach next calcsign */
  accept_empty(ps.pf);
  *ps.return_index = -1;
  free(num_p1_int);
}


static void
pav_priority(pavstate ps , String *curr , craastdesc **root)
{
  copy_string(curr , ps.calcsign);
  ps.pf->curr++;
  accept_empty(ps.pf);
  copy_string(ps.pf->tokens->tokens[ps.pf->curr] , ps.num_p2);
  int *num_p2_int = str_convert_int(ps.num_p2);
  craastdesc *r = make_num(ps.xm , *num_p2_int);
  *root = make_binop(ps.xm , ps.calcsign->str , *root , r);
  ps.pf->curr++;
  accept_empty(ps.pf);
}


static void
pav_lowpriority_nextispriority(pavstate ps , craastdesc **root)
{
  ps.pf->curr++;
  accept_empty(ps.pf); //num2
  copy_string(ps.pf->tokens->tokens[ps.pf->curr] , ps.num_p2);
  int *num_p1_int = str_convert_int(ps.num_p1);
  int *num_p2_int = str_convert_int(ps.num_p2);
  craastdesc *l = make_num(ps.xm , *num_p1_int);
  craastdesc *r = make_num(ps.xm , *num_p2_int);
  ps.temp_node = make_binop(ps.xm 
  , ps.calcsign2->str , l , r);
  *root = make_binop(ps.xm 
  , ps.calcsign->str , *root , ps.temp_node);
  ps.pf->curr++;
  accept_empty(ps.pf);
  free(num_p1_int);
  free(num_p2_int);
}


static void
pav_lowpriority_main(pavstate ps , String *curr)
{
  if(compare(curr , "+") ||
    compare(curr , "-")){
    pav_lowpriority_init(ps , curr);
    if(compare(ps.calcsign2 , "*") ||
    compare(ps.calcsign2 , "/")){
    pav_lowpriority_nextispriority(ps , &ps.root);
    }else{
    pav_lowpriority_nextislowpriority(ps , &ps.root);
    /* current pf.curr is '+' or '-', can goto continue. */
    }
  }
}


// intsalary = 91 * 78 + 9 + 69 + 96 / 2;
static craastdesc*
pav_main_exe(pavstate ps)
{
  while(ps.pf->curr < ps.pf->tokens->count){
    pav_clear_string(ps.num_p1 , ps.num_p2 , ps.calcsign , ps.calcsign2);
    accept_empty(ps.pf);
    String *curr = ps.pf->tokens->tokens[ps.pf->curr];
    if(compare(curr , ";")){
    print_ast(ps.root, 0);
    printf("+++]%d\n" , ps.root->as.binop.left->as.binop.right->as.num);
    return ps.root;
   }
    LOG_C("[]" , curr->str);
    if(compare(curr , "*") || compare(curr , "/")){
    pav_priority(ps , curr , &ps.root);
    continue;
    }
    if(compare(curr , "+") || compare(curr , "-")){
    pav_lowpriority_init(ps , curr);
    if(compare(ps.calcsign2 , "*") || compare(ps.calcsign2 , "/")){
    pav_lowpriority_nextispriority(ps , &ps.root);
    continue;
    }else{
    pav_lowpriority_nextislowpriority(ps , &ps.root);
    /* current pf.curr is '+' or '-', can goto continue. */
    continue;
    }
  }
   pav_init_make_num(ps , curr , &ps.root);
  }
  return ps.root;
}




static craastdesc*
parse_assign_value(XokMalloc *xm , ParseFactor *pf)
{
  craastdesc *root = NULL;
  craastdesc *curr_node = NULL;
  craastdesc *temp_node = NULL;
  String *num_p1 = create_string("");
  String *num_p2 = create_string(""); 
  String *calcsign = create_string("");
  String *calcsign2 = create_string("");
  int return_index = -1;
  pavstate ps = pavstate_init(xm 
  , pf, root , curr_node , temp_node , num_p1 
  , num_p2 , calcsign , calcsign2 , &return_index);
  ps.root = pav_main_exe(ps);
  return root;
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
  String *name = pf->tokens->tokens[pf->curr++];
  craastdesc* var = make_var(xm , name->str); /* variable's name */
  accept_empty(pf); /* throught empty to '=' */
  int isEqu = accept(pf , "="); /* judge '=' */
  if(!isEqu){ /* if isEqu not's true , declare is 'int a;' */
    //craastdesc *undefinevardesc push_var_undefine();  /* undefine variable tackle */
    //return undefinevardesc;
  }
  craastdesc *v = parse_assign_value(xm , pf); /* tackle after '=' value tree */
  craastdesc *desc = make_assign(xm , name->str , v);
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

