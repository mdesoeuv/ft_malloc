#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void    print(char *s)
{
    write(1, s, strlen(s));
}

int     main(void)
{
    int   i;
    char  *addr;
    char* ptrs[1024];

    i = 0;
    srand(0);
    while (i < 1024)
    {
        addr = (char*)malloc(1024);
        if (addr == NULL)
        {
            print("Failed to allocate memory\n");
            return (1);
        }
        for (int j = 0; j < 1024; j++) {
            char c = (char)rand();
            addr[j] = c;
        }
        ptrs[i] = addr;
        i++;
    }
    print("Check\n");
    srand(0);
    for (i = 0; i < 1024; ++i) {
        for (int j = 0; j < 1024; j++) {
            char c = (char)rand();
            if (ptrs[i][j] != c) {
                print("Corrupted\n");
                return (1);
            }
        }
    }
    i++;
    return (0);
}
