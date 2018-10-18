#ifndef MALLOC_H_
# define MALLOC_H_

void *malloc(size_t size);

struct list
{
    int status;
    size_t size;
    void *next;
//    int listcount;
//    int size_remaining;
};


#endif /* !MALLOC_H_ */
