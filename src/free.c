#include "../includes/ft_malloc.h"

extern mstate g_state;

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
