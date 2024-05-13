#include <stdio.h>

typedef struct {
    size_t size;
    char allocated;
} block_header;

int main()
{
    int size_size_t = sizeof(size_t);
    int size_char = sizeof(char);
    int size_block_header = sizeof(block_header);

    printf("Size of size_t: %d\n", size_size_t);
    printf("Size of char: %d\n", size_char);
    printf("Size of block_header: %d\n", size_block_header);
    return 0;
}