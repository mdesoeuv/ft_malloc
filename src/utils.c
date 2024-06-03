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
        ft_log("Pointer is not aligned\n");        
    }
}

size_t chunk_header_get_size(chunk_header *self) {
    return self->word_count << 3;
}

void chunk_header_set_size(chunk_header *self, size_t size) {
    // Check size is a multiple of 8
    if ((size >> 3 << 3) != size) {
        ft_log("Size is not a multiple of 8\n");
        exit(1);
    }
    self->word_count = size / 8;
}

bool chunk_header_get_arena(chunk_header *self) {
    return self->arena;
}

void chunk_header_set_arena(chunk_header *self, bool arena) {
    self->arena = arena;
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
            free_chunk_remove(&g_state.small_free, cursor);
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

void chunk_header_print_metadata(chunk_header *self) {
    ft_log("--- Chunk metadata: ---\n");
    ft_log("- Address: %p\n", self);
    ft_log("- Size: %d\n", chunk_header_get_size(self));
    ft_log("- Arena: %d\n", chunk_header_get_arena(self));
    ft_log("- MMapped: %d\n", chunk_header_get_mmapped(self));
    ft_log("- Prev In Use: %d\n", chunk_header_get_prev_inuse(self));
    ft_log("--- End of chunk metadata ---\n");
}

allocation_type chunk_get_allocation_type(size_t size) {

    if (size < SMALL_THRESHOLD) {
        return TINY;
    }
    if (size < LARGE_THRESHOLD) {
        return SMALL;
    }
    return LARGE;
}

void* payload_to_header(void* payload) {
    return (chunk_header*)payload - 1;
}

void* page_get_first_chunk(page *self) {
    void* header_end = (void*)(self + 1);
    return align(header_end, CHUNK_ALIGNMENT);
}

void* page_get_end(page *self) {
    return (void*)((size_t)self + self->size);
}

page* page_get_start(chunk_header* first_chunk) {
    return (page*)((size_t)(first_chunk) - (size_t)first_chunk % getpagesize());
}

size_t page_get_rounded_size(size_t size) {
    
    size_t page_size = getpagesize();
    size_t page_count = size / page_size;
    if (size % page_size) {
        page_count++;
    }
    return page_count * page_size;
}

void page_print_metadata(page *self) {
    ft_log("-- Page metadata: --\n");
    ft_log("Address: %p\n", self);
    ft_log("Next: %p\n", self->next);
    ft_log("Size: %d\n", self->size);
    ft_log("First free chunk: %p\n", self->first_chunk);
    ft_log("-- End of page metadata --\n");
}

void show_alloc_mem() {
    ft_log("-- Show alloc mem! --\n");
    ft_log("TINY\n");
    page* current = g_state.tiny;
    size_t total_size = 0;
    while (current) {
        ft_printf("%p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        total_size += current->size;
        current = current->next;
    }
    ft_log("TINY Size: %d\n\n", total_size);
    ft_log("SMALL\n");
    current = g_state.small;
    total_size = 0;
    while (current) {
        ft_printf("%p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        total_size += current->size;
        current = current->next;
    }
    ft_log("SMALL Size: %d\n\n", total_size);
    ft_log("LARGE\n");
    current = g_state.large;
    total_size = 0;
    while (current) {
        ft_printf("%p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        total_size += current->size;
        current = current->next;
    }
    ft_log("LARGE Size: %d\n\n", total_size);

    ft_log("FREE\n");
    ft_log("TINY\n");
    free_print_list(g_state.tiny_free);
    ft_log("SMALL\n");
    free_print_list(g_state.small_free);

    ft_log("-- End of show alloc mem! --\n");
}

void show_chunk_status(void *ptr) {

    ft_log("--------------------\n");
    ft_log("Memory block status: \n");
    chunk_header* header = payload_to_header(ptr);
    ft_log("Size: %d\n", chunk_header_get_size(header));
    ft_log("Allocated: %d\n", chunk_header_get_arena(header));
    ft_log("Header address: %p\n", header);
    ft_log("Payload address: %p\n", ptr);
    ft_log("Next block header: %p\n", (size_t)header + chunk_header_get_size(header));
    ft_log("--------------------\n");
}
