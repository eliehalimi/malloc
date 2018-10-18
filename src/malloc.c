#define _GNU_SOURCE
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "malloc.h"

struct list *list = NULL;

static void *magic_move(void *addr, size_t offset)
{
    addr = (char *) addr + offset;
    return addr;
}


static void *block_map(void)
{
    long size = sysconf(_SC_PAGESIZE);
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(addr == MAP_FAILED)
        return NULL;
    list = addr;
    list->status = 1;
    list->size = size;
    list->next = NULL;
    return list;
}

static struct list *list_init(void)
{
    if(list)
        return list;
    struct list *addr = block_map();
    if(!addr)
        return NULL;
    return addr;
}

static void* list_add(struct list *list, size_t size)
{
    if(!list)
        list_init();
    struct list *l = list;
    if(l->status == 1 && !list->next)
    {
        struct list *new = magic_move(l, sizeof(struct list) + size);
        new->status = 1;
        new->size = l->size - sizeof(struct list) - size;
        l->next = new;
        l->status = 0;
        l->size = size;
        return l + sizeof(struct list);
    }
    while(l->next && l->size < size)
        l = l->next;
    if(l->size >= size)
    {
        l->size = size;
        l->status = 0;
        return l + sizeof(struct list);
    }
    else
    {
        struct list *o = NULL;
        return list_add(o, size);
    }
}

    __attribute__((visibility("default")))
void *malloc(size_t size)
{
    //aligns size to 8
    //use sysconf
    size_t umask = 0x07;
    while(umask * size != 0)
        size++;
    void *addr = list_add(list, size);
    return addr;
}

