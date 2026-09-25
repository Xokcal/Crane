#include <stdio.h>
#include "../tool/String/String.h"
#include "types.h"
#include "../tool/map/hashmap_int.h"
#include "cgm_gramma.h"
#include "../tool/xokmalloc/xokmalloc.h"


#define TYPE_VARS_LEN 3
#define CALC_SIGN_LEN 5

char *typeVars[TYPE_VARS_LEN] = {
    "int" , "float" , "string"
};

char *calcSigns[CALC_SIGN_LEN] = {"+" , "-" , "*" , "/" , "^"};


int CraTypeVar_Parse(XokMalloc *xokmalloc, TOKENSB *t, Hashmap *hashmap, int curr);
