#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <sys/mman.h> // mmap, munmap
#include <unistd.h> // getpagesize
#include <sys/resource.h> // getrlimit
#include <pthread.h> // pthread_mutex_t
#include <stdlib.h> // getenv
#include "../ft_printf/ft_printf.h"


#define ft_log(format, ...) \
    do { \
        if (LOG_LEVEL) { \
            ft_printf(format, ##__VA_ARGS__); \
        } \
    } while(0)


void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();
void    show_block_status(void *ptr);

#define MALLOC_ALIGNMENT            16
#define MIN_CHUNK_SIZE              (sizeof(struct malloc_chunk))
#define MAX_FAST_SIZE               160
#define NFASTBINS                   10
#define NBINS                       128
#define DEFAULT_MMAP_THRESHOLD      (128 * 1024)
#define DEFAULT_MMAP_THRESHOLD_MAX  (4 * 1024 * 1024 * sizeof(long)) // 32MB on 64-bit
#define HEAP_MIN_SIZE               (32 * 1024)
#define HEAP_MAX_SIZE               (1024 * 1024)

struct malloc_chunk {

  size_t      mchunk_prev_size;  /* Size of previous chunk (if free).  */
  size_t      mchunk_size;       /* Size in bytes, including overhead. */

  struct malloc_chunk* fd;         /* double links -- used only if free. */
  struct malloc_chunk* bk;

  /* Only used for large blocks: pointer to next larger size.  */
  struct malloc_chunk* fd_nextsize; /* double links -- used only if free. */
  struct malloc_chunk* bk_nextsize;
};

typedef struct malloc_chunk* mchunkptr;

struct heap_info {
    struct malloc_state* ar_ptr;
    struct heap_info* prev;
    struct heap_info* next;
    size_t size;
};

struct malloc_state
{
  pthread_mutex_t   mutex;
  mchunkptr         fastbins[NFASTBINS];
  mchunkptr         top;
  mchunkptr         bins[NBINS];

  /* Linked list */
  struct malloc_state *next;

  /* Linked list for free arenas.  Access to this field is serialized
     by free_list_lock in arena.c.  */
  struct malloc_state *next_free;
  
  /* Number of threads attached to this arena.  0 if the arena is on
     the free list.  Access to this field is serialized by
     free_list_lock in arena.c.  */
  size_t attached_threads;

  /* Memory allocated from the system in this arena.  */
  size_t system_mem;
  size_t max_system_mem;
};

typedef struct malloc_state* mstate;

#define MMAP(addr, size, prot, flags) \
    mmap((addr), (size), (prot), (flags)|MAP_ANONYMOUS|MAP_PRIVATE, -1, 0)

#define PREV_INUSE 0x1
#define IS_MMAPPED 0x2
#define NON_MAIN_ARENA 0x4

int is_mmapped(mchunkptr p);
int prev_inuse(mchunkptr p);
int is_non_main_arena(mchunkptr p);


#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))


/*
    Block header structure
*/
typedef struct {
    size_t size;
    char allocated;
} block_header;

/* 
    Macros to access block header fields
*/
#define HEADER_ADDR(ptr) ((block_header*)((char*)ptr - sizeof(block_header)))
#define BLOCK_SIZE(ptr) (HEADER_ADDR(ptr))->size
#define BLOCK_ALLOCATED(ptr) (HEADER_ADDR(ptr))->allocated
#define BLOCK_PAYLOAD(ptr) (ptr + sizeof(block_header))
#define NEXT_BLOCK_HEADER(ptr) (HEADER_ADDR(ptr) + BLOCK_SIZE(ptr) + sizeof(block_header))
#define NEXT_BLOCK_PAYLOAD(ptr) ((void*)NEXT_BLOCK_HEADER(ptr) + sizeof(block_header))

#endif