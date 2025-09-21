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

#ifndef _VMM_H_
#define _VMM_H_

#include <type.h>
#include <mem/pmm.h>  // 已经包含PAGE_SIZE定义，不需要重复定义

// 从pmm.h继承PAGE_SIZE定义，避免重复
#define PAGE_SHIFT 12
#define PAGE_MASK 0xFFF

// 页表项标志位
#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_CACHE_DISABLE 0x10
#define PAGE_ACCESSED 0x20
#define PAGE_DIRTY 0x40
#define PAGE_LARGE 0x80
#define PAGE_GLOBAL 0x100

// 页表项类型
typedef uint64_t pte_t;
typedef uint64_t pde_t;

// 页表结构
typedef struct {
    pte_t entries[512];
} page_table_t;

// 页目录结构
typedef struct {
    pde_t entries[512];
} page_directory_t;

// 虚拟内存管理器结构
typedef struct {
    page_directory_t* pml4;          // PML4页表（x86_64）
    PhysicalMemoryManager* pmm;      // 物理内存管理器指针
} VirtualMemoryManager;

// 初始化虚拟内存管理器
void initVMM(VirtualMemoryManager* vmm, PhysicalMemoryManager* pmm);

// 映射虚拟地址到物理地址
void map_page(VirtualMemoryManager* vmm, uint64_t virt_addr, uint64_t phys_addr, uint64_t flags);

// 解除虚拟地址映射
void unmap_page(VirtualMemoryManager* vmm, uint64_t virt_addr);

// 获取虚拟地址对应的物理地址
uint64_t get_physical_address(VirtualMemoryManager* vmm, uint64_t virt_addr);

// 激活页表
void activate_page_table(page_directory_t* pml4);

// 分配并映射多个连续页
uint64_t allocate_and_map_pages(VirtualMemoryManager* vmm, uint64_t virt_addr, uint64_t count, uint64_t flags);

#endif