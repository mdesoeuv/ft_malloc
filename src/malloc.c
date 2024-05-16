#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include "../includes/ft_malloc.h"

// TODO: Use an enum
int LOG_LEVEL = 0;
mstate g_state = {NULL, NULL, NULL};


void initialize_log_level() __attribute__((constructor));

void initialize_log_level() {
    char* log = getenv("FT_MALLOC_LOG_LEVEL");
    // char *log = "1";
    if (!log) {
        ft_printf("LOG LEVEL SET TO DEFAULT\n");
        return ;
    }
    int res = ft_atoi(log);
    if (res > 0) {
        ft_printf("LOG_LEVEL: DEBUG\n");
        LOG_LEVEL = 1;
    }
}

page* page_get_new(size_t page_size, allocation_type type) {
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
        ft_log("Error while allocating memory\n");
        return (NULL);
    }

    // Write page metadata
    page* new_page = (page *)ptr;
    new_page->size = page_size;
    new_page->first_free = page_get_first_chunk(new_page);
    new_page->next = NULL;

    size_t remaining_size = page_size - to_next_multiple(sizeof(page), CHUNK_ALIGNMENT);
    ft_log("Remaining size: %d\n", remaining_size);
    chunk_header* first = new_page->first_free;
    chunk_header_set_size(first, remaining_size);
    chunk_header_set_mmapped(first, true);
    chunk_header_set_prev_inuse(first, true);

    page_print_metadata(new_page);
    switch(type) {
        case TINY:
            // TODO: page_insert(&g_state.tiny, new_page);
            page_insert(&g_state.large, new_page);
            break;
        case SMALL:
            page_insert(&g_state.small, new_page);
            break;
        case LARGE:
            page_insert(&g_state.large, new_page);
            break;
    }

    return new_page;
}

void *malloc(size_t size) {

    ft_log("Malloc! Requested size: %d\n", size);

    if (size == 0) {
        ft_log("Size is 0: returning NULL\n");
        return (NULL);
    }

    allocation_type type;

    if (size < SMALL_THRESHOLD) {
        ft_log("Tiny Allocation\n");
        type = TINY;
    }
    else if (size < LARGE_THRESHOLD) {
        ft_log("Small Allocation\n");
        type = SMALL;
    } else {
        ft_log("Large Allocation\n");
        type = LARGE;
    }

    // TODO: ensure that allocation is enough for linked list of freed chunks

    // Compute page size
    size_t chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log("Computed chunk size: %d\n", chunk_size);

    chunk_header* chunk;

    // Determine allocation type
    switch(type) {
        case TINY:
            ft_log("Tiny Allocation\n");
            chunk = large_alloc(chunk_size);
            break;
        case SMALL:
            ft_log("Small Allocation\n");
            chunk = large_alloc(chunk_size);
            break;
        case LARGE:
            ft_log("Large Allocation\n");
            chunk = large_alloc(chunk_size);
            break;
    }

    chunk_header_print_metadata(chunk);
    
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
    ft_log("unmapping page of size %d at address: %p\n", target->size, target);
    if(munmap((void*)target, target->size)) {
        ft_log("Error while unmaping page\n");
    }
}


chunk_header* large_alloc(size_t chunk_size) {
    
    size_t page_size = page_get_rounded_size(chunk_size);

    // Request page from kernel
    page* new_page = page_get_new(page_size, LARGE);

    chunk_header* chunk = new_page->first_free;

    chunk_header_print_metadata(chunk);

    new_page->first_free = chunk_header_get_next(chunk);

    return chunk;
}

chunk_header* small_alloc(size_t chunk_size) {
    page* current = g_state.small;

    if (!current || current->size < chunk_size) {
        ft_log("No small page available, requesting new page to Kernel\n");
        current = page_get_new(page_get_rounded_size(chunk_size), SMALL);
    }
    chunk_header* chunk = page_get_first_chunk(current);
}