#include "../includes/ft_malloc.h"

void* align(void* ptr, size_t alignment) {
    return (void*)(((size_t)ptr + alignment - 1) & ~(alignment - 1));
}


size_t to_next_multiple(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

void is_aligned(void* ptr) {
    if (((size_t)ptr & (ALLOCATION_ALIGNMENT - 1)) != 0) {
        ft_log("Pointer is not aligned\n");        
        exit(1);
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
    self->word_count = size >> 3;
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
