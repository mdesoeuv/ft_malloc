#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include "../includes/ft_malloc.h"

// TODO: Use an enum
int LOG_LEVEL = 0;
static mstate g_state = {NULL, NULL, NULL};

void initialize_log_level() __attribute__((constructor));

void initialize_log_level() {
    char* log = getenv("FT_MALLOC_LOG_LEVEL");
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

void *malloc(size_t size) {
    ft_log("Malloc! Requested size: %d\n", size);
    // TODO: return valid unique pointer
    if (size == 0) {
        ft_log("Size is 0: returning NULL\n");
        return (NULL);
    }

    // TODO: ensure that allocation is enough for linked list of freed chunks

    // Compute page size
    int chunk_size = to_next_multiple(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log("Computed chunk size: %d\n", chunk_size);
    int page_size = get_rounded_page_size(chunk_size);
    ft_log("Computed page size: %d\n", page_size);

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
    ft_log("Allocated page size: %d\n", page_size);

    // Write page metadata
    page* new_page = (page *)ptr;
    new_page->size = size;
    new_page->next = NULL;

    ft_log("page metadata: \n");
    ft_log("Size: %d\n", new_page->size);
    ft_log("Next: %p\n", new_page->next);



    // Write chunk metadata
    chunk_header* chunk = page_get_first_chunk(new_page);
    chunk_header_set_size(chunk, size);
    chunk_header_set_mmapped(chunk, true);

    ft_log("Chunk metadata: \n");
    ft_log("Size: %d\n", chunk_header_get_size(chunk));
    ft_log("MMapped: %d\n", chunk_header_get_mmapped(chunk));


    new_page->first_chunk = chunk;

    ft_log("Allocated block size: %d\n", chunk_header_get_size(chunk));
    ft_log("Header address: %p\n", chunk);
    ft_log("Payload address: %p\n", (char *)chunk + sizeof(chunk_header));
    
    push_page_to_state(&(g_state.large), new_page);
    
    return chunk_header_get_payload(chunk);
}

void free(void *ptr) {
    ft_log("Free!\n");
    if (ptr == NULL) {
        ft_log("Null pointer: nothing to free\n");
        return;
    }
    ft_log("Freeing memory at address: %p\n", ptr);
    chunk_header* header = payload_to_header(ptr);
    if (!chunk_header_get_mmapped(header)) {
        ft_log("Chunk is already freed\n");
        return;
    }
    int size = chunk_header_get_size(header);
    ft_log("Chunk Size: %d\n", size);
    page* start = page_get_start(header);
    remove_page_from_state(&(g_state.large), start);
    int res = munmap(start, start->size);
    if (res == -1) {
        ft_log("Error while freeing memory\n");
    }
    ft_log("Memory freed\n");

}

// void *realloc(void *ptr, size_t size) {
//     ft_log("Realloc! Requested size: %d\n", size);
//     if (!ptr) {
//         ft_log("Null pointer: the address was not previously allocated.\n");
//         return malloc(size);
//     }
//     if (size == 0) {
//         ft_log("Size is 0: freeing memory\n");
//         free(ptr);
//         return (NULL);
//     }

//     size_t old_size = BLOCK_SIZE(ptr);
//     ft_log("Old size: %d\n", old_size);
//     size = ALIGN(size + sizeof(chunk_header));
//     int page_size = getpagesize();
//     int page_count = get_rounded_page_size(size, page_size);
//     ft_log("Requested aligned size: %d\n", size);
//     ft_log("Requested page count: %d\n", page_count);
//     size = page_count * page_size;
//     void* new_ptr = mmap(
//         NULL,
//         size,
//         PROT_READ | PROT_WRITE,
//         MAP_PRIVATE | MAP_ANONYMOUS,
//         -1,
//         0
//     );
//     if (new_ptr == MAP_FAILED) {
//         ft_log("Error while reallocating memory\n");
//         return (NULL);
//     }
//     chunk_header* new_header = (chunk_header*)new_ptr;
//     new_header->size = size;
//     new_header->allocated = 1;

//     size_t min_size = old_size < size ? old_size : size;
//     ft_memcpy(BLOCK_PAYLOAD(new_ptr), ptr, min_size - sizeof(chunk_header));
//     free(ptr);
//     push_page_to_state(&g_heap, new_header);
//     return BLOCK_PAYLOAD(new_ptr);
// }

void show_alloc_mem() {
    ft_log("-- Show alloc mem! --\n");
    page* current = g_state.large;
    int total_size = 0;
    while (current) {
        ft_printf("%p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        total_size += current->size;
        current = current->next;
    }
    ft_log("LARGE Size: %d\n", g_state.large->size);
    ft_log("-- End of show alloc mem! --\n");
}

void show_block_status(void *ptr) {


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

void push_page_to_state(page** state, page* new) {
    if (*state == NULL) {
        ft_log("push_page_to_state: initializing empty state\n");
        *state = new;
        return;
    }
    page* current = *state;
    while (current->next) {
        current = current->next;
    }
    current->next = new;
    return;
}

void remove_page_from_state(page** state, page* target) {

    if (state == NULL) {
        ft_log("remove_page_from_state: empty state\n");
        return;
    }
    page* current = *state;
    while (current && current->next != target) {
        current = current->next;
    }
    if (current == NULL) {
        ft_log("remove_page_from_state: chunk not found\n");
        return;
    }
    current->next = target->next;
    return;
}

// TODO: use the same int type
// TODO: look at ALIGN macro
int get_rounded_page_size(size_t size) {
    
    int page_size = getpagesize();
    int page_count = size / page_size;
    if (size % page_size) {
        page_count++;
    }
    return page_count * page_size;
}

// int is_mmapped(mchunkptr p) {
//     if (p->mchunk_size & IS_MMAPPED)
//         return (1);
//     return (0);
// }

// int is_non_main_arena(mchunkptr p) {
//     if (p->mchunk_size & NON_MAIN_ARENA)
//         return (1);
//     return (0);
// }

// int prev_inuse(mchunkptr p) {
//     if (p->mchunk_size & PREV_INUSE)
//         return (1);
//     return (0);
// }

// int current_in_use(mchunkptr p) {
//     if (p->fd->mchunk_prev_size & PREV_INUSE)
//         return (1);
//     return (0);
// }
