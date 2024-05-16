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

page* get_new_page(size_t page_size) {
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
    new_page->next = NULL;

    ft_log("page metadata: \n");
    ft_log("Page Size: %d\n", new_page->size);
    ft_log("Next Page: %p\n", new_page->next);

    page_insert(&g_state.large, new_page);

    return new_page;
}

void *malloc(size_t size) {
    ft_log("Malloc! Requested size: %d\n", size);
    // TODO: return valid unique pointer
    if (size == 0) {
        ft_log("Size is 0: returning NULL\n");
        return (NULL);
    }

    // TODO: ensure that allocation is enough for linked list of freed chunks

    // Compute page size
    size_t chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log("Computed chunk size: %d\n", chunk_size);
    size_t page_size = page_get_rounded_size(chunk_size);

    // Request page from kernel
    page* new_page = get_new_page(page_size);

    // Write chunk metadata
    chunk_header* chunk = page_get_first_chunk(new_page);
    chunk_header_set_size(chunk, chunk_size);
    chunk_header_set_mmapped(chunk, true);
    // TODO: set prev_inuse to true for the first chunk
    chunk_header_set_prev_inuse(chunk, true);

    ft_log("Chunk metadata: \n");
    ft_log("Size: %d\n", chunk_header_get_size(chunk));
    ft_log("MMapped: %d\n", chunk_header_get_mmapped(chunk));
    ft_log("Prev In Use: %d\n", chunk_header_get_prev_inuse(chunk));

    // TODO: set first chunk pointer only when new page is allocated
    new_page->first_chunk = chunk;

    ft_log("Allocated block size: %d\n", chunk_header_get_size(chunk));
    ft_log("Header address: %p\n", chunk);
    ft_log("Payload address: %p\n", (char *)chunk + sizeof(chunk_header));
    
    
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
