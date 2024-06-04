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
void    show_chunk_status(void *ptr);

extern int LOG_LEVEL;

typedef enum e_log_level {
    INFO,
    DEBUG,
} log_level;

#define ft_log_info(format, ...) \
    do { \
        if (LOG_LEVEL >= INFO) { \
            ft_printf(format, ##__VA_ARGS__); \
        } \
    } while(0)

#define ft_log_debug(format, ...) \
    do { \
        if (LOG_LEVEL > INFO) { \
            ft_printf(format, ##__VA_ARGS__); \
        } \
    } while(0)

#define ft_log_error(format, ...) \
    do { \
        if (LOG_LEVEL >= INFO) { \
            ft_printf(format, ##__VA_ARGS__); \
        } \
    } while(0)

#define ALLOCATION_ALIGNMENT 16 // must be a power of 2
#define CHUNK_ALIGNMENT      16 // must be a power of 2
#define SMALL_THRESHOLD      512
#define LARGE_THRESHOLD      2048
#define SMALL_PAGE_REQUEST   12 * PAGE_SIZE
#define TINY_PAGE_REQUEST    13 * PAGE_SIZE
#define CHUNK_MIN_SIZE       sizeof(free_chunk_header)
#define PAGE_SIZE            4096

typedef enum e_allocation_type {
    TINY,
    SMALL,
    LARGE
} allocation_type;

/*
    Chunk header structure
    This structure is used to keep track of the memory chunks

    This structure uses bit fields, for more info:
    https://en.cppreference.com/w/cpp/language/bit_field

    ------------------------------
    |        prev_size           |
    ------------------------------
    |       size         | A M P |
    ------------------------------
    |        payload             |
    ------------------------------

*/
typedef struct s_chunk_header {
    size_t                  prev_size;
    size_t                  word_count : 8 * sizeof(size_t) - 3;
    bool                     arena : 1;
    bool                     mmapped : 1;
    bool                     prev_inuse : 1;
} chunk_header;



/*
    Free chunk header structure
    This structure is used to keep track of the free memory chunks

    ------------------------------
    |        prev_size           |
    ------------------------------
    |       size         | A M P |
    ------------------------------
    |        next ptr            |
    ------------------------------
    |        prev ptr            |
    ------------------------------
    |        payload             |
    ------------------------------

    A: arena
    M: mmapped
    P: prev_inuse

*/
typedef struct s_free_chunk_header {
    chunk_header            header;
    struct s_free_chunk_header* next;
    struct s_free_chunk_header* prev;
} free_chunk_header;


void*   align(void* ptr, size_t alignment);

size_t  to_next_multiple(size_t value, size_t alignment);

void    is_aligned(void* ptr);


size_t          chunk_header_get_size(chunk_header *self);
void            chunk_header_set_size(chunk_header *self, size_t size);
bool            chunk_header_get_arena(chunk_header *self);
void            chunk_header_set_arena(chunk_header *self, bool arena);
bool            chunk_header_get_mmapped(chunk_header *self);
void            chunk_header_set_mmapped(chunk_header *self, bool mmapped);
bool            chunk_header_get_prev_inuse(chunk_header *self);
void            chunk_header_set_prev_inuse(chunk_header *self, bool prev_inuse);
void*           chunk_header_get_payload(chunk_header *self);
void*           chunk_header_get_next(chunk_header *self);
void*           chunk_header_get_page(chunk_header *self);
void            chunk_header_print_metadata(chunk_header *self);
allocation_type chunk_get_allocation_type(size_t size);
void*           payload_to_header(void* payload);
void*           chunk_header_get_free_small(size_t chunk_size);
void            chunk_header_divide(chunk_header* chunk, size_t new_size, allocation_type type);

void                    free_chunk_insert(free_chunk_header* chunk);
void                    free_chunk_remove(free_chunk_header* target);
free_chunk_header*      free_find_size(free_chunk_header* self, size_t size, allocation_type type);
void                    free_large(chunk_header* header);
void                    free_coalesce_chunk(chunk_header* chunk);
chunk_header*           free_coalesce_prev_chunk(chunk_header* chunk);
chunk_header*           free_coalesce_next_chunk(chunk_header* chunk);
void                    free_print_list(free_chunk_header* self);


/*
    Page structure
    This structure is used to keep track of the memory pages
    allocated by the program

    ------------------------------
    |        next_page ptr        |
    ------------------------------
    |        first_chunk ptr      |
    ------------------------------
    |        page size            |
    ------------------------------
    |        allocation type      |
    ------------------------------
    |        padding (alignment)  |
    ------------------------------
    |        CHUNKS               |
    ------------------------------

    Allocation Type is an enum of TINY, SMALL, LARGE
    This is need to determine the page pool of chunk
    during free() operation

*/

typedef struct s_page {
    struct s_page*  next;
    chunk_header*   first_chunk;
    size_t          size;
    allocation_type type;
} page;


void*   page_get_first_chunk(page *self);
void*   page_get_end(page *self);
page*   page_get_new(size_t page_size, allocation_type type);
page*   page_get_start(chunk_header* first_chunk);
void    page_print_metadata(page *self);


/*
    Global state structure
    This structure is used to keep track of 
    the page lists and free chunk lists
    
    The next free chunk pointers are written directly in 
    the headers of the chunks

    Identically, the next page pointers are written directly in
    the page headers 
*/
typedef struct s_mstate {
    page*  tiny;
    page*  small;
    page*  large;
    free_chunk_header* tiny_free;
    free_chunk_header* small_free;
} mstate;

void        page_insert(page** self, page* new);
void        page_remove(page** self, page* target);
size_t      page_get_rounded_size(size_t size);

chunk_header* large_alloc(size_t chunk_size);
chunk_header* small_alloc(size_t chunk_size);
chunk_header* tiny_alloc(size_t chunk_size);


#endif
