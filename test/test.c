
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include "../includes/extended.h"


extern int LOG_LEVEL;

void    show_alloc_mem() {
	ft_putstr("show_alloc_mem mock\n");
}

void    show_block_status(void *ptr) {
	(void)ptr;
	ft_putstr("show_block_status mock\n");
}

int main(void)
{
	void* ptr = malloc(13 * sizeof(char));
	if (!ptr) {
		ft_putstr("Malloc failed\n");
		return 0;
	}
	// show_block_status(ptr);
	// show_alloc_mem();
	// ft_memcpy(ptr, "Hello World", 12);
	// ft_printf("ptr: %s\n", ptr);
	// void* ptr2 = malloc(100 * sizeof(char));
	// if (!ptr2) {
	// 	ft_putstr("Realloc failed\n");
	// 	return 0;
	// }
	// ft_putstr("Realloc successful\n");
	// ft_printf("ptr2: %s\n", ptr2);
	// show_block_status(ptr2);
	// show_alloc_mem();
	// void* ptr3 = malloc(100 * sizeof(char));
	// if (!ptr3) {
	// 	ft_putstr("Malloc failed\n");
	// 	return 0;
	// }
	// ft_putstr("ptr3 allocated\n");
	// show_alloc_mem();
	// free(ptr);
	// free(ptr2);
	// free(ptr3);
	// ft_putstr("ptr2 and ptr3 freed\n");
	// ft_putstr("Requesting large block\n");
	// void* ptr4 = malloc(7000 * sizeof(char));
	// for (int i = 0; i < 7000; i++) {
	// 	((char *)ptr4)[i] = 'A';
	// }
	// ft_putstr("ptr4: ");
	// ft_putstr(ptr4);
	// ft_putstr("\n");
	// show_alloc_mem();
	// free(ptr4);
	// show_alloc_mem();
	return 0;
}
