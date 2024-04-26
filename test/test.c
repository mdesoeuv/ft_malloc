#include "../includes/ft_malloc.h"

int main(void)
{
	int a = 0;
	dummy_function();
	(void)main_arena;
	void* ptr = malloc(10);
	if (!ptr)
		return 1;
	free(ptr);
	(void)a;
	return 0;
}