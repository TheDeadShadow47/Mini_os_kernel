#include "pmm.h"
uint8_t* bitmap;
uint64_t total_pages;
uint64_t free_pages;
void pmm_init(uint64_t mem_size, void* bitmap_addr){
    bitmap=(uint8_t*)bitmap_addr;
    total_pages=mem_size/ PAGE_SIZE;
    free_pages=total_pages;
    for(uint64_t i = 0 ; i < total_pages/8;i++){
        bitmap[i]=0;
    }   
}
void pmm_mark_used(void* addr){
    uint64_t page= (uint64_t)addr/ PAGE_SIZE;
    bitmap[page/8] |=(1<<(page % 8));
    free_pages--;
}
void pmm_mark_free(void* addr){
    uint64_t page= (uint64_t)addr/ PAGE_SIZE;
    bitmap[page/8] &=~(1<<(page % 8));
    free_pages++;
}
void* pmm_alloc_page() {
    for (uint64_t i = 0; i < total_pages / 8; i++) {
        if (bitmap[i] != 0xFF) { // At least one bit is 0
            for (int j = 0; j < 8; j++) {
                if (!(bitmap[i] & (1 << j))) {
                    void* addr = (void*)((i * 8 + j) * PAGE_SIZE);
                    pmm_mark_used(addr);
                    return addr;
                }
            }
        }
    }
    return (void*)0; 
}
void pmm_free_page(void* ptr) {
    if (ptr == NULL) return;
    pmm_mark_free(ptr);
}
uint64_t pmm_get_free_count() {
    return free_pages * PAGE_SIZE;
}