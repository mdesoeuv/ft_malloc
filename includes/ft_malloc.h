#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <sys/mman.h> // mmap, munmap
#include <unistd.h> // getpagesize
#include <sys/resource.h> // getrlimit

int     dummy_function(void);
void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

#endif