#include "../includes/ft_malloc.h"

void *realloc(void *ptr, size_t size) {
    ft_log_debug("Realloc! Requested size: %d\n", size);
    if (!ptr) {
        ft_log_debug("Null pointer: the address was not previously allocated.\n");
        return malloc(size);
    }
    if (size == 0) {
        ft_log_debug("Size is 0: freeing memory\n");
        free(ptr);
        return (NULL);
    }

    allocation_type type = chunk_get_allocation_type(size);

    // Retrieve the chunk metadata
    chunk_header* header = payload_to_header(ptr);
    size_t old_size = chunk_header_get_size(header);
    ft_log_debug("Old size: %d\n", old_size);
    ft_log_debug("New size: %d\n", size);

    // Compute the new size
    size_t chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log_debug("Computed chunk size: %d\n", chunk_size);



    chunk_header* chunk;
    
    switch(type) {
        case TINY:
            ft_log_debug("Tiny Allocation\n");
            chunk = tiny_alloc(chunk_size);
            break;
        case SMALL:
            ft_log_debug("Small Allocation\n");
            chunk = small_alloc(chunk_size);
            break;
        case LARGE:
            ft_log_debug("Large Allocation\n");
            chunk = large_alloc(chunk_size);
            break;
    }

    chunk_header_print_metadata(chunk);

    // Copy the data from the old block to the new block
    size_t min_size = old_size < chunk_size ? old_size : chunk_size;
    ft_log_debug("Copying %d bytes\n", min_size - sizeof(chunk_header));
    ft_memcpy(chunk_header_get_payload(chunk), ptr, min_size - sizeof(chunk_header));
    ft_log_debug("Data copied\n");
    free(ptr);
    return chunk_header_get_payload(chunk);
}
