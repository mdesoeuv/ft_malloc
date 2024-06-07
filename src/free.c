#include "../includes/ft_malloc.h"

extern mstate g_state;

void free(void *ptr) {
    ft_log_info("[ %p ] <- free()\n", ptr);
    if (ptr == NULL) {
        return;
    }

    bool res = chunk_header_validate_pointer(ptr);
    if (!res) {
        ft_log_error("[free] invalid pointer: %p\n", ptr);
        return;
    }

    chunk_header* header = payload_to_header(ptr);
    int size = chunk_header_get_size(header);
    

    if (chunk_header_get_mmapped(header)) {
        free_large(header);
    } else {
        free_coalesce_chunk(header);
    }
    ft_log_trace("[free] size: %d at address: %p\n", size - sizeof(chunk_header), ptr);
}

void    free_large(chunk_header* header) {
    heap* start = heap_get_start(header);
    heap_remove(&g_state.large, start);
    g_state.large_page_count--;
    ft_log_trace("[free] large chunk unmmaped\n");
}

void    free_chunk_insert(free_chunk_header* chunk) {
    free_chunk_header** list;
    heap* pool = (heap *)chunk_header_get_page((chunk_header*)chunk);
    if (pool->type == TINY) {
        list = &g_state.tiny_free;        
    }
    else if (pool->type == SMALL) {
        list = &g_state.small_free;
    }
    else {
        return;
    }
    chunk_header_set_allocated((chunk_header*)chunk, false);
    chunk->next = *list;
    *list = chunk;
    ft_log_trace("[free] inserted free chunk in list %d\n", pool->type);
}

void    free_chunk_remove(free_chunk_header* target) {
    
    free_chunk_header** cursor;

    heap* current_page = (heap *)chunk_header_get_page((chunk_header*)target);
    

    if (current_page->type == TINY) {
       cursor=&g_state.tiny_free;
    } else {
        cursor=&g_state.small_free;
    }
    while (*cursor != target) {
        cursor = &(*cursor)->next;
    }
    *cursor = (*cursor)->next;
    ft_log_trace("[free] removed free chunk from list %d\n", current_page->type);
}

void    free_print_list(free_chunk_header* self) {
    free_chunk_header* cursor = self;
    while (cursor != NULL) {
        ft_printf("Free chunk of size %d, at address: %p\n", chunk_header_get_size((chunk_header*)cursor), cursor);
        cursor = cursor->next;
    }
}


free_chunk_header*    free_find_size(free_chunk_header* self, size_t size, allocation_type type) {
    free_chunk_header* cursor = self;
    ft_log_trace("[malloc] searching for free chunk of size %d in list %d\n", size, type);
    while (cursor != NULL) {
        if (chunk_header_get_size((chunk_header*)cursor) >= size + sizeof(size_t)) {
            ft_log_debug("[malloc] found chunk of size %d at address: %p\n", size, cursor);
            chunk_header_alloc_update_free_pages((chunk_header*)cursor);
            chunk_header_set_allocated((chunk_header*)cursor, true);
            chunk_header_split((chunk_header*)cursor, size);
            free_chunk_remove(cursor);
            return cursor;
        }
        cursor = cursor->next;
    }
    ft_log_debug("[malloc] no chunk of size %d found, requesting new page\n", size);
    return NULL;
}


void    free_coalesce_chunk(chunk_header* chunk) {

    chunk_header* new = free_coalesce_prev_chunk(chunk);
    free_coalesce_next_chunk(new);
    if (chunk_header_free_update_free_pages(new)) {
        heap* current_page = (heap*)chunk_header_get_page(new);
        ft_log_trace("[free] page is free in list %d\n", current_page->type);
        if (heap_remove_if_extra(current_page)) {
            ft_log_debug("[free] page removed\n");
            return;
        }
    }
	free_chunk_insert((free_chunk_header*)new);
}


chunk_header*    free_coalesce_prev_chunk(chunk_header* chunk) {
    chunk_header* prev_chunk = chunk->prev;
    if (chunk_header_is_first_on_heap(chunk)) {
        ft_log_trace("[free] first chunk in page, no previous chunk, cannot coalesce\n");
        return chunk;
    }
    if (chunk_header_get_allocated(prev_chunk)) {
        ft_log_trace("[free] previous chunk is in use, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t prev_size = chunk_header_get_size(prev_chunk);
    size_t new_size = size + prev_size;
    free_chunk_remove((free_chunk_header*)prev_chunk);
    chunk_header_set_size(prev_chunk, new_size);
    chunk_header_set_allocated(prev_chunk, false);
    if (!chunk_header_is_last_on_heap(chunk)) {
        chunk_header* next = chunk_header_get_next(chunk);
        next->prev = prev_chunk;
    }
    ft_log_debug("[free] coalesced with previous chunk of size %d at address %p, new chunk size: %d\n", prev_size, prev_chunk, new_size);
    return prev_chunk;
}


chunk_header*     free_coalesce_next_chunk(chunk_header* chunk) {

    if (chunk_header_is_last_on_heap(chunk)) {
        ft_log_trace("[free] last chunk in page, no next chunk, cannot coalesce\n");
        return chunk;
    }

    chunk_header* next = chunk_header_get_next(chunk);
    if (chunk_header_get_allocated(next)) {
        ft_log_trace("[free] next chunk is allocated, cannot coalesce\n");
        return chunk;
    }
    size_t size = chunk_header_get_size(chunk);
    size_t next_size = chunk_header_get_size(next);
    size_t new_size = size + next_size;
    chunk_header_set_size(chunk, new_size);
    if (!chunk_header_is_last_on_heap(next)) {
        chunk_header* next_next = chunk_header_get_next(next);
        next_next->prev = chunk;
    }
    free_chunk_remove((free_chunk_header*)next);
    ft_log_debug("[free] coalesced with next chunk of size %d at address %p, new size: %d\n", next_size, next, new_size);
    return chunk;
}


bool    chunk_header_is_last_on_heap(chunk_header* chunk) {
    heap* current_page = (heap*)chunk_header_get_page(chunk);
    size_t page_end_addr = (size_t)current_page + current_page->size;
    size_t chunk_end_addr = (size_t)chunk + chunk_header_get_size(chunk);
    return page_end_addr == chunk_end_addr;
}

bool    chunk_header_is_first_on_heap(chunk_header* chunk) {
    heap* current_page = (heap*)chunk_header_get_page(chunk);
    return current_page->first_chunk == chunk;
}
