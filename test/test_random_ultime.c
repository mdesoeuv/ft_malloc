#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../extra/extra.h"

void    print(char *s)
{
    write(1, s, strlen(s));
}

int     main(void)
{
    char  *addr;
    char* ptrs[1024];

    srand(0);
    for (int i = 0; i < 1024; ++i)
    {
        show_alloc_mem();
        int category = rand() % 3;
        size_t size;
        if (category == 0)
            size = (size_t)rand() % 128;
        else if (category == 1)
            size = (size_t)rand() % 1024;
        else
            size = (size_t)rand() % 10240;
        addr = (char*)malloc(size);
        if (size != 0 && addr == NULL)
        {
            print("Failed to allocate memory\n");
            return (1);
        }
        for (int j = 0; j < size; j++) {
            addr[j] = (char)rand();
        }
        ptrs[i] = addr;
    }
    print("Check\n");
    srand(0);
    for (int i = 0; i < 1024; ++i)
    {
        int category = rand() % 3;
        size_t size;
        if (category == 0)
            size = (size_t)rand() % 128;
        else if (category == 1)
            size = (size_t)rand() % 1024;
        else
            size = (size_t)rand() % 10240;
        for (int j = 0; j < size; ++j) {
            if (ptrs[i][j] != (char)rand()) {
                print("Corrupted\n");
                return (1);
            }
        }
    }
    print("OK\n");
    return (0);
}
