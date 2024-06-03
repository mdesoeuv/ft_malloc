#include "../includes/ft_malloc.h"

extern mstate g_state;

void free(void *ptr) {
    ft_log("Free!\n");
    if (ptr == NULL) {
        ft_log("Null pointer: nothing to free\n");
        return;
    }
    ft_log("Freeing memory at address: %p\n", ptr);
    chunk_header* header = payload_to_header(ptr);

    int size = chunk_header_get_size(header);
    ft_log("Chunk Size: %d\n", size);

    if (chunk_header_get_mmapped(header)) {
        free_large(header);
    } else if (size < SMALL_THRESHOLD) {
        free_tiny(header);
    } else {
        free_small(header);
    }

    ft_log("Memory freed\n");
}

// TODO: Refactor for code duplication
void    free_tiny(chunk_header* header) {
    ft_log("Freeing tiny chunk\n");
    free_coalesce_chunk(header);
    free_chunk_insert((free_chunk_header*)header);
}


void    free_small(chunk_header* header) {
    ft_log("Freeing small chunk\n");
    free_coalesce_chunk(header);
    free_chunk_insert((free_chunk_header*)header);
}

void    free_large(chunk_header* header) {
    ft_log("Freeing large chunk\n");
    page* start = page_get_start(header);
    page_remove(&g_state.large, start);
}

void    free_chunk_insert(free_chunk_header* chunk) {
    allocation_type type = chunk_get_allocation_type(chunk_header_get_size((chunk_header*)chunk));
    free_chunk_header** list;
    
    if (type == TINY) {
        list = &g_state.tiny_free;        
    }
    else {
        list = &g_state.small_free;
    }
    
    chunk->next = *list;
    *list = chunk;
}

void    free_chunk_remove(free_chunk_header* target) {
    
    free_chunk_header** cursor;
    
    allocation_type type = chunk_get_allocation_type(chunk_header_get_size((chunk_header*)target));
    
    if (type == TINY) {
       cursor=&g_state.tiny_free;
    } else {
        cursor=&g_state.small_free;
    }
    while (*cursor != target) {
        cursor = &(*cursor)->next;
    }
    *cursor = (*cursor)->next;
}

void    free_print_list(free_chunk_header* self) {
    free_chunk_header* cursor = self;
    while (cursor != NULL) {
        ft_log("Free chunk of size %d, at address: %p\n", chunk_header_get_size((chunk_header*)cursor), cursor);
        cursor = cursor->next;
    }
    ft_log("End of free list\n");
}


free_chunk_header*    free_find_size(free_chunk_header* self, size_t size, allocation_type type) {
    free_chunk_header* cursor = self;
    ft_log("Searching for chunk of size %d in list %d\n", size, type);
    while (cursor != NULL) {
        if (chunk_header_get_size((chunk_header*)cursor) >= size + sizeof(size_t)) {
            ft_log("Found chunk of size %d at address: %p\n", size, cursor);
            chunk_header_divide((chunk_header*)cursor, size, type);
            free_chunk_remove(cursor);
            return cursor;
        }
        cursor = cursor->next;
    }
    ft_log("No chunk of size %d found, requesting new page\n", size);
    return NULL;
}


void    free_coalesce_chunk(chunk_header* chunk) {

    free_coalesce_next_chunk(chunk);
    chunk_header* new = free_coalesce_prev_chunk(chunk);
    chunk_header* next = chunk_header_get_next(new);
    chunk_header_set_prev_inuse(next, false);
    next->prev_size = chunk_header_get_size(chunk);
}


chunk_header*    free_coalesce_prev_chunk(chunk_header* chunk) {
    ft_log("Coalesce previous chunk with chunk of size %d at address %p\n", chunk_header_get_size(chunk), chunk);
    chunk_header* prev_chunk = (chunk_header*)((size_t)chunk - chunk->prev_size);
    if (chunk_header_get_prev_inuse(prev_chunk) || chunk->prev_size == 0) {
        ft_log("Previous chunk is in use or size is 0, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t new_size = size + chunk->prev_size;
    chunk_header_set_size(prev_chunk, new_size);
    // chunk_header* next = chunk_header_get_next(chunk);
    // chunk_header_set_prev_inuse(next, false);
    // next->prev_size = new_size;
    free_chunk_remove((free_chunk_header*)prev_chunk);
    ft_log("Coalesced with previous chunk of size %d at address %p\n", chunk->prev_size, prev_chunk);
    return prev_chunk;
}


chunk_header*     free_coalesce_next_chunk(chunk_header* chunk) {
    ft_log("Coalesce next chunk with chunk of size %d at address %p\n", chunk_header_get_size(chunk), chunk);
    chunk_header* next = chunk_header_get_next(chunk);
    if (!next || chunk_header_get_prev_inuse(next)) {
        ft_log("Next chunk is in use or does not exist, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t next_size = chunk_header_get_size(next);
    size_t new_size = size + next_size;
    chunk_header_set_size(chunk, new_size);
    // chunk_header* next_next = chunk_header_get_next(next);
    // chunk_header_set_prev_inuse(next_next, false);
    // next_next->prev_size = new_size;

    free_chunk_remove((free_chunk_header*)next);
    ft_log("Coalesced with next chunk of size %d at address %p\n", next_size, next);
    return chunk;
}
