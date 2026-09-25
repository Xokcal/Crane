#include <stdio.h>
#include "tool/String/String.h"
#include "tool/log/log.h"
#include "cranegram/cgm_file.h"
#include "cranegram/cgm_parse.h"
#include "cranegram/cgm_gramma.h"
#include "tool/xokmalloc/xokmalloc.h"

#define ATM_MULTIPY_STRI(a , d) (a + b)
#define PI 3.14
#define ARR_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
static float ATM_FRONT_STRING_TRIANGE = 34.234;

int main() {
    String *origin_str = open_file("Crane/test.cr");
    TOKENSB *tokens = tokens_parse(origin_str);
    XokMalloc *xokMalloc =  create_XokMalloc();
    Hashmap *hashmap = create_hashmap();
    __CRVM_RUN__(xokMalloc , tokens , tokens->count , hashmap);
    free_XokMalloc(xokMalloc);
    return 0;
}
