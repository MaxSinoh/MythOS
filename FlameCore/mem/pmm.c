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

#include <mem/pmm.h>
#include <std/string.h>
#include <uefi.h>

// 计算总物理内存大小
uint64_t calculate_total_memory(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size) {
    uint64_t total = 0;
    uint64_t entries = map_size / desc_size;
    for (uint64_t i = 0; i < entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + i * desc_size);
        if (desc->Type == EfiConventionalMemory) {
            total += desc->NumberOfPages * PAGE_SIZE;
        }
    }
    return total;
}

// 查找足够大的空闲内存区域存储位图
uint64_t find_free_memory_region(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, 
                                 uint64_t desc_size, uint64_t pages_needed) {
    uint64_t entries = map_size / desc_size;
    for (uint64_t i = 0; i < entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + i * desc_size);
        if (desc->Type == EfiConventionalMemory && desc->NumberOfPages >= pages_needed) {
            return desc->PhysicalStart;
        }
    }
    return 0;  // 未找到足够大的区域
}

// 标记可用内存区域（初始化为未使用）
void mark_usable_memory(PhysicalMemoryManager* pmm, EFI_MEMORY_DESCRIPTOR* map, 
                       uint64_t map_size, uint64_t desc_size) {
    uint64_t entries = map_size / desc_size;
    for (uint64_t i = 0; i < entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + i * desc_size);
        if (desc->Type == EfiConventionalMemory) {
            uint64_t start_page = desc->PhysicalStart / PAGE_SIZE;
            uint64_t page_count = desc->NumberOfPages;
            for (uint64_t j = 0; j < page_count; j++) {
                mark_page_free(pmm, start_page + j);
            }
        }
    }
}

// 判断页是否被使用
bool is_page_used(PhysicalMemoryManager* pmm, uint64_t page_index) {
    if (!pmm || !pmm->bitmap) return 1;
    if (page_index >= pmm->total_pages) return 1; // 越界视为已使用
    
    uint64_t byte_idx = BITMAP_INDEX(page_index);
    uint64_t bit_idx = BITMAP_BIT(page_index);
    return (pmm->bitmap[byte_idx] & (1U << bit_idx)) != 0;
}

// 标记页为已使用
void mark_page_used(PhysicalMemoryManager* pmm, uint64_t page_index) {
    if (!pmm || !pmm->bitmap) return;
    if (page_index >= pmm->total_pages) return;
    
    uint64_t byte_idx = BITMAP_INDEX(page_index);
    uint64_t bit_idx = BITMAP_BIT(page_index);
    pmm->bitmap[byte_idx] |= (1U << bit_idx);
}

// 标记连续页为已使用
void mark_pages_used(PhysicalMemoryManager* pmm, uint64_t start_page, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        mark_page_used(pmm, start_page + i);
    }
}

// 标记页为空闲
void mark_page_free(PhysicalMemoryManager* pmm, uint64_t page_index) {
    if (!pmm || !pmm->bitmap) return;
    if (page_index >= pmm->total_pages) return;
    
    uint64_t byte_idx = BITMAP_INDEX(page_index);
    uint64_t bit_idx = BITMAP_BIT(page_index);
    pmm->bitmap[byte_idx] &= ~(1U << bit_idx);
}

// 分配一个物理页
uint64_t pmmAllocatePage(PhysicalMemoryManager* pmm) {
    if (!pmm || !pmm->bitmap) return 0;
    
    // 遍历位图查找空闲页
    for (uint64_t i = 0; i < pmm->total_pages; i++) {
        if (!is_page_used(pmm, i)) {
            mark_page_used(pmm, i);
            pmm->used_pages++;
            return i * PAGE_SIZE;  // 返回页的物理地址
        }
    }
    return 0;  // 内存不足，返回0表示分配失败
}

// 释放一个物理页
void pmmFreePage(PhysicalMemoryManager* pmm, uint64_t page_addr) {
    if (!pmm || !pmm->bitmap) return;
    
    uint64_t page_index = page_addr / PAGE_SIZE;
    if (page_index >= pmm->total_pages) return;  // 越界检查
    
    if (is_page_used(pmm, page_index)) {
        mark_page_free(pmm, page_index);
        if (pmm->used_pages > 0) pmm->used_pages--;
    }
}

// 初始化物理内存管理器
void initPMM(PhysicalMemoryManager* pmm, EFI_MEMORY_DESCRIPTOR* map, 
             uint64_t map_size, uint64_t desc_size) {
    if (!pmm || !map) return;
    
    // 计算总内存页数
    uint64_t total_memory = calculate_total_memory(map, map_size, desc_size);
    pmm->total_pages = total_memory / PAGE_SIZE;

    // 计算位图大小（每个页用1 bit表示，向上取整）
    uint64_t bitmap_size = pmm->total_pages / 8 + 1;
    uint64_t bitmap_pages = (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;  // 位图占用的页数

    // 查找存储位图的空闲区域
    uint64_t bitmap_addr = find_free_memory_region(map, map_size, desc_size, bitmap_pages);
    if (bitmap_addr == 0) {
        // 处理错误：没有足够内存存储位图
        return;
    }

    // 初始化位图（默认标记所有页为已使用）
    pmm->bitmap = (uint64_t*)bitmap_addr;
    memset(pmm->bitmap, 0xFF, bitmap_size);  // 全部置为1（已使用）

    // 标记可用内存区域为空闲
    mark_usable_memory(pmm, map, map_size, desc_size);

    // 标记位图自身占用的页为已使用
    uint64_t bitmap_start_page = bitmap_addr / PAGE_SIZE;
    mark_pages_used(pmm, bitmap_start_page, bitmap_pages);

    // 初始化已使用页数（位图占用的页数）
    pmm->used_pages = bitmap_pages;
}