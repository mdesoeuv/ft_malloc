#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include "../includes/ft_malloc.h"


int LOG_LEVEL = 0;

void initialize_log_level() __attribute__((constructor));

void initialize_log_level() {
    char* log = getenv("FT_MALLOC_LOG_LEVEL");
    if (!log) {
        ft_printf("LOG LEVEL SET TO DEFAULT\n");
        return ;
    }
    int res = ft_atoi(log);
    if (res > 0) {
        ft_printf("LOG_LEVEL: DEBUG\n");
        LOG_LEVEL = 1;;
    }
}


void *malloc(size_t size) {
    ft_log("Malloc! Requested size: %d\n", size);
    if (size == 0) {
        ft_log("Size is 0: returning NULL\n");
        return (NULL);
    }

    void* ptr = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    int page_size = getpagesize();
    ft_log("Page size: %d bytes\n", page_size);

    if (ptr == MAP_FAILED) {
        ft_log("Error while allocating memory\n");
        return (NULL);
    }
    block_header *header = (block_header *)ptr;
    header->size = size + sizeof(block_header);
    header->allocated = 1;

    ft_log("Allocated block size: %d\n", header->size);
    ft_log("Header address: %p\n", header);
    ft_log("Payload address: %p\n", (char *)header + sizeof(block_header));
    
    return BLOCK_PAYLOAD(ptr);
}

void free(void *ptr) {
    ft_log("Free!\n");
    if (ptr == NULL) {
        return;
    }
    size_t size = BLOCK_SIZE(ptr);

    ft_log("Size: %d\n", size);
    BLOCK_ALLOCATED(ptr) = 0;
    int res = munmap(HEADER_ADDR(ptr), size);
    if (res == -1) {
        ft_log("Error while freeing memory\n");
    }
    ft_log("Memory freed\n");
}

void *realloc(void *ptr, size_t size) {
    ft_log("Realloc!\n");
    if (!ptr) {
        ft_log("Null pointer: the address was not previously allocated.\n");
        return malloc(size);
    }
    if (size == 0) {
        ft_log("Size is 0: freeing memory\n");
        free(ptr);
        return (NULL);
    }

    size_t old_size = BLOCK_SIZE(ptr);
    ft_log("Old size: %d\n", old_size);

    void* new_ptr = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (new_ptr == MAP_FAILED) {
        ft_log("Error while reallocating memory\n");
        return (NULL);
    }

    block_header* new_header = (block_header*)new_ptr;
    new_header->size = size + sizeof(block_header);
    new_header->allocated = 1;

    size_t min_size = old_size < size ? old_size : size;
    ft_memcpy(BLOCK_PAYLOAD(new_ptr), ptr, min_size);
    free(ptr);

    return BLOCK_PAYLOAD(new_ptr);
}


void show_alloc_mem() {
    ft_log("Show alloc mem!\n");
}

void show_block_status(void *ptr) {
    ft_log("Memory block status: \n");
    ft_log("Size: %d\n", BLOCK_SIZE(ptr));
    ft_log("Allocated: %d\n", BLOCK_ALLOCATED(ptr));
    ft_log("Header address: %p\n", HEADER_ADDR(ptr));
    ft_log("Payload address: %p\n", BLOCK_PAYLOAD(ptr));
    ft_log("Next block header: %p\n", NEXT_BLOCK_HEADER(ptr));
}

int is_mmapped(mchunkptr p) {
    if (p->mchunk_size & IS_MMAPPED)
        return (1);
    return (0);
}

int is_non_main_arena(mchunkptr p) {
    if (p->mchunk_size & NON_MAIN_ARENA)
        return (1);
    return (0);
}

int prev_inuse(mchunkptr p) {
    if (p->mchunk_size & PREV_INUSE)
        return (1);
    return (0);
}

int current_in_use(mchunkptr p) {
    if (p->fd->mchunk_prev_size & PREV_INUSE)
        return (1);
    return (0);
}
