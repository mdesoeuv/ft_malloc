#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print(char *s)
{
    write(1, s, strlen(s));
}

int main() {
    void* ptr = malloc(10);
    free(ptr + 5);
}