#include <stdio.h>
#include "../tool/String/String.h"
#include "types.h"
#include "../tool/map/hashmap_int.h"
#include "cgm_gramma.h"
#include "../tool/xokmalloc/xokmalloc.h"


#define TYPE_VARS_LEN 3
#define CALC_SIGN_LEN 5

#define CALCULATE_ADD(p1 , p2)(*(p1)) + (*(p2))
#define CALCULATE_SUB(p1 , p2)(*(p1)) - (*(p2))
#define CALCULATE_MUL(p1 , p2)(*(p1)) * (*(p2))
#define CALCULATE_DIVI(p1 , p2)(*(p1)) / (*(p2))


int CraTypeVar_Parse(XokMalloc *xokmalloc, TOKENSB *t, Hashmap *hashmap, int curr);
