#include <stdio.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/map/hashmap_int.h"
#include "../tool/xokmalloc/xokmalloc.h"

static char* printf_PlaceHolds[] = {
    "%d" , "%f" , "%s"
};


int
CraPrintf_fule(XokMalloc *xokmalloc ,TOKENSB *t , Hashmap *hashmap , int curr);