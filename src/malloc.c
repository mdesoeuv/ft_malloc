#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include "../includes/ft_malloc.h"

// TODO: Use an enum
int LOG_LEVEL = 0;
static mstate g_state = {NULL, NULL, NULL};

void initialize_log_level() __attribute__((constructor));

void initialize_log_level() {
    // char* log = getenv("FT_MALLOC_LOG_LEVEL");
    char *log = "1";
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
    int chunk_size = to_next_multiple_truc_a(size + sizeof(chunk_header), ALLOCATION_ALIGNMENT);
    ft_log("Computed chunk size: %d\n", chunk_size);
    int page_size = get_rounded_page_size(chunk_size);

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


    new_page->first_chunk = chunk;

    ft_log("Allocated block size: %d\n", chunk_header_get_size(chunk));
    ft_log("Header address: %p\n", chunk);
    ft_log("Payload address: %p\n", (char *)chunk + sizeof(chunk_header));
    
    page_insert(&g_state.large, new_page);
    
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
    page_remove(&g_state.large, start);
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
//     ft_log("Requested page count: %d\n", page_count);*cursor && 
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
    ft_log("LARGE\n");
    page* current = g_state.large;
    int total_size = 0;
    while (current) {
        ft_printf("%p - %p : %d bytes\n", current, (char*)current + current->size, current->size);
        total_size += current->size;
        current = current->next;
    }
    ft_log("LARGE Size: %d\n", total_size);
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

size_t get_rounded_page_size(size_t size) {
    
    size_t page_size = getpagesize();
    size_t page_count = size / page_size;
    if (size % page_size) {
        page_count++;
    }
    return page_count * page_size;
}
