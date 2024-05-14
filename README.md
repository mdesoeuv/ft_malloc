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
-  With performance in mind, the number of calls to `mmap()` and `munmap()` must be limited : Some memory zones must be "pre-allocated" to store “small” and “medium” malloc
- The size of these zones must be a multiple of `getpagesize()`
- Each zone must contain at least 100 allocations.
    - “TINY” mallocs, from 1 to n bytes, will be stored in N bytes big zones.
    - “SMALL” mallocs, from (n+1) to m bytes, will be stored in M bytes big zones.
    - “LARGE” mallocs, from (m+1) bytes and more, will be stored out of zone, which simply means with mmap(), they will be in a zone on their own.
- The size of n, m, N and M must be defined to find a good compromise between speed (saving on system recall) and saving memory.

- The memory given by our malloc must be aligned (on a multiple of the largest component) -> addresses must be a multiple of the requested size (1, 2, 4 or 8 bytes)

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

- For large (>= 512 bytes) requests, it is a pure best-fit allocator,
    with ties normally decided via FIFO (i.e. least recently used).
- For small (<= 64 bytes by default) requests, it is a caching
    allocator, that maintains pools of quickly recycled chunks.
- In between, and for combinations of large and small requests, it does
    the best it can trying to meet both goals at once.
- For very large requests (>= 128KB by default), it relies on system
    memory mapping facilities, if supported.


### Chunk Allocation Strategy

The simplified chunk-allocation strategy for small chunks is this:     

1) If there is a previously-freed chunk of memory, and that chunk is big enough to service the request, the heap manager will use that freed chunk for the new allocation.
2) Otherwise, if there is available space at the top of the heap, the heap manager will allocate a new chunk out of that available space and use that.
3) Otherwise, the heap manager will ask the kernel to add new memory to the end of the heap, and then allocates a new chunk from this newly allocated space.
4) If all these strategies fail, the allocation can’t be serviced, and malloc returns NULL.

### Free Strategy

1) If the chunk has the M bit set in the metadata, the allocation was allocated off-heap and should be munmaped.
2) Otherwise, if the chunk before this one is free, the chunk is merged backwards to create a bigger free chunk.
3) Similarly, if the chunk after this one is free, the chunk is merged forwards to create a bigger free chunk.
4) If this potentially-larger chunk borders the “top” of the heap, the whole chunk is absorbed into the end of the heap, rather than stored in a “bin”.
5) Otherwise, the chunk is marked as free and placed in an appropriate bin.


## Bins

### Types

- fast
- unsorted
- small
- large
- tcache

### Fast Bins

``` Bins for sizes < 512 bytes contain chunks of all the same size, spaced
8 bytes apart. Larger bins are approximately logarithmically spaced:

64 bins of size       8
32 bins of size      64
16 bins of size     512
8  bins of size    4096
4  bins of size   32768
2  bins of size  262144
1  bin  of size what's left
```


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

The log level is set to `DEBUG` if the environment variable `FT_MALLOC_LOG_LEVEL` is set to `1`

## Notions

- **Arena**: A structure that is shared among one or more threads which contains references to one or more heaps, as well as linked lists of chunks within those heaps which are "free". Threads assigned to each arena will allocate memory from that arena's free lists.
- **Heap**: A contiguous region of memory that is subdivided into chunks to be allocated. Each heap belongs to exactly one arena.
- **Chunk**: A small range of memory that can be allocated (owned by the application), freed (owned by glibc), or combined with adjacent chunks into larger ranges. Note that a chunk is a wrapper around the block of memory that is given to the application. Each chunk exists in one heap and belongs to one arena.
- **Memory**: A portion of the application's address space which is typically backed by RAM or swap.

- **Alignment**: The address of an N-bytes value must be divisible by N. For a structure it must be divisible by the largest component of the structure.

- minimum size of a chunk is `4*sizeof(void*)`

## Resources

- https://sourceware.org/glibc/wiki/MallocInternals
- https://azeria-labs.com/heap-exploitation-part-1-understanding-the-glibc-heap-implementation/
- https://my.eng.utah.edu/~cs4400/malloc.pdf


