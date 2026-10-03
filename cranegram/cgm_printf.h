#include <stdio.h>
#include "../tool/String/String.h"
#include "cgm_parse.h"
#include "../tool/map/hashmap_int.h"
#include "../tool/xokmalloc/xokmalloc.h"

#define NUMBER_CHAR 10
#define PRINTF_PLACEHOLDS_LEN 3

static char *printf_PlaceHolds[PRINTF_PLACEHOLDS_LEN] = {
    "%d" , "%f" , "%s"
};

static char number_char[NUMBER_CHAR] = {'1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0'};




int
CraPrintf_fule(XokMalloc *xokmalloc ,TOKENSB *t , Hashmap *hashmap , int curr);