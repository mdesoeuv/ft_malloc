# ft_malloc


## Objectives

Recode the `malloc`, `free` and `realloc` functions of the libc   
The goal is to understand how memory allocation works at a lower level and to be able to implement a custom memory allocator  


## Constraints

### Allowed Functions

- libraries :
    - libft + libft_printf (42)
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
- With performance in mind, the number of calls to `mmap()` and `munmap()` must be limited : Some memory zones must be "pre-allocated" to store “small” and “medium” malloc
- The size of these zones must be a multiple of `getpagesize()`
- Each zone must contain at least 100 allocations.
    - “TINY” mallocs, from 1 to n bytes, will be stored in N bytes big zones.
    - “SMALL” mallocs, from (n+1) to m bytes, will be stored in M bytes big zones.
    - “LARGE” mallocs, from (m+1) bytes and more, will be stored out of zone, which simply means with mmap(), they will be in a zone on their own.
- The size of n, m, N and M must be defined to find a good compromise between speed (saving on system recall) and saving memory.

- The memory given by our malloc must be aligned (on a multiple of the largest component) -> addresses must be a multiple of a power of 2 (1, 2, 4 or 8 bytes)

### Maximum Overhead

```
>>> 272 * 4096
1114112
>>> 1024 * 1024
1048576
>>> 1114112 - 1048576
65536
>>> 65536 / 1024
64.0
```

- 272 pages of 4096 bytes = 1114112 bytes authorized for 1024 * 1024 bytes of mallocs  
- Each pre allocated zone must be a multiple a page size (4096 bytes) and contain at least 100 allocations

## Description


## Algorithms

- `LARGE` allocations are handled by `mmap` and `munmap`
- `TINY` and `SMALL` allocations are handled by 
    - first searching for a free chunk in the corresponding heap, then by allocating a new chunk if no free chunk is found
    - if the heap is full, a new heap is created


### Chunk Allocation Strategy

The simplified chunk-allocation strategy for small chunks is this:     

1) If there is a previously-freed chunk of memory, and that chunk is big enough to service the request, the heap manager will use that freed chunk for the new allocation.
2) Otherwise, if there is available space at the top of the heap, the heap manager will allocate a new chunk out of that available space and use that.
3) Otherwise, the heap manager will ask the kernel to add new memory to the end of the heap, and then allocates a new chunk from this newly allocated space.
4) If all these strategies fail, the allocation can’t be serviced, and malloc returns NULL.

### Free Strategy

1) If the chunk has the M bit set in the metadata, the allocation is `LARGE` and should be munmaped.
2) Otherwise, if the chunk before this one is free, the chunk is merged backwards to create a bigger free chunk.
3) Similarly, if the chunk after this one is free, the chunk is merged forwards to create a bigger free chunk.
4) Otherwise, the chunk is marked as free and placed in an appropriate bin.


## Heaps


### Types

- `TINY` : 0 - 128 + 16 bytes
- `SMALL` : 129 + 16 - 1024 + 16 bytes
- `LARGE` : > 1024 + 16  bytes


### Heap Sizes

Each heap must contain at least 100 allocations  

- `TINY` : 4 * 4096 bytes
- `SMALL` : 27 * 4096 bytes


## Usage

### Tests
```bash
LD_PRELOAD=./libft_malloc_x86_64_Darwin.so LD_LIBRARY_PATH=$(pwd) ./test/test_exec
```

or using the wrapper script
```bash
./preload_wapper.sh test/test_exec
```

with existing binary
```bash
make && ./preload_wapper.sh cat
```

### Debug

The log level is defined with the environment variable `M_LOGLEVEL`
- `TRACE`
- `DEBUG`
- `INFO`

## Notions


- **Heap**: A contiguous region of memory that is subdivided into chunks to be allocated. Each heap belongs to exactly one arena.
- **Chunk**: A small range of memory that can be allocated (owned by the application), freed (owned by glibc), or combined with adjacent chunks into larger ranges. Note that a chunk is a wrapper around the block of memory that is given to the application. Each chunk exists in one heap and belongs to one arena.
- **Alignment**: The address of an N-bytes value must be divisible by N. For a structure it must be divisible by the largest component of the structure.

## Resources

- https://sourceware.org/glibc/wiki/MallocInternals
- https://azeria-labs.com/heap-exploitation-part-1-understanding-the-glibc-heap-implementation/
- https://my.eng.utah.edu/~cs4400/malloc.pdf


