#include "../includes/ft_malloc.h"

void *realloc(void *ptr, size_t size) {
    ft_log("Realloc! Requested size: %d\n", size);
    if (!ptr) {
        ft_log("Null pointer: the address was not previously allocated.\n");
        return malloc(size);
    }
    if (size == 0) {
        ft_log("Size is 0: freeing memory\n");
        free(ptr);
        return (NULL);
    }

    // Retrieve the chunk metadata
    chunk_header* header = payload_to_header(ptr);
    size_t old_size = chunk_header_get_size(header);
    ft_log("Old size: %d\n", old_size);
    ft_log("New size: %d\n", size);

    // Compute the new size
    size_t chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log("Computed chunk size: %d\n", chunk_size);
    size_t page_size = page_get_rounded_size(chunk_size);

    page* new_page = page_get_new(page_size, LARGE);

    chunk_header* chunk = large_alloc(chunk_size);

    ft_log("Chunk metadata: \n");
    ft_log("Size: %d\n", chunk_header_get_size(chunk));
    ft_log("MMapped: %d\n", chunk_header_get_mmapped(chunk));
    ft_log("Prev In Use: %d\n", chunk_header_get_prev_inuse(chunk));

    ft_log("Allocated block size: %d\n", chunk_header_get_size(chunk));
    ft_log("Header address: %p\n", chunk);
    ft_log("Payload address: %p\n", (char *)chunk + sizeof(chunk_header));

    // Copy the data from the old block to the new block
    size_t min_size = old_size < chunk_size ? old_size : chunk_size;
    ft_log("Copying %d bytes\n", min_size - sizeof(chunk_header));
    ft_memcpy(chunk_header_get_payload(chunk), ptr, min_size - sizeof(chunk_header));
    ft_log("Data copied\n");
    free(ptr);
    return chunk_header_get_payload(chunk);
}
