#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include "../ft_printf/ft_printf.h"
#include "../libft/libft.h"
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
#define SMALL_THRESHOLD      1008
#define LARGE_THRESHOLD      1024 * 1024

typedef enum e_allocation_type {
    TINY,
    SMALL,
    LARGE
} allocation_type;

// TODO: Document structure and glossary
/*
    Block header structure
    This structure uses bit fields, for more info:
    https://en.cppreference.com/w/cpp/language/bit_field
*/
typedef struct s_chunk_header {
    // TODO: document
    size_t                  prev_size;
    size_t                  word_count : 8 * sizeof(size_t) - 3;
    bool                     arena : 1;
    bool                     mmapped : 1;
    bool                     prev_inuse : 1;
} chunk_header;

void* align(void* ptr, size_t alignment);

size_t to_next_multiple(size_t value, size_t alignment);

void is_aligned(void* ptr);

size_t chunk_header_get_size(chunk_header *self);

void chunk_header_set_size(chunk_header *self, size_t size);

bool chunk_header_get_arena(chunk_header *self);

void chunk_header_set_arena(chunk_header *self, bool arena);

bool chunk_header_get_mmapped(chunk_header *self);

void chunk_header_set_mmapped(chunk_header *self, bool mmapped);

bool chunk_header_get_prev_inuse(chunk_header *self);

void chunk_header_set_prev_inuse(chunk_header *self, bool prev_inuse);

void* chunk_header_get_payload(chunk_header *self);

void* payload_to_header(void* payload);

typedef void* page_ptr;

typedef struct s_page {
    struct s_page*  next;
    chunk_header*   first_free;
    size_t          size;
} page;


void* page_get_first_chunk(page *self);
void* page_get_end(page *self);
page* page_get_new(size_t page_size, allocation_type type);
page* page_get_start(chunk_header* first_chunk);

typedef struct s_mstate {
    page*  tiny;
    page*  small;
    page*  large;
    chunk_header* tiny_bin;
    chunk_header* small_bin;
} mstate;

void        page_insert(page** self, page* new);
void        page_remove(page** self, page* target);
size_t      page_get_rounded_size(size_t size);

chunk_header* large_alloc(size_t chunk_size);

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
