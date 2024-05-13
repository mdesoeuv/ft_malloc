#include "../includes/ft_malloc.h"
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"


extern int LOG_LEVEL;

int main(void)
{
	ft_printf("LOG_LEVEL: %d\n", LOG_LEVEL);
	ft_log("TEST LOG\n");
	void* ptr = malloc(10 * sizeof(char));
	if (!ptr)
		ft_putstr("Malloc failed\n");
	show_block_status(ptr);
	void* ptr2 = realloc(ptr, 100 * sizeof(char));
	if (!ptr2)
		ft_putstr("Realloc failed\n");
	show_block_status(ptr2);
	free(ptr2);
	return 0;
}