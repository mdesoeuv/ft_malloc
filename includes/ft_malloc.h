#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include "../ft_printf/ft_printf.h"
#include <sys/mman.h> // mmap, munmap
#include <unistd.h> // getpagesize
#include <sys/resource.h> // getrlimit
#include <pthread.h> // pthread_mutex_t
#include <stdlib.h> // getenv
#include <stdbool.h> // bool

void    free(void *ptr);
void    *malloc(size_t size);
// void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();
void    show_block_status(void *ptr);

extern int LOG_LEVEL;

#define ft_log(format, ...) \
    do { \
        if (LOG_LEVEL) { \
            ft_printf(format, ##__VA_ARGS__); \
        } \
    } while(0)

#define ALLOCATION_ALIGNMENT 16 // must be a power of 2
#define CHUNK_ALIGNMENT      16 // must be a power of 2

// TODO: Document structure and glossary
/*
    Block header structure
    This structure uses bit fields, for more info:
    https://en.cppreference.com/w/cpp/language/bit_field
*/
typedef struct s_chunk_header {
    // TODO: document
    size_t                  prev_size;
    size_t                  word_count : sizeof(size_t) - 3;
    bool                     arena : 1;
    bool                     mmapped : 1;
    bool                     prev_inuse : 1;
} chunk_header;

inline void* align(void* ptr, size_t alignment) {
    return (void*)(((size_t)ptr + alignment - 1) & ~(alignment - 1));
}

inline int to_next_multiple(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

inline void is_aligned(void* ptr) {
    if (((size_t)ptr & (ALLOCATION_ALIGNMENT - 1)) != 0) {
        ft_log("Pointer is not aligned\n");        
        exit(1);
    }
}

inline size_t chunk_header_get_size(chunk_header *self) {
    return self->word_count << 3;
}

inline void chunk_header_set_size(chunk_header *self, size_t size) {
    // Check size is a multiple of 8
    if ((size >> 3 << 3) != size) {
        ft_log("Size is not a multiple of 8\n");
        exit(1);
    }
    self->word_count = size >> 3;
}

inline bool chunk_header_get_arena(chunk_header *self) {
    return self->arena;
}

inline void chunk_header_set_arena(chunk_header *self, bool arena) {
    self->arena = arena;
}

inline bool chunk_header_get_mmapped(chunk_header *self) {
    return self->mmapped;
}

inline void chunk_header_set_mmapped(chunk_header *self, bool mmapped) {
    self->mmapped = mmapped;
}

inline bool chunk_header_get_prev_inuse(chunk_header *self) {
    return self->prev_inuse;
}

inline void chunk_header_set_prev_inuse(chunk_header *self, bool prev_inuse) {
    self->prev_inuse = prev_inuse;
}

inline void* chunk_header_get_payload(chunk_header *self) {
    void* header_end = (void*)(self + 1);
    return align(header_end, ALLOCATION_ALIGNMENT);
}

inline void* payload_to_header(void* payload) {
    return (chunk_header*)payload - 1;
}

typedef void* page_ptr;

typedef struct s_page {
    struct s_page*  next;
    chunk_header*   first_chunk;
    size_t          size;
} page;

inline void* page_get_first_chunk(page *self) {
    void* header_end = (void*)(self + 1);
    return align(header_end, CHUNK_ALIGNMENT);
}

inline void* page_get_end(page *self) {
    return (void*)((size_t)self + self->size);
}

inline page* page_get_start(chunk_header* first_chunk) {
    return (page*)((size_t)first_chunk % getpagesize());
}

typedef struct s_mstate {
    page*  tiny;
    page*  small;
    page*  large;
} mstate;

void    page_insert(page** self, page* new);
void    page_remove(page** self, page* target);
int     get_rounded_page_size(size_t size);


// typedef struct malloc_header {
//     size_t prev_size;
//     size_t size;
// } malloc_header;

// struct malloc_chunk {

//   size_t      mchunk_prev_size;  /* Size of previous chunk (if free).  */
//   size_t      mchunk_size;       /* Size in bytes, including overhead. */

//   struct malloc_chunk* fd;         /* double links -- used only if free. */
//   struct malloc_chunk* bk;

//   /* Only used for large blocks: pointer to next larger size.  */
//   struct malloc_chunk* fd_nextsize; /* double links -- used only if free. */
//   struct malloc_chunk* bk_nextsize;
// };

// typedef struct malloc_chunk* mchunkptr;


#endif
