#include "../includes/ft_malloc.h"

extern mstate g_state;

void* align(void* ptr, size_t alignment) {
    return (void*)(((size_t)ptr + alignment - 1) & ~(alignment - 1));
}


size_t to_next_multiple(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

void is_aligned(void* ptr) {
    if (((size_t)ptr & (ALLOCATION_ALIGNMENT - 1)) != 0) {
        ft_log_error("[malloc] ERROR: pointer is not aligned\n");        
    }
}

size_t chunk_header_get_size(chunk_header *self) {
    return self->word_count << 3;
}

void chunk_header_set_size(chunk_header *self, size_t size) {
    // Check size is a multiple of 8
    if ((size >> 3 << 3) != size) {
        ft_log_error("[malloc] ERROR: chunk size is not a multiple of 8\n");
        return;
    }
    self->word_count = size / 8;
}

bool chunk_header_get_allocated(chunk_header *self) {
    return self->allocated;
}

void chunk_header_set_allocated(chunk_header *self, bool allocated) {
    self->allocated = allocated;
}

bool chunk_header_get_mmapped(chunk_header *self) {
    return self->mmapped;
}

void chunk_header_set_mmapped(chunk_header *self, bool mmapped) {
    self->mmapped = mmapped;
}

bool chunk_header_get_prev_inuse(chunk_header *self) {
    return self->prev_inuse;
}

void chunk_header_set_prev_inuse(chunk_header *self, bool prev_inuse) {
    self->prev_inuse = prev_inuse;
}

void* chunk_header_get_payload(chunk_header *self) {
    void* header_end = (void*)(self + 1);
    return align(header_end, ALLOCATION_ALIGNMENT);
}

void* chunk_header_get_next(chunk_header *self) {
    return (void*)((size_t)self + chunk_header_get_size(self));
}

void* chunk_header_get_free_small(size_t chunk_size) {
    free_chunk_header* cursor = g_state.small_free;
    while (cursor != NULL) {
        if (chunk_header_get_size((chunk_header*)cursor) >= chunk_size) {
            free_chunk_remove(cursor);
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}


void*   chunk_header_get_heap(chunk_header *self) {
    chunk_header* cursor = self;
    while(!(cursor->prev == NULL)) {
        cursor = cursor->prev;
    }
    return heap_get_start(cursor);
}


bool    chunk_header_free_update_free_pages(chunk_header *self) {
    heap* current_page = (heap*)chunk_header_get_heap(self);
    ft_log_trace("[free] updating free pages: page size %d, chunk size %d\n", current_page->size, chunk_header_get_size(self));

    if (chunk_header_get_size(self) + to_next_multiple(sizeof(heap), CHUNK_ALIGNMENT) == current_page->size) {
        if (current_page->type == TINY) {
            g_state.free_tiny_page_count++;
            ft_log_trace("[free] tiny page is now free\n");
        } else if (current_page->type == SMALL) {
            g_state.free_small_page_count++;
            ft_log_trace("[free] small page is now free\n");
        }
        return true;
    }
    return false;
}


bool    chunk_header_alloc_update_free_pages(chunk_header *self) {
    heap* current_page = (heap*)chunk_header_get_heap(self);
    ft_log_trace("[malloc] updating free pages: page size %d, chunk size %d\n", current_page->size, chunk_header_get_size(self));
    if (chunk_header_get_size(self) + to_next_multiple(sizeof(heap), CHUNK_ALIGNMENT) == current_page->size) {
        if (current_page->type == TINY) {
            g_state.free_tiny_page_count--;
            ft_log_trace("[malloc] free tiny page is now used\n");
        } else if (current_page->type == SMALL) {
            g_state.free_small_page_count--;
            ft_log_trace("[malloc] free small page is now used\n");
        }
        return true;
    }
    return false;
}


bool    heap_remove_if_extra(heap* self) {

    if (self->type == TINY && g_state.free_tiny_page_count > 1 && ((float)g_state.free_tiny_page_count > ((float)g_state.tiny_page_count * FREE_PAGE_RATIO))) {
        ft_log_trace("[free] free tiny pages (%d/%d), removing extra tiny page\n", g_state.free_tiny_page_count, g_state.tiny_page_count);
        heap_remove(&g_state.tiny, self);
        g_state.free_tiny_page_count--;
        g_state.tiny_page_count--;
        return true;
    }
    else if (self->type == TINY) {
        ft_log_trace("[free] free tiny pages (%d/%d), not removing extra tiny page\n", g_state.free_tiny_page_count, g_state.tiny_page_count);
        return false;
    }

    if (self->type == SMALL && g_state.free_small_page_count > 1 && ((float)g_state.free_small_page_count > ((float)g_state.small_page_count * FREE_PAGE_RATIO))) {
        ft_log_trace("[free] free small pages (%d/%d), removing extra small page\n", g_state.free_small_page_count, g_state.small_page_count);
        heap_remove(&g_state.small, self);
        g_state.free_small_page_count--;
        g_state.small_page_count--;
        return true;
    }
    else if (self->type == SMALL) {
        ft_log_trace("[free] free small pages (%d/%d), not removing extra small page\n", g_state.free_small_page_count, g_state.small_page_count);
        return false;
    }

    return false;
}


void chunk_header_print_metadata(chunk_header *self) {
    ft_log_trace("--- Chunk metadata: ---\n");
    ft_log_trace("- Address: %p\n", self);
    ft_log_trace("- Size: %d\n", chunk_header_get_size(self));
    ft_log_trace("- Arena: %d\n", chunk_header_get_allocated(self));
    ft_log_trace("- MMapped: %d\n", chunk_header_get_mmapped(self));
    ft_log_trace("- Prev In Use: %d\n", chunk_header_get_prev_inuse(self));
    ft_log_trace("--- End of chunk metadata ---\n");
}

allocation_type chunk_get_allocation_type(size_t size) {

    if (size <= SMALL_THRESHOLD) {
        return TINY;
    }
    if (size <= LARGE_THRESHOLD) {
        return SMALL;
    }
    return LARGE;
}

//TODO: return chunk_header
void* payload_to_header(void* payload) {
    return (chunk_header*)payload - 1;
}

void* heap_get_first_chunk(heap *self) {
    void* header_end = (void*)(self + 1);
    return align(header_end, CHUNK_ALIGNMENT);
}

void* heap_get_end(heap *self) {
    return (void*)((size_t)self + self->size);
}

heap* heap_get_start(chunk_header* first_chunk) {
    return (heap*)((size_t)(first_chunk) - (size_t)first_chunk % PAGE_SIZE);
}

size_t heap_get_rounded_size(size_t size) {
    
    size_t page_size = PAGE_SIZE;
    size_t page_count = size / page_size;
    if (size % page_size) {
        page_count++;
    }
    return page_count * page_size;
}

void heap_print_metadata(heap *self) {
    char** types = (char*[]){"TINY", "SMALL", "LARGE"};
    ft_log_trace("-- Page metadata: --\n");
    ft_log_trace("Type: %s\n", types[self->type]);
    ft_log_trace("Address: %p\n", self);
    ft_log_trace("Next: %p\n", self->next);
    ft_log_trace("Size: %d\n", self->size);
    ft_log_trace("First free chunk: %p\n", self->first_chunk);
    ft_log_trace("-- End of page metadata --\n");
}

void show_alloc_mem() {

    heap* current = g_state.tiny;
    size_t total_size = 0;
    while (current) {
        ft_printf("TINY : %p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        print_chunk_in_use(current);
        total_size += current->size;
        current = current->next;
    }
    ft_printf("TINY Total Size: %d\n\n", total_size);
    current = g_state.small;
    total_size = 0;
    while (current) {
        ft_printf("SMALL : %p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        print_chunk_in_use(current);
        total_size += current->size;
        current = current->next;
    }
    ft_printf("SMALL Total Size: %d\n\n", total_size);
    current = g_state.large;
    total_size = 0;
    while (current) {
        ft_printf("LARGE : %p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        print_chunk_in_use(current);
        total_size += current->size;
        current = current->next;
    }
    ft_printf("LARGE Total Size: %d\n\n", total_size);
}

void show_chunk_status(void *ptr) {

    ft_log_debug("--------------------\n");
    ft_log_debug("Memory block status: \n");
    chunk_header* header = payload_to_header(ptr);
    ft_log_debug("Size: %d\n", chunk_header_get_size(header));
    ft_log_debug("Allocated: %d\n", chunk_header_get_allocated(header));
    ft_log_debug("Header address: %p\n", header);
    ft_log_debug("Payload address: %p\n", ptr);
    ft_log_debug("Next block header: %p\n", (size_t)header + chunk_header_get_size(header));
    ft_log_debug("--------------------\n");
}

void show_state_status() {
    ft_log_debug("State status: \n");
    ft_log_debug("TINY\n");
    ft_log_debug("Total pages: %d\n", g_state.tiny_page_count);
    ft_log_debug("Free pages: %d\n", g_state.free_tiny_page_count);
    ft_log_debug("SMALL\n");
    ft_log_debug("Total pages: %d\n", g_state.small_page_count);
    ft_log_debug("Free pages: %d\n", g_state.free_small_page_count);
    ft_log_debug("LARGE\n");
    ft_log_debug("Total pages: %d\n", g_state.large_page_count);
}

void print_header_sizes() {
    ft_log_debug("Page Header size: %d\n", sizeof(heap));
    ft_log_debug("Chunk Header size: %d\n", sizeof(chunk_header));
    ft_log_debug("Free Chunk Header size: %d\n", sizeof(free_chunk_header));
}


void print_chunk_in_use(heap* self) {
    chunk_header* cursor = self->first_chunk;
    while (cursor) {
        if (chunk_header_get_allocated(cursor)){
            ft_printf("%p - %p : %d bytes\n", cursor, (size_t)cursor + chunk_header_get_size(cursor), chunk_header_get_size(cursor));
        }
        else {
            ft_log_debug("(free) %p - %p : %d bytes\n", cursor, (size_t)cursor + chunk_header_get_size(cursor), chunk_header_get_size(cursor));
        }
        if (chunk_header_is_last_on_heap(cursor)) {
            break;
        }
        cursor = (chunk_header*)((size_t)cursor + chunk_header_get_size(cursor));

    }
}

bool chunk_header_validate_pointer(void* ptr) {
    if (ptr == NULL) {
        ft_log_error("[malloc] ERROR: pointer is NULL\n");
        return false;
    }
    ft_log_trace("[malloc/realloc/free] validating pointer: %p\n", ptr);
    chunk_header* header = payload_to_header(ptr);

    // check in LARGE pages
    ft_log_trace("[malloc/realloc/free] checking in LARGE pages\n");
    heap* current = g_state.large;
    while (current) {
        chunk_header* cursor = current->first_chunk;
        while (cursor) {
            if (cursor == header) {
                ft_log_debug("[malloc] valid pointer found in LARGE heap\n");
                return true;
            }
            if (chunk_header_is_last_on_heap(cursor)) {
                ft_log_trace("[malloc] last chunk on heap, trying next heap\n");
                break;
            }
            cursor = (chunk_header*)((size_t)cursor + chunk_header_get_size(cursor));
        }
        current = current->next;
    }
    // check in SMALL pages
    current = g_state.small;
    while (current) {
        chunk_header* cursor = current->first_chunk;
        while (cursor) {
            // ft_log_trace("[malloc/realloc/free] checking chunk %p\n", cursor);
            if (cursor == header) {
                ft_log_debug("[malloc/realloc/free] valid pointer found in SMALL heap\n");
                return true;
            }
            if (chunk_header_is_last_on_heap(cursor)) {
                ft_log_trace("[malloc/realloc/free] last chunk on heap, trying next heap\n");
                break;
            }
            cursor = (chunk_header*)((size_t)cursor + chunk_header_get_size(cursor));
        }
        current = current->next;
    }
    // check in TINY pages
    ft_log_trace("[malloc/realloc/free] checking in TINY pages\n");
    current = g_state.tiny;
    while (current) {
        chunk_header* cursor = current->first_chunk;
        while (cursor) {
            if (cursor == header) {
                ft_log_debug("[malloc] valid pointer found in TINY heap\n");
                return true;
            }
            if (chunk_header_is_last_on_heap(cursor)) {
                ft_log_trace("[malloc] last chunk on heap\n");
                break;
            }
            cursor = (chunk_header*)((size_t)cursor + chunk_header_get_size(cursor));
        }
        current = current->next;
    }
    ft_log_error("[malloc] ERROR: pointer not found in any heap\n");
    return false;
}
