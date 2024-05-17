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
    
    // ft_log("Tiny free list:\n");
    // free_print_list(g_state.tiny_free);
    
    // ft_log("Small free list:\n");
    // free_print_list(g_state.small_free);


}

// TODO: Refactor for code duplication
void    free_tiny(chunk_header* header) {
    ft_log("Freeing tiny chunk\n");
    free_chunk_insert(&g_state.tiny_free, (free_chunk_header*)header);
    chunk_header* next = chunk_header_get_next(header);
    if (next != NULL) {
        chunk_header_set_prev_inuse(next, false);
    }
}


void    free_small(chunk_header* header) {
    ft_log("Freeing small chunk\n");
    free_chunk_insert(&g_state.small_free, (free_chunk_header*)header);
    chunk_header* next = chunk_header_get_next(header);
    if (next != NULL) {
        chunk_header_set_prev_inuse(next, false);
    }
}

void    free_large(chunk_header* header) {
    ft_log("Freeing large chunk\n");
    page* start = page_get_start(header);
    page_remove(&g_state.large, start);
}

void    free_chunk_insert(free_chunk_header** self, free_chunk_header* chunk) {
    chunk->next = *self;
    *self = chunk;
}

void    free_chunk_remove(free_chunk_header** self, free_chunk_header* target) {
    free_chunk_header** cursor = self;
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


// TODO: divide this function in find and not found cases
free_chunk_header*    free_find_size(free_chunk_header* self, size_t size, allocation_type type) {
    free_chunk_header* cursor = self;
    free_chunk_header** list;
    ft_log("Searching for chunk of size %d in list %d\n", size, type);
    if (type == TINY) {
        list = &g_state.tiny_free;
    } else {
        list = &g_state.small_free;
    }
    while (cursor != NULL) {
        if (chunk_header_get_size((chunk_header*)cursor) >= size) {
            ft_log("Found chunk of size %d at address: %p\n", size, cursor);
            chunk_header_divide((chunk_header*)cursor, size, type);
            free_chunk_remove(list, cursor);
            return cursor;
        }
        cursor = cursor->next;
    }
    ft_log("No chunk of size %d found, requesting new page\n", size);
    return NULL;
}
