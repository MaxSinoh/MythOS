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
#include <mem/pmm.h>

#define PAGE_SIZE 4096      // 页大小（4KB）
#define PT_ENTRIES 512      // 页表项数量，每个页表512个项

// 页表项结构
typedef struct
{
    uint64_t present : 1;       // 页存在于物理内存
    uint64_t write : 1;         // 可写
    uint64_t user : 1;          // 用户可访问（0：内核；1：用户）
    uint64_t pwt : 1;           // 页级写透
    uint64_t pcd : 1;           // 页级缓存禁用
    uint64_t accessed : 1;      // 已访问
    uint64_t dirty : 1;         // 已修改
    uint64_t pat : 1;           // 页属性表
    uint64_t global : 1;        // 全局页
    uint64_t available : 3;     // 可用位
    uint64_t phys_addr : 40;    // 物理页框地址
    uint64_t reserved : 12;     // 保留位
} PageTableEntry;

// 页表结构（四级页表：PML4 → PDPT → PDT → PTE）
typedef struct
{
    PageTableEntry entries[PT_ENTRIES];
} PageTable;

// 虚拟内存管理器
typedef struct
{
    PageTable* pml4;        // PML4页表基地址（物理地址）
} VirtualMemoryManager;

void initVMM();
int map(void* virtual_addr, void* physical_addr, uint64_t flags);
int unmap(void* virtual_addr);
void* vtop(void* virtual_addr);

#endif