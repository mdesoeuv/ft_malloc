#include "../includes/ft_malloc.h"

extern mstate g_state;

void free(void *ptr) {
    ft_log_info("[ %p ] <- free()\n", ptr);
    if (ptr == NULL) {
        return;
    }

    chunk_header* header = payload_to_header(ptr);
    int size = chunk_header_get_size(header);
    

    if (chunk_header_get_mmapped(header)) {
        free_large(header);
    } else {
        free_coalesce_chunk(header);
    }
    ft_log_debug("[free] size: %d at address: %p\n", size - sizeof(chunk_header), ptr);
}

void    free_large(chunk_header* header) {
    page* start = page_get_start(header);
    page_remove(&g_state.large, start);
    g_state.large_page_count--;
    ft_log_debug("[free] large chunk unmmaped\n");
}

void    free_chunk_insert(free_chunk_header* chunk) {
    free_chunk_header** list;
    page* pool = (page *)chunk_header_get_page((chunk_header*)chunk);
    if (pool->type == TINY) {
        list = &g_state.tiny_free;        
    }
    else if (pool->type == SMALL) {
        list = &g_state.small_free;
    }
    else {
        return;
    }

    chunk->next = *list;
    *list = chunk;
    ft_log_debug("[free] inserted free chunk in list %d\n", pool->type);
}

void    free_chunk_remove(free_chunk_header* target) {
    
    free_chunk_header** cursor;

    page* current_page = (page *)chunk_header_get_page((chunk_header*)target);
    

    if (current_page->type == TINY) {
       cursor=&g_state.tiny_free;
    } else {
        cursor=&g_state.small_free;
    }
    while (*cursor != target) {
        cursor = &(*cursor)->next;
    }
    *cursor = (*cursor)->next;
    ft_log_debug("[free] removed free chunk from list %d\n", current_page->type);
}

void    free_print_list(free_chunk_header* self) {
    free_chunk_header* cursor = self;
    while (cursor != NULL) {
        ft_log_debug("Free chunk of size %d, at address: %p\n", chunk_header_get_size((chunk_header*)cursor), cursor);
        cursor = cursor->next;
    }
}


free_chunk_header*    free_find_size(free_chunk_header* self, size_t size, allocation_type type) {
    free_chunk_header* cursor = self;
    ft_log_debug("[malloc] searching for free chunk of size %d in list %d\n", size, type);
    while (cursor != NULL) {
        if (chunk_header_get_size((chunk_header*)cursor) >= size + sizeof(size_t)) {
            ft_log_debug("[malloc] found chunk of size %d at address: %p\n", size, cursor);
            chunk_header_alloc_update_free_pages((chunk_header*)cursor);
            chunk_header_divide((chunk_header*)cursor, size);
            free_chunk_remove(cursor);
            return cursor;
        }
        cursor = cursor->next;
    }
    ft_log_debug("[malloc] no chunk of size %d found, requesting new page\n", size);
    return NULL;
}


void    free_coalesce_chunk(chunk_header* chunk) {

    free_coalesce_next_chunk(chunk);
    chunk_header* new = free_coalesce_prev_chunk(chunk);
    chunk_header* next = chunk_header_get_next(new);
    chunk_header_set_prev_inuse(next, false);
    next->prev_size = chunk_header_get_size(new);
    if (chunk_header_free_update_free_pages(new)) {
        page* current_page = (page*)chunk_header_get_page(new);
        ft_log_trace("[free] page is free in list %d\n", current_page->type);
        if (page_remove_if_extra(current_page)) {
            ft_log_trace("[free] page removed\n");
            return;
        }
    }
	free_chunk_insert((free_chunk_header*)new);
}


chunk_header*    free_coalesce_prev_chunk(chunk_header* chunk) {
    chunk_header* prev_chunk = (chunk_header*)((size_t)chunk - chunk->prev_size);
    if (chunk->prev_size == 0) {
        ft_log_debug("[free] first chunk in page, no previous chunk, cannot coalesce\n");
        return chunk;
    }
    if (chunk_header_get_prev_inuse(chunk)) {
        ft_log_debug("[free] previous chunk is in use, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t prev_size = chunk_header_get_size(prev_chunk);
    size_t new_size = size + prev_size;
    free_chunk_remove((free_chunk_header*)prev_chunk);
    chunk_header_set_size(prev_chunk, new_size);
    ft_log_debug("[free] coalesced with previous chunk of size %d at address %p, new chunk size: %d\n", prev_size, prev_chunk, new_size);
    return prev_chunk;
}


chunk_header*     free_coalesce_next_chunk(chunk_header* chunk) {
    chunk_header* next = chunk_header_get_next(chunk);
    chunk_header* next_next = chunk_header_get_next(next);
    if (!next) {
        ft_log_debug("[free] no next chunk, cannot coalesce\n");
        return chunk;
    }        
        
    if (chunk_header_get_prev_inuse(next_next)) {
        ft_log_debug("[free] next chunk in use, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t next_size = chunk_header_get_size(next);
    size_t new_size = size + next_size;
    chunk_header_set_size(chunk, new_size);

    free_chunk_remove((free_chunk_header*)next);
    ft_log_debug("[free] coalesced with next chunk of size %d at address %p, new size: %d\n", next_size, next, new_size);
    return chunk;
}

