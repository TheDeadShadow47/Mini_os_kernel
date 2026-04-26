#pragma once
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096
void pmm_init(uint64_t mem_size, void* bitmap_addr);
void pmm_mark_used(void* addr);
void pmm_mark_free(void* addr);
void* pmm_alloc_page();
void pmm_free_page(void* ptr);
uint64_t pmm_get_free_memory();
uint64_t pmm_get_free_count();