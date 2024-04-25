#include "../libft/libft.h"
#include "../includes/ft_malloc.h"

int dummy_function(void)
{
    ft_putstr("Dummy function!\n");
    return (0);
}

void *malloc(size_t size) {
    (void)size;
    ft_putstr("Malloc!\n");
    return (NULL);
}

void free(void *ptr) {
    (void)ptr;
    ft_putstr("Free!\n");
}

void *realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    ft_putstr("Realloc!\n");
    return (NULL);
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


    return 0;
}