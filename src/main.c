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


int dummy_function(void)
{
    ft_putstr("Dummy function!\n");
    return (0);
}

void *malloc(size_t size) {
    ft_putstr("Malloc!\n");

    void* ptr = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    void* size_addr = ptr + sizeof(void*);
    *(size_t*)size_addr = size;

    ft_printf("Size: %d\n", size);
    ft_printf("Ptr: %p\n", ptr);

    if (ptr == MAP_FAILED) {
        ft_putstr("Error while allocating memory\n");
        return (NULL);
    }

    
    return ptr;
}

void free(void *ptr) {
    ft_putstr("Free!\n");
    if (ptr == NULL) {
        ft_putstr("Error: Null pointer\n");
        return;
    }
    size_t *size;
    size = ptr + sizeof(void*);
    ft_printf("Size: %d\n", *size);

    int res = munmap(ptr, *size);
    if (res == -1) {
        ft_putstr("Error while freeing memory\n");
    }
}

void *realloc(void *ptr, size_t size) {
    ft_putstr("Realloc!\n");
    if (!ptr) {
        return malloc(size);
    }

    size_t *old_size;
    old_size = ptr + sizeof(void*);
    ft_printf("Old size: %d\n", *old_size);

    void* new_ptr = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (new_ptr == MAP_FAILED) {
        ft_putstr("Error while reallocating memory\n");
        return (NULL);
    }

    void* new_size_addr = new_ptr + sizeof(void*);
    *(size_t*)new_size_addr = size;

    free(ptr);

    return new_ptr;
}


void show_alloc_mem() {
    ft_putstr("Show alloc mem!\n");
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

int main() {

    char *str = "Hello World!\n";
    ft_putstr(str);
    (void)main_arena;

    (void)main_arena;
    return 0;
}