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
    } else {
        free_coalesce_chunk(header);
    }

    ft_log("Memory freed\n");
}

void    free_large(chunk_header* header) {
    ft_log("Freeing large chunk\n");
    page* start = page_get_start(header);
    page_remove(&g_state.large, start);
}

void    free_chunk_insert(free_chunk_header* chunk) {
    free_chunk_header** list;
    page* pool = (page *)chunk_header_get_page((chunk_header*)chunk);
    if (pool->type == TINY) {
        list = &g_state.tiny_free;        
    }
    else {
        list = &g_state.small_free;
    }

    ft_log("Inserting free chunk in list %d\n", pool->type);
    
    chunk->next = *list;
    *list = chunk;
}

void    free_chunk_remove(free_chunk_header* target) {
    
    free_chunk_header** cursor;

    page* current_page = (page *)chunk_header_get_page((chunk_header*)target);
    
    ft_log("Removing free chunk from list %d\n", current_page->type);

    if (current_page->type == TINY) {
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
    next->prev_size = chunk_header_get_size(new);
	free_chunk_insert((free_chunk_header*)new);
}


chunk_header*    free_coalesce_prev_chunk(chunk_header* chunk) {
    ft_log("Coalesce previous chunk with chunk of size %d at address %p\n", chunk_header_get_size(chunk), chunk);
    chunk_header* prev_chunk = (chunk_header*)((size_t)chunk - chunk->prev_size);
    
    if (chunk->prev_size == 0) {
        ft_log("Previous chunk size is 0 => Start of the page: cannot coalesce\n");
        return chunk;
    }
    if (chunk_header_get_prev_inuse(chunk)) {
        ft_log("Previous chunk is in use, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t prev_size = chunk_header_get_size(prev_chunk);
    size_t new_size = size + prev_size;
    free_chunk_remove((free_chunk_header*)prev_chunk);
    chunk_header_set_size(prev_chunk, new_size);
    ft_log("Coalesced with previous chunk of size %d at address %p\n", prev_size, prev_chunk);
    ft_log("Coalesced chunk new size: %d\n", new_size);
    return prev_chunk;
}


chunk_header*     free_coalesce_next_chunk(chunk_header* chunk) {
    ft_log("Coalesce next chunk with chunk of size %d at address %p\n", chunk_header_get_size(chunk), chunk);
    chunk_header* next = chunk_header_get_next(chunk);
    chunk_header* next_next = chunk_header_get_next(next);
    if (!next) {
        ft_log("Next chunk does not exist, cannot coalesce\n");
        return chunk;
    }        
        
    if (chunk_header_get_prev_inuse(next_next)) {
        ft_log("Next chunk is in use or does not exist, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t next_size = chunk_header_get_size(next);
    size_t new_size = size + next_size;
    chunk_header_set_size(chunk, new_size);

    free_chunk_remove((free_chunk_header*)next);
    ft_log("Coalesced with next chunk of size %d at address %p\n", next_size, next);
    return chunk;
}

