// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//           /======================\\`
//           | /==================\`||;
//           | |        ||        |;||;
//           | |   ,,   ||   ,,   |;||;
//           | |   ||   ||   ||   |;||;
//           | |   ||   ||   ||   |;||;
//           | |   ||   ``   ||   |;||;
//           | |   ||        ||   |;||;
//           | \––––––––––––––––––/´||;
//           \––––––––––––––––––––––//,
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       COPYRIGHT (c) 2025 MYTHOS PROJECT
//              ALL RIGHTS RESERVED
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _PMM_H_
#define _PMM_H_

#include <type.h>
#include <uefi.h>

typedef struct {
    uint64_t total_pages;      // 总物理页数
    uint64_t used_pages;       // 已使用页数
    uint64_t* bitmap;          // 位图：每一位表示一个页是否被使用
} PhysicalMemoryManager;

void initPMM(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size);
uint64_t calculate_total_memory(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size);
uint64_t find_free_memory_region(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size, uint64_t pages_needed);
void mark_usable_memory(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size);
bool is_page_used(uint64_t page_index);
void mark_page_used(uint64_t page_index);
void mark_pages_used(uint64_t start_page, uint64_t page_count);
void mark_page_free(uint64_t page_index);
void pmmFreePage(uint64_t page_address);
uint64_t pmmAllocatePage();

#endif