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

#include <type.h>
#include <mem/vmm.h>
#include <mem/pmm.h>
#include <std/string.h>

extern PhysicalMemoryManager pmm;
VirtualMemoryManager vmm;

// 计算页表索引
static uint64_t pml4_index(void* virtual_addr)
{
    return ((uint64_t)virtual_addr >> 39) & 0x1FF;
}
static uint64_t pdpt_index(void* virtual_addr)
{
    return ((uint64_t)virtual_addr >> 30) & 0x1FF;
}
static uint64_t pdt_index(void* virtual_addr)
{
    return ((uint64_t)virtual_addr >> 21) & 0x1FF;
}
static uint64_t pte_index(void* virtual_addr)
{
    return ((uint64_t)virtual_addr >> 12) & 0x1FF;
}
// 创建页表并返回其虚拟地址
static PageTable* createPageTable()
{
    void* phys_addr = (void *)pmmAllocatePage();    // 分配物理页
    if (!phys_addr) return NULL;                    // 分配失败
    
    // 确保物理地址被映射到虚拟地址空间
    void* virt_addr = vtop(phys_addr);
    if (!virt_addr) {
        // 如果没有映射，创建临时映射
        virt_addr = (void*)0xFFFF800000000000; // 使用临时虚拟地址
        if (map(virt_addr, phys_addr, 0x03) != 0) {
            pmmFreePage((uint64_t)phys_addr);
            return NULL;
        }
    }
    PageTable* page_table = (PageTable*)virt_addr;  // 使用映射后的虚拟地址
    memset(page_table, 0, PAGE_SIZE);               // 清零页表内容
    return page_table;
}
// 初始化VMM
void initVMM()
{
    vmm.pml4 = createPageTable();                    // 创建PML4表
    if (!vmm.pml4) while(1);                         // 创建失败
    // 映射实际可用的物理内存
    uint64_t total_memory = pmm.total_pages * PAGE_SIZE;
    for (uint64_t addr = 0; addr < total_memory; addr += PAGE_SIZE)
    {
        map((void*)addr, (void*)addr, 0x03); // 映射物理地址到虚拟地址，权限为可读可写
    }
    __asm__ __volatile__("mov %0, %%cr3" :: "r"(vmm.pml4)); // 加载PML4表到CR3寄存器
}

// 映射虚拟地址到物理地址
int map(void* virtual_addr, void* physical_addr, uint64_t flags)
{
    // 获取页表索引
    uint64_t pml4_idx = pml4_index(virtual_addr);
    uint64_t pdpt_idx = pdpt_index(virtual_addr);
    uint64_t pdt_idx = pdt_index(virtual_addr);
    uint64_t pte_idx = pte_index(virtual_addr);
    // 设置PML4表项
    PageTableEntry* pml4Entry = &vmm.pml4->entries[pml4_idx];
    PageTable* pdpt;
    if (!pml4Entry->present)
    {
        pdpt = createPageTable();                      // 创建PDPT表
        if (!pdpt) return -1;                          // 创建失败
        pml4Entry->phys_addr = (uint64_t)pdpt >> 12;   // 设置PDPT表的物理地址
        pml4Entry->present = 1;                        // 设置PDPT表存在位
        pml4Entry->write = (flags & 0x2) ? 1 : 0;      // 设置PDPT表可写位
        pml4Entry->user = (flags & 0x4) ? 1 : 0;       // 设置PDPT表用户位
    } else
    {
        pdpt = (PageTable*)(uintptr_t)(pml4Entry->phys_addr << 12); // 获取PDPT表的虚拟地址
    }
    // 设置PDPT表项
    PageTableEntry* pdptEntry = &pdpt->entries[pdpt_idx];
    PageTable* pdt;
    if (!pdptEntry->present)
    {
        pdt = createPageTable();                      // 创建PDT表
        if (!pdt) return -1;                          // 创建失败
        pdptEntry->phys_addr = (uint64_t)pdt >> 12;   // 设置PDT表的物理地址
        pdptEntry->present = 1;                       // 设置PDT表存在位
        pdptEntry->write = (flags & 0x2) ? 1 : 0;     // 设置PDT表可写位
        pdptEntry->user = (flags & 0x4) ? 1 : 0;      // 设置PDT表用户位
    } else
    {
        pdt = (PageTable*)(uintptr_t)(pdptEntry->phys_addr << 12); // 获取PDT表的虚拟地址
    }
    // 设置PDT表项
    PageTableEntry* pdtEntry = &pdt->entries[pdt_idx];
    PageTable* pte;
    if (!pdtEntry->present)
    {
        pte = createPageTable();                      // 创建PTE表
        if (!pte) return -1;                          // 创建失败
        pdtEntry->phys_addr = (uint64_t)pte >> 12;    // 设置PTE表的物理地址
        pdtEntry->present = 1;                        // 设置PTE表存在位
        pdtEntry->write = (flags & 0x2) ? 1 : 0;      // 设置PTE表可写位
        pdtEntry->user = (flags & 0x4) ? 1 : 0;       // 设置PTE表用户位
    } else
    {
        pte = (PageTable*)(uintptr_t)(pdtEntry->phys_addr << 12); // 获取PTE表的虚拟地址
    }
    // 设置PTE表项
    PageTableEntry* pteEntry = &pte->entries[pte_idx];
    pteEntry->phys_addr = (uint64_t)physical_addr >> 12;    // 设置物理地址
    pteEntry->present = 1;                                  // 设置存在位
    pteEntry->write = (flags & 0x2) ? 1 : 0;                // 设置可写位
    pteEntry->user = (flags & 0x4) ? 1 : 0;                 // 设置用户位

    __asm__ __volatile__("invlpg (%0)" :: "r"(virtual_addr)); // 刷新TLB缓存
    return 0;
}

// 取消映射
int unmap(void* virtual_addr)
{
    // 获取页表索引
    uint64_t pml4_idx = pml4_index(virtual_addr);
    uint64_t pdpt_idx = pdpt_index(virtual_addr);
    uint64_t pdt_idx = pdt_index(virtual_addr);
    uint64_t pte_idx = pte_index(virtual_addr);
    // 设置PML4表项
    PageTableEntry* pml4Entry = &vmm.pml4->entries[pml4_idx];
    if (!pml4Entry->present) return -1; // 不存在则返回错误
    PageTable* pdpt = (PageTable*)(uintptr_t)(pml4Entry->phys_addr << 12);
    // 设置PDPT表项
    PageTableEntry* pdptEntry = &pdpt->entries[pdpt_idx];
    if (!pdptEntry->present) return -1; // 不存在则返回错误
    PageTable* pdt = (PageTable*)(uintptr_t)(pdptEntry->phys_addr << 12);
    // 设置PDT表项
    PageTableEntry* pdtEntry = &pdt->entries[pdt_idx];
    if (!pdtEntry->present) return -1; // 不存在则返回错误
    PageTable* pte = (PageTable*)(uintptr_t)(pdtEntry->phys_addr << 12);
    // 设置PTE表项
    PageTableEntry* pteEntry = &pte->entries[pte_idx];
    if (!pteEntry->present) return -1; // 不存在则返回错误
    memset(pteEntry, 0, sizeof(PageTableEntry)); // 清零PTE表项
    __asm__ __volatile__("invlpg (%0)" :: "r"(virtual_addr)); // 刷新TLB缓存
    return 0;
}

// 虚拟内存地址地址转为转为物理内存地址
void* vtop(void* virtual_addr)
{
    // 获取页表索引
    uint64_t pml4_idx = pml4_index(virtual_addr);
    uint64_t pdpt_idx = pdpt_index(virtual_addr);
    uint64_t pdt_idx = pdt_index(virtual_addr);
    uint64_t pte_idx = pte_index(virtual_addr);
    // 检查PML4表项
    PageTableEntry* pml4Entry = &vmm.pml4->entries[pml4_idx];
    if (!pml4Entry->present) return NULL; // 不存在则返回NULL
    // 注意：这里需要通过物理内存管理器将物理地址转换为虚拟地址
    PageTable* pdpt = (PageTable*)(uintptr_t)(pml4Entry->phys_addr << 12);
    // 检查PDPT表项
    PageTableEntry* pdptEntry = &pdpt->entries[pdpt_idx];
    if (!pdptEntry->present) return NULL; // 不存在则返回NULL
    PageTable* pdt = (PageTable*)(uintptr_t)(pdptEntry->phys_addr << 12);
    // 检查PDT表项
    PageTableEntry* pdtEntry = &pdt->entries[pdt_idx];
    if (!pdtEntry->present) return NULL; // 不存在则返回NULL
    PageTable* pte = (PageTable*)(uintptr_t)(pdtEntry->phys_addr << 12);
    // 检查PTE表项
    PageTableEntry* pteEntry = &pte->entries[pte_idx];
    if (!pteEntry->present) return NULL; // 不存在则返回NULL
    // 正确计算物理地址：页框地址 + 页内偏移
    return (void*)((pteEntry->phys_addr << 12) | ((uint64_t)virtual_addr & 0xFFF));
}