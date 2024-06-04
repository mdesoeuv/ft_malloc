#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include "../includes/ft_malloc.h"

int LOG_LEVEL = -1;
mstate g_state = {NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0};


void initialize_log_level() __attribute__((constructor));

void initialize_log_level() {
    char* log = getenv("M_LOGLEVEL");
    if (!log) {
        return ;
    }

    if (ft_strcmp(log, "TRACE") == 0) {
        ft_printf("[malloc] log level set to TRACE\n");
        LOG_LEVEL = TRACE;
        return ;
    }

    if (ft_strcmp(log, "DEBUG") == 0) {
        ft_printf("[malloc] log level set to DEBUG\n");
        LOG_LEVEL = DEBUG;
        return ;
    }

    if (ft_strcmp(log, "INFO") == 0) {
        ft_printf("[malloc] log level set to INFO\n");
        LOG_LEVEL = INFO;
        return ;
    }
}

page* page_get_new(size_t page_size, allocation_type type) {

    ft_log_debug("[malloc] requesting new page of size: %d to kernel\n", page_size);
    // Request page from kernel
    void* ptr = mmap(
        NULL,
        page_size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (ptr == MAP_FAILED) {
        ft_log_error("[malloc] ERROR: memory allocation with mmap failed\n");
        return (NULL);
    }


    // Write page metadata
    page* new_page = (page *)ptr;
    new_page->size = page_size;
    new_page->first_chunk = page_get_first_chunk(new_page);
    new_page->next = NULL;
    new_page->type = type;

    // TODO: Check if substraction of last chunk_header size does not compromise alignment
    size_t remaining_size = page_size - to_next_multiple(sizeof(page), CHUNK_ALIGNMENT) - sizeof(chunk_header);
    chunk_header* first = new_page->first_chunk;

    // Write chunk metadata
    chunk_header_set_size(first, remaining_size);
    chunk_header_set_mmapped(first, false);
    chunk_header_set_prev_inuse(first, true);
    first->prev_size = 0;

    chunk_header* last = chunk_header_get_next(first);
    chunk_header_set_size(last, 0);
    last->prev_size = remaining_size;

    page_print_metadata(new_page);

    // Insert page in the appropriate list
    switch(type) {
        case TINY:
            page_insert(&g_state.tiny, new_page);
            g_state.tiny_page_count++;
            break;
        case SMALL:
            page_insert(&g_state.small, new_page);
            g_state.small_page_count++;
            break;
        case LARGE:
            page_insert(&g_state.large, new_page);
            g_state.large_page_count++;
            break;
    }

    
    return new_page;
}

void *malloc(size_t size) {

    ft_log_debug("[malloc] requested size: %d\n", size);

    if (size == 0) {
        ft_log_debug("[malloc] size is 0, returning NULL pointer\n");
        return (NULL);
    }


    // TODO: ensure that allocation is enough for linked list of freed chunks

    // Compute page size
    size_t chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log_debug("[malloc] computed chunk size: %d\n", chunk_size);

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
    chunk_header_print_metadata(chunk);
    ft_log_info("[ %p ] <- malloc(%d)\n", chunk_header_get_payload(chunk), size);
    ft_log_debug("[ %p ] <- chunk(%d)\n", chunk, chunk_size);
    return chunk_header_get_payload(chunk);
}

void page_insert(page** self, page* new) {
    new->next = *self;
    *self = new;
}

/* Target must be in the list otherwise undefined behavior */
void page_remove(page** self, page* target) {
    page** cursor = self;
    while (*cursor != target) {
        cursor = &(*cursor)->next;
    }
    *cursor = (*cursor)->next;
    ft_log_debug("[malloc] unmapping page of size %d at address: %p\n", target->size, target);
    if(munmap((void*)target, target->size)) {
        ft_log_error("[malloc] ERROR unmaping page\n");
    }
}

int page_count(page* self) {
    int count = 0;
    while (self != NULL) {
        count++;
        self = self->next;
    }
    return count;
}


chunk_header* large_alloc(size_t chunk_size) {
    ft_log_debug("[malloc] large allocation\n");
    size_t page_size = page_get_rounded_size(chunk_size);

    // Request page from kernel
    page* new_page = page_get_new(page_size, LARGE);

    chunk_header* chunk = new_page->first_chunk;
    chunk_header_set_mmapped(chunk, true);
    // chunk_header_print_metadata(chunk);
    new_page->first_chunk = chunk_header_get_next(chunk);

    return chunk;
}

chunk_header* small_alloc(size_t chunk_size) {
    ft_log_debug("[malloc] small allocation\n");
    chunk_header* free_chunk = (chunk_header*)free_find_size(g_state.small_free, chunk_size, SMALL);
    if (free_chunk == NULL) {
        page* new_page = page_get_new(SMALL_PAGE_REQUEST, SMALL);
        free_chunk = new_page->first_chunk;
        chunk_header_divide((chunk_header*)free_chunk, chunk_size, SMALL);
    }
    return free_chunk;
}

chunk_header* tiny_alloc(size_t chunk_size) {
    ft_log_debug("[malloc] tiny allocation\n");
    chunk_header* free_chunk = (chunk_header*)free_find_size(g_state.tiny_free, chunk_size, TINY);
    if (free_chunk == NULL) {
        size_t page_size = page_get_rounded_size(chunk_size);
        page* new_page = page_get_new(page_size, TINY);
        free_chunk = new_page->first_chunk;
        chunk_header_divide((chunk_header*)free_chunk, chunk_size, TINY);

    }
    return free_chunk;
}


void chunk_header_divide(chunk_header* chunk, size_t new_size, allocation_type type) {
    ft_log_debug("[malloc] dividing chunk at address: %p\n", chunk);
    size_t old_size = chunk_header_get_size(chunk);
    if (new_size >= old_size) {
        ft_log_error("[malloc] ERROR: New size is greater than old size\n");
        return;
    }
    size_t diff = old_size - new_size;

    // TODO: ensure that new chunk is big enough for metadata
    if (diff < CHUNK_MIN_SIZE) {
        ft_log_debug("[malloc] remaining chunk size is too small: chunk can't be divided\n");
        chunk_header_set_prev_inuse(chunk_header_get_next(chunk), true);
        return;
    }

    // Write new chunk metadata
    chunk_header* new_chunk = (chunk_header *)((size_t)chunk + new_size);
    chunk_header_set_size(new_chunk, diff);
    chunk_header_set_mmapped(new_chunk, false);
    chunk_header_set_prev_inuse(new_chunk, true);
    new_chunk->prev_size = new_size;

    // Update the size of the current chunk
    chunk_header_set_size(chunk, new_size);

    // Update prev_size of the next chunk
    chunk_header* next = chunk_header_get_next(new_chunk);
    next->prev_size = diff;
    
    // Insert the new chunk in the free list
    if (type != LARGE) {
        free_chunk_insert((free_chunk_header *)new_chunk);
    }

    ft_log_debug("[malloc] resized chunk of size %d at address: %p, new chunk of size %d at address: %p\n", new_size, chunk, diff, new_chunk);
}
