#include "../includes/ft_malloc.h"

int LOG_LEVEL = -1;
mstate g_state = {NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0};

int ft_strcmp(const char *s1, const char *s2) {

    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}


void initialize_log_level() __attribute__((constructor));

void initialize_heaps() __attribute__((constructor));

void initialize_log_level() {
    char* log = getenv("M_LOGLEVEL");
    if (!log) {
        return ;
    }

    if (ft_strcmp(log, "TRACE") == 0) {
        LOG_LEVEL = TRACE;
        return ;
    }

    if (ft_strcmp(log, "DEBUG") == 0) {
        LOG_LEVEL = DEBUG;
        return ;
    }

    if (ft_strcmp(log, "INFO") == 0) {
        LOG_LEVEL = INFO;
        return ;
    }
}

void initialize_heaps() {
    heap* tiny = heap_get_new(TINY_PAGE_REQUEST, TINY);
    heap* small = heap_get_new(SMALL_PAGE_REQUEST, SMALL);
    if (!tiny || !small) {
        ft_log_error("[malloc] ERROR: could not initialize tiny and small heaps\n");
        return ;
    }
    ft_log_debug("[malloc] initialized tiny and small heaps\n");
}

heap* heap_get_new(size_t heap_size, allocation_type type) {

    ft_log_trace("[malloc] requesting new page of size: %d to kernel\n", heap_size);
    // Request page from kernel
    void* ptr = mmap(
        NULL,
        heap_size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    for (size_t i = 0; i < heap_size; i++) {
        ((char*)ptr)[i] = 0;
    }

    if (ptr == MAP_FAILED) {
        ft_log_error("[malloc] ERROR: memory allocation with mmap failed\n");
        return (NULL);
    }


    // Write page metadata
    heap* new_heap = (heap *)ptr;
    new_heap->size = heap_size;
    new_heap->first_chunk = heap_get_first_chunk(new_heap);
    new_heap->next = NULL;
    new_heap->type = type;

    // TODO: Check if substraction of last chunk_header size does not compromise alignment
    size_t remaining_size = heap_size - to_next_multiple(sizeof(heap), CHUNK_ALIGNMENT);
    chunk_header* first = new_heap->first_chunk;

    ft_log_trace("[malloc] first chunk size on new page: %d\n", remaining_size);
    // Write chunk metadata
    chunk_header_set_size(first, remaining_size);
    chunk_header_set_mmapped(first, false);
    chunk_header_set_prev_inuse(first, true);
    chunk_header_set_allocated(first, false);
    first->prev = NULL;

    if (type != LARGE) {
        free_chunk_insert((free_chunk_header*)first);
    }

    // Insert page in the appropriate list
    switch(type) {
        case TINY:
            heap_insert(&g_state.tiny, new_heap);
            g_state.tiny_page_count++;
            break;
        case SMALL:
            heap_insert(&g_state.small, new_heap);
            g_state.small_page_count++;
            break;
        case LARGE:
            heap_insert(&g_state.large, new_heap);
            g_state.large_page_count++;
            break;
    }

    
    return new_heap;
}

void *malloc(size_t size) {

    ft_log_trace("[malloc] requested size: %d\n", size);

    if (size == 0) {
        ft_log_debug("[malloc] size is 0, returning NULL pointer\n");
        return (NULL);
    }



    // Compute page size
    size_t chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log_trace("[malloc] computed chunk size: %d\n", chunk_size);

    // Ensure that allocation is enough for linked list of freed chunks
    if (chunk_size < sizeof(free_chunk_header)) {
        chunk_size = sizeof(free_chunk_header);
    }

    allocation_type type = chunk_get_allocation_type(chunk_size);
    
    chunk_header* chunk;

    // Determine allocation type
    switch(type) {
        case TINY:
            chunk = tiny_alloc(chunk_size);
            break;
        case SMALL:
            chunk = small_alloc(chunk_size);
            break;
        case LARGE:
            chunk = large_alloc(chunk_size);
            break;
    }
    if (!chunk) {
        ft_log_error("[malloc] ERROR: could not allocate chunk\n");
        return (NULL);
    }
    chunk_header_set_allocated(chunk, true);
    ft_log_info("[ %p ] <- malloc(%d)\n", chunk_header_get_payload(chunk), size);
    ft_log_debug("[ %p ] <- chunk(%d)\n", chunk, chunk_size);
    return chunk_header_get_payload(chunk);
}

void heap_insert(heap** self, heap* new) {
    new->next = *self;
    *self = new;
}

/* Target must be in the list otherwise undefined behavior */
void heap_remove(heap** self, heap* target) {
    heap** cursor = self;
    while (*cursor != target) {
        cursor = &(*cursor)->next;
    }
    *cursor = (*cursor)->next;
    ft_log_trace("[malloc] unmapping page of size %d at address: %p\n", target->size, target);
    if(munmap((void*)target, target->size)) {
        ft_log_error("[malloc] ERROR unmaping page\n");
    }
}

int heap_count(heap* self) {
    int count = 0;
    while (self != NULL) {
        count++;
        self = self->next;
    }
    return count;
}


chunk_header* large_alloc(size_t chunk_size) {
    ft_log_trace("[malloc] large allocation\n");
    // Add size of heap header
    chunk_size += sizeof(heap);
    size_t heap_size = heap_get_rounded_size(chunk_size);

    // Request page from kernel
    heap* new_heap = heap_get_new(heap_size, LARGE);
    if (new_heap == NULL) {
        ft_log_error("[malloc] ERROR: could not allocate large heap\n");
        return NULL;
    }

    chunk_header* chunk = new_heap->first_chunk;
    chunk_header_set_mmapped(chunk, true);
    // chunk_header_print_metadata(chunk);

    return chunk;
}

chunk_header* small_alloc(size_t chunk_size) {
    ft_log_trace("[malloc] small allocation\n");
    chunk_header* free_chunk = (chunk_header*)free_find_size(g_state.small_free, chunk_size, SMALL);
    if (free_chunk == NULL) {
        heap* new_heap = heap_get_new(SMALL_PAGE_REQUEST, SMALL);
        if (new_heap == NULL) {
            ft_log_error("[malloc] ERROR: could not allocate small heap\n");
            return NULL;
        }
        free_chunk = new_heap->first_chunk;
        chunk_header_split((chunk_header*)free_chunk, chunk_size);
        free_chunk_remove((free_chunk_header*)free_chunk);
    }
    return free_chunk;
}

chunk_header* tiny_alloc(size_t chunk_size) {
    ft_log_trace("[malloc] tiny allocation\n");
    chunk_header* free_chunk = (chunk_header*)free_find_size(g_state.tiny_free, chunk_size, TINY);
    if (free_chunk == NULL) {
        heap* new_heap = heap_get_new(TINY_PAGE_REQUEST, TINY);
        if (new_heap == NULL) {
            ft_log_error("[malloc] ERROR: could not allocate tiny heap\n");
            return NULL;
        }
        free_chunk = new_heap->first_chunk;
        chunk_header_split((chunk_header*)free_chunk, chunk_size);
        free_chunk_remove((free_chunk_header*)free_chunk);

    }
    return free_chunk;
}


void chunk_header_split(chunk_header* chunk, size_t new_size) {
    ft_log_trace("[malloc] dividing chunk at address: %p\n", chunk);
    size_t old_size = chunk_header_get_size(chunk);
    size_t diff = old_size - new_size;
    if (new_size >= old_size) {
        ft_log_error("[malloc] new size is equal or greater than old size\n");
        return;
    }
    if (diff <= sizeof(free_chunk_header)) {
        ft_log_trace("[malloc] new size is too small to split chunk\n");
        return;
    }

    // Write new chunk metadata
    chunk_header* new_chunk = (chunk_header *)((size_t)chunk + new_size);
    chunk_header_set_size(new_chunk, diff);
    chunk_header_set_mmapped(new_chunk, false);
    chunk_header_set_prev_inuse(new_chunk, true);
    chunk_header_set_allocated(new_chunk, false);
    new_chunk->prev = chunk;

   // Update the next chunk's prev pointer
    if (!chunk_header_is_last_on_heap(new_chunk)) {
        chunk_header* next = chunk_header_get_next(new_chunk);
        next->prev = new_chunk;
    }

    // Update the size of the current chunk
    chunk_header_set_size(chunk, new_size);
    
    // Insert the new chunk in the free list
    free_chunk_insert((free_chunk_header *)new_chunk);

    ft_log_debug("[malloc] resized chunk of size %d at address: %p, new chunk of size %d at address: %p\n", new_size, chunk, diff, new_chunk);
}
