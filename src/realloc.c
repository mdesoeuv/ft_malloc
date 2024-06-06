#include "../includes/ft_malloc.h"

void ft_memcpy(void* dst, void* src, size_t size) {
    char* dst_c = (char*)dst;
    char* src_c = (char*)src;
    for (size_t i = 0; i < size; i++) {
        dst_c[i] = src_c[i];
    }
}

void *realloc(void *ptr, size_t size) {
    ft_log_info("[realloc] origin: %p, requested size: %d\n", ptr, size);
    if (!ptr) {
        ft_log_debug("[realloc] null pointer: the address was not previously allocated.\n");
        return malloc(size);
    }
    if (size == 0) {
        ft_log_debug("[realloc] requested size is 0: freeing memory\n");
        free(ptr);
        return (NULL);
    }

    bool res = chunk_header_validate_pointer(ptr);
    if (!res) {
        ft_log_error("[realloc] invalid pointer: %p\n", ptr);
        return NULL;
    }


    // Retrieve the chunk metadata
    chunk_header* header = payload_to_header(ptr);
    size_t old_size = chunk_header_get_size(header);

    // Compute the new size
    size_t chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);

    ft_log_debug("[realloc] old size: %d, new requested size: %d, computed chunk size: %d\n", old_size, size, chunk_size);


    if (old_size >= chunk_size) {
        ft_log_debug("[realloc] old size is equal or larger than the new size: resizing chunk and returning the same pointer\n");
        chunk_header_divide(header, chunk_size);
        return ptr;
    }

    // Check if next chunk is free and large enough for the extra size
    size_t extra_size = chunk_size - old_size;
    ft_log_debug("[realloc] extra size needed: %d\n", extra_size);
    if (!chunk_header_is_last_on_heap(header)) {
        chunk_header* next = chunk_header_get_next(header);
        if (!chunk_header_get_allocated(next)) {
            size_t next_size = chunk_header_get_size(next);
            if (next_size >= extra_size) {
                if (!chunk_header_is_last_on_heap(next)) {
                    chunk_header* next_next = chunk_header_get_next(next);
                    next_next->prev = (chunk_header*)header;
                }
                ft_log_debug("[realloc] next chunk is free and large enough to accomodate the extra size (%d)\n", next_size);
                free_chunk_remove((free_chunk_header*)next);
                chunk_header_set_size(header, old_size + next_size);
                return ptr;
            }
        }
    }

    ft_log_debug("[realloc] chunk cannot be extended: allocating new chunk\n");
    chunk_header* chunk = payload_to_header(malloc(size));

    // Copy the data from the old block to the new block
    size_t min_size = old_size < chunk_size ? old_size : chunk_size;
    ft_log_debug("[realloc] copying %d bytes\n", min_size - sizeof(chunk_header));
    ft_memcpy(chunk_header_get_payload(chunk), ptr, min_size - sizeof(chunk_header));
    ft_log_debug("[realloc] data copied\n");
    free(ptr);
    return chunk_header_get_payload(chunk);
}