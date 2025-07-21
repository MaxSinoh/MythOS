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

#include <uefi.h>
#include <type.h>
#include <mem/pmm.h>
#include <std/string.h>

#define PAGE_SIZE 4096

PhysicalMemoryManager pmm;

uint64_t pmmAllocatePage() {
    // 查找空闲页
    for (uint64_t i = 0; i < pmm.total_pages; i++) {
        if (!is_page_used(i)) {
            mark_page_used(i);
            pmm.used_pages++;
            return i * PAGE_SIZE;
        }
    }
    return 0;  // 内存不足
}

void pmmFreePage(uint64_t page_address) {
    uint64_t page_index = page_address / PAGE_SIZE;
    if (is_page_used(page_index)) {
        mark_page_free(page_index);
        pmm.used_pages--;
    }
}

uint64_t calculate_total_memory(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size) {
    uint64_t total = 0;
    uint64_t entries = map_size / desc_size;
    
    for (uint64_t i = 0; i < entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint8_t*)map + i * desc_size);
        
        // 只计算可用内存类型（如EfiConventionalMemory）
        if (desc->Type == EfiConventionalMemory) {
            total += desc->NumberOfPages * 4096;  // 每页4KB
        }
    }
    
    return total;
}

void mark_usable_memory(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size) {
    uint64_t entries = map_size / desc_size;
    
    for (uint64_t i = 0; i < entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint8_t*)map + i * desc_size);
        
        // 只处理可用内存类型
        if (desc->Type == EfiConventionalMemory) {
            uint64_t start_page = desc->PhysicalStart / PAGE_SIZE;
            uint64_t page_count = desc->NumberOfPages;
            
            // 标记这些页为未使用
            for (uint64_t j = 0; j < page_count; j++) {
                mark_page_free(start_page + j);
            }
        }
    }
}

// 检查页是否被使用
bool is_page_used(uint64_t page_index) {
    uint64_t byte_index = page_index / 8;
    uint64_t bit_index = page_index % 8;
    
    return (pmm.bitmap[byte_index] & (1ULL << bit_index)) != 0;
}

// 标记单个页为已使用
void mark_page_used(uint64_t page_index) {
    uint64_t byte_index = page_index / 8;
    uint64_t bit_index = page_index % 8;
    
    pmm.bitmap[byte_index] |= (1ULL << bit_index);
}

// 标记多个连续页为已使用
void mark_pages_used(uint64_t start_page, uint64_t page_count) {
    for (uint64_t i = 0; i < page_count; i++) {
        mark_page_used(start_page + i);
    }
}

// 标记单个页为未使用
void mark_page_free(uint64_t page_index) {
    uint64_t byte_index = page_index / 8;
    uint64_t bit_index = page_index % 8;
    
    pmm.bitmap[byte_index] &= ~(1ULL << bit_index);
}

// 修改 initPMM 函数中的内存映射参数传递错误
void initPMM(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size) {
    // 计算可用物理内存总量（修复：包含所有内存类型用于总页数计算）
    uint64_t total_memory = 0;
    uint64_t entries = map_size / desc_size;
    for (uint64_t i = 0; i < entries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint8_t*)map + i * desc_size);
        total_memory += desc->NumberOfPages * PAGE_SIZE;
    }
    pmm.total_pages = total_memory / PAGE_SIZE;
    
    // 分配位图内存
    uint64_t bitmap_size = pmm.total_pages / 8 + 1;
    uint64_t bitmap_pages = (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
    
    // 找到一个合适的位置存储位图（修复：检查返回值有效性）
    uint64_t bitmap_address = find_free_memory_region(map, map_size, desc_size, bitmap_pages);
    if (bitmap_address == 0) {
        // 处理内存分配失败的情况
        while (1); // 实际应用中应添加更合理的错误处理
    }
    pmm.bitmap = (uint64_t*)bitmap_address;
    // 初始化位图（标记所有页为已使用）
    memset(pmm.bitmap, 0xFF, bitmap_size);
    // 遍历内存映射，标记可用内存为未使用
    mark_usable_memory(map, map_size, desc_size);
    // 标记位图本身占用的页为已使用
    mark_pages_used(bitmap_address / PAGE_SIZE, bitmap_pages);
    // 重新计算已使用页数
    pmm.used_pages = 0;
    for (uint64_t i = 0; i < pmm.total_pages; i++) {
        if (is_page_used(i)) {
            pmm.used_pages++;
        }
    }
}

uint64_t find_free_memory_region(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size, uint64_t pages_needed)
{
    uint64_t entries = map_size / desc_size;
    for (uint64_t i = 0; i < entries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint8_t*)map + i * desc_size);
        // 寻找足够大的可用内存区域（修复：检查内存类型并确保连续）
        if (desc->Type == EfiConventionalMemory && desc->NumberOfPages >= pages_needed)
        {
            return desc->PhysicalStart;
        }
    }
    return 0; // 未找到合适区域
}