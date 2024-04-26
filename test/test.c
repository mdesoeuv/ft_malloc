#include "../includes/ft_malloc.h"
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"

int main(void)
{
	int a = 0;
	dummy_function();
	(void)main_arena;
	void* ptr = malloc(10 * sizeof(char));
	if (!ptr)
		ft_putstr("Malloc failed\n");
	void* ptr2 = realloc(ptr, 100 * sizeof(char));
	(void)a;
	free(ptr2);
	ft_printf("Hello world\n");
	return 0;
}