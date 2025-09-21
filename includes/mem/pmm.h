#ifndef _PMM_H_
#define _PMM_H_

#include <type.h>
#include <uefi.h>

#define PAGE_SIZE 4096  // 页大小定义为4KB

// 物理内存管理器结构体
typedef struct {
    uint64_t total_pages;      // 总物理页数
    uint64_t used_pages;       // 已使用页数
    uint64_t* bitmap;          // 页分配位图（每 bit 表示一页是否被使用）
} PhysicalMemoryManager;

// 初始化物理内存管理器
void initPMM(PhysicalMemoryManager* pmm, EFI_MEMORY_DESCRIPTOR* map, 
             uint64_t map_size, uint64_t desc_size);

// 分配一个物理页（返回页的物理地址）
uint64_t pmmAllocatePage(PhysicalMemoryManager* pmm);

// 释放一个物理页
void pmmFreePage(PhysicalMemoryManager* pmm, uint64_t page_addr);

// 辅助函数声明
uint64_t calculate_total_memory(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, uint64_t desc_size);
uint64_t find_free_memory_region(EFI_MEMORY_DESCRIPTOR* map, uint64_t map_size, 
                                 uint64_t desc_size, uint64_t pages_needed);
void mark_usable_memory(PhysicalMemoryManager* pmm, EFI_MEMORY_DESCRIPTOR* map, 
                       uint64_t map_size, uint64_t desc_size);
bool is_page_used(PhysicalMemoryManager* pmm, uint64_t page_index);
void mark_page_used(PhysicalMemoryManager* pmm, uint64_t page_index);
void mark_pages_used(PhysicalMemoryManager* pmm, uint64_t start_page, uint64_t count);
void mark_page_free(PhysicalMemoryManager* pmm, uint64_t page_index);

#endif