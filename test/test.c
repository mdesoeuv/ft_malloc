
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include "../extra/extra.h"
#include <assert.h>

#define TINY_THRESHOLD 512
#define SMALL_THRESHOLD 1024

/* 
	Trick for the compiler to not optimize the free function
	and call it with null parameter
*/
void non_optimized_free(void* ptr) {
	free(ptr);
}

void test_free_null() {
	ft_putstr("Testing free(NULL)\n");
	non_optimized_free(NULL);
}


int main(void)
{
	print_header_sizes();
	test_free_null();

	void* ptr = malloc(12 * sizeof(char));
	if (!ptr) {
		ft_putstr("Malloc failed\n");
		return 0;
	}
	show_alloc_mem();
	ft_memcpy(ptr, "Hello World", 12);
	ft_printf("ptr: %s\n", ptr);
	void* ptr2 = malloc(100 * sizeof(char));
	// if (!ptr2) {
	// 	ft_putstr("Realloc failed\n");
	// 	return 0;
	// }
	// ft_putstr("Realloc successful\n");
	ft_memcpy(ptr2, "Hello World from pointer 2", 12);
	ft_printf("ptr2: %s\n", ptr2);
	show_alloc_mem();
	void* ptr3 = malloc(100 * sizeof(char));
	if (!ptr3) {
		ft_putstr("Malloc failed\n");
		return 0;
	}
	ft_putstr("ptr3 allocated\n");
	show_alloc_mem();
	free(ptr);
	show_alloc_mem();
	free(ptr2);
	show_alloc_mem();
	free(ptr3);
	show_alloc_mem();
	ft_putstr("ptr2 and ptr3 freed\n");
	ft_putstr("Requesting large block\n");
	void* ptr4 = malloc(7000 * sizeof(char));
	for (int i = 0; i < 7000; i++) {
		((char *)ptr4)[i] = 'A';
	}
	ft_putstr("ptr4: ");
	ft_putstr(ptr4);
	ft_putstr("\n");
	show_alloc_mem();
	free(ptr4);
	show_alloc_mem();
	void* ptr5 = malloc(600 * sizeof(char));
	for (int i = 0; i < 600; i++) {
		((char *)ptr5)[i] = 'B';
	}
	ft_putstr("ptr5: ");
	ft_putstr(ptr5);
	ft_putstr("\n");
	show_alloc_mem();
	free(ptr5);
	show_alloc_mem();

	// Realloc tests
	ft_putstr("Realloc tests\n");
	ptr5 = malloc(6 * sizeof(char));
	ft_memcpy(ptr5, "Hello", 5);
	ft_printf("ptr5: %s\n", ptr5);
	void* ptr6 = realloc(ptr5, 10 * sizeof(char));
	ft_printf("ptr6: %s\n", ptr6);
	show_alloc_mem();
	void* ptr7 = realloc(ptr6, 2 * sizeof(char));
	write(1, "ptr7: ", 6);
	write(1, ptr7, 2);
	write(1, "\n", 1);
	free(ptr7);
	show_alloc_mem();

	ft_putstr("Testing 1024 * 1024 bytes allocation\n");
	void* pointers[1024];
	for (int i = 0; i < 1024; i++) {
		pointers[i] = malloc(1024 * sizeof(char));
		if (!pointers[i]) {
			ft_putstr("Malloc failed\n");
			return 0;
		}
	}
	ft_putstr("1024 * 1024 bytes allocated\n");
	show_alloc_mem();
	for (int i = 0; i < 1024; i++) {
		ft_printf("free ptr[%d]\n", i);
		free(pointers[i]);
	}
	ft_putstr("1024 * 1024 bytes freed\n");
	show_alloc_mem();
	show_state_status();


	// Realloc tests pt2
	ft_putstr("Realloc tests pt2\n");
	char* r_ptr1 = (char*)malloc(6 * sizeof(char));
	ft_memcpy(r_ptr1, "Hello", 5);
	ft_printf("r_ptr1: %s\n", r_ptr1);
	show_alloc_mem();
	ft_putstr("Extending r_ptr1 to 13 bytes\n");
	r_ptr1 = realloc(r_ptr1, 13 * sizeof(char));
	ft_printf("r_ptr1: %s\n", r_ptr1);
	r_ptr1[5] = ' ';
	r_ptr1[6] = 'W';
	r_ptr1[7] = 'o';
	r_ptr1[8] = 'r';
	r_ptr1[9] = 'l';
	r_ptr1[10] = 'd';
	r_ptr1[11] = '!';
	r_ptr1[12] = '\0';
	ft_printf("r_ptr1: %s\n", r_ptr1);
	show_alloc_mem();
	ft_putstr("Extending r_ptr1 to 50 bytes\n");
	r_ptr1 = realloc(r_ptr1, 50 * sizeof(char));
	// fill the rest of the chunk
	for (int i = 12; i < 49; i++) {
		r_ptr1[i] = 'A';
	}
	r_ptr1[49] = '\0';
	ft_printf("r_ptr1: %s\n", r_ptr1);
	show_alloc_mem();
	ft_putstr("shrinking chunk to 10 bytes\n");
	r_ptr1 = realloc(r_ptr1, 10 * sizeof(char));
	r_ptr1[9] = '\0';
	ft_printf("r_ptr1: %s\n", r_ptr1);
	show_alloc_mem();
	free(r_ptr1);
	show_alloc_mem();

	return 0;
}
