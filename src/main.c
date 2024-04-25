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

int main() {

    char *str = "Hello World!\n";
    ft_putstr(str);



    return 0;
}