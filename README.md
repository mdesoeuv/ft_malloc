# ft_malloc

## Allowed Functions

- libraries :
    - libft (42)
    - libpthread 

- functions :
    - mmap(2)
    - munmap(2)
    - getpagesize(2 or 3 depending on OS)
    - getrlimit(2)


## Constraints

- The lib must be named `libft_malloc_$HOSTTYPE.so`
- Makefile must be provided
    - The Makefile will have to create a symbolic link libft_malloc.so pointing to libft_malloc_$HOSTTYPE.so so for example : `libft_malloc.so -> libft_malloc_intel-mac.so`
- One global variable to manage allocations and one for the threadsafe


## Description



## Usage


## Resources

- https://sourceware.org/glibc/wiki/MallocInternals


