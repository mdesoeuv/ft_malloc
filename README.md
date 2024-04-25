# ft_malloc


## Objectives

Recode the `malloc`, `free` and `realloc` functions of the libc   
The goal is to understand how memory allocation works at a lower level and to be able to implement a custom memory allocator  


## Constraints

### Allowed Functions

- libraries :
    - libft (42)
    - libpthread 

- functions :
    - mmap(2)
    - munmap(2)
    - getpagesize(2 or 3 depending on OS)
    - getrlimit(2)

### Mandatory

- The lib must be named `libft_malloc_$HOSTTYPE.so`
- Makefile must be provided
    - The Makefile will have to create a symbolic link libft_malloc.so pointing to libft_malloc_$HOSTTYPE.so so for example : `libft_malloc.so -> libft_malloc_intel-mac.so`
- One global variable to manage allocations and one for the threadsafe




## Description



## Usage


## Notions

- **Arena**: A structure that is shared among one or more threads which contains references to one or more heaps, as well as linked lists of chunks within those heaps which are "free". Threads assigned to each arena will allocate memory from that arena's free lists.
- **Heap**: A contiguous region of memory that is subdivided into chunks to be allocated. Each heap belongs to exactly one arena.
- **Chunk**: A small range of memory that can be allocated (owned by the application), freed (owned by glibc), or combined with adjacent chunks into larger ranges. Note that a chunk is a wrapper around the block of memory that is given to the application. Each chunk exists in one heap and belongs to one arena.
- **Memory**: A portion of the application's address space which is typically backed by RAM or swap.


- minimum size of a chunk is `4*sizeof(void*)`

## Resources

- https://sourceware.org/glibc/wiki/MallocInternals


