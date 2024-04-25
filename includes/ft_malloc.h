#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <sys/mman.h> // mmap, munmap
#include <unistd.h> // getpagesize
#include <sys/resource.h> // getrlimit
#include <pthread.h> // pthread_mutex_t

int     dummy_function(void);
void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

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

static struct malloc_state main_arena = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .next = NULL,
    .attached_threads = 1,
};

#endif