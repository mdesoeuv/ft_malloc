#include "../includes/ft_malloc.h"
#include "../libft/libft.h"

int main(void)
{
	int a = 0;
	dummy_function();
	(void)main_arena;
	void* ptr = malloc(10);
	if (!ptr)
		ft_putstr("Malloc failed\n");
	free(ptr);
	void* ptr2 = realloc(ptr, 100);
	(void)a;
	(void)ptr2;
	return 0;
}