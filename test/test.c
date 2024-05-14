#include "../includes/ft_malloc.h"
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"


extern int LOG_LEVEL;

int main(void)
{
	ft_printf("LOG_LEVEL: %d\n", LOG_LEVEL);
	ft_log("TEST LOG\n");
	void* ptr = malloc(12 * sizeof(char));
	if (!ptr)
		ft_putstr("Malloc failed\n");
	show_block_status(ptr);
	show_alloc_mem();
	ft_memcpy(ptr, "Hello World", 12);
	ft_printf("ptr: %s\n", ptr);
	void* ptr2 = realloc(ptr, 100 * sizeof(char));
	if (!ptr2)
		ft_putstr("Realloc failed\n");
	ft_printf("ptr2: %s\n", ptr2);
	show_block_status(ptr2);
	show_alloc_mem();
	void* ptr3 = malloc(100 * sizeof(char));
	if (!ptr3)
		ft_putstr("Malloc failed\n");
	show_alloc_mem();
	free(ptr2);
	return 0;
}