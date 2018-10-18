#define _GNU_SOURCE
#include <stddef.h>
#include <sys/mman.h>
#include "malloc.h"

struct list *list = NULL;

static void *block_map(void)
{
    void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(addr == MAP_FAILED)
        return NULL;
    return addr;
}

static struct list *list_init(void)
{
    if(list)
        return list;
    struct list *addr = block_map();
    if(!addr)
        return NULL;
    list = addr;
    list->status = 1;
    list->size = 4096;
    list->next = NULL;
    return list;
}

static void* list_add(size_t size)
{
    if(!list)
        list_init();
    struct list *l = list;
    while(l->status != 1 && l->next)
        l = l->next;
    l->size = size;
    return l + sizeof(struct list);
}

    __attribute__((visibility("default")))
void *malloc(size_t size)
{
    //aligns size to 8
    //use sysconf
    size_t umask = 0x07;
    while(umask * size != 0)
        size++;
    void * addr = list_add(size);
    return addr;
}

