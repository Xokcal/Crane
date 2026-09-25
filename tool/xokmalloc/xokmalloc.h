#include <stdio.h>
#include <stdlib.h>

typedef struct XokMalloc{
    void *xok_pool;
    void *base_address;
    size_t offset;
    int count;
}XokMalloc;


XokMalloc *
create_XokMalloc();

void* 
xalloc_XokMalloc(XokMalloc *xokmalloc , size_t size);

void 
free_XokMalloc(XokMalloc *xokmalloc);