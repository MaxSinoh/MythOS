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

#include <mem/vmm.h>
#include <asm/hal/io.h>
#include <std/string.h>

// 辅助函数：获取页表项索引
static inline uint64_t get_pml4_index(uint64_t addr) {
    return (addr >> 39) & 0x1FF;
}

static inline uint64_t get_pdpt_index(uint64_t addr) {
    return (addr >> 30) & 0x1FF;
}

static inline uint64_t get_pd_index(uint64_t addr) {
    return (addr >> 21) & 0x1FF;
}

static inline uint64_t get_pt_index(uint64_t addr) {
    return (addr >> 12) & 0x1FF;
}

// 初始化虚拟内存管理器
void initVMM(VirtualMemoryManager* vmm, PhysicalMemoryManager* pmm) {
    vmm->pmm = pmm;
    
    // 分配PML4页表，传递pmm参数
    uint64_t pml4_phys = pmmAllocatePage(pmm);
    vmm->pml4 = (page_directory_t*)pml4_phys;
    memset(vmm->pml4, 0, PAGE_SIZE);
    
    // 映射内核区域（示例：映射前1GB内存）
    for (uint64_t i = 0; i < 0x40000000; i += PAGE_SIZE) {
        map_page(vmm, i, i, PAGE_PRESENT | PAGE_WRITE);
    }
    
    // 激活页表
    activate_page_table(vmm->pml4);
}

// 获取或创建页表项
static void* get_or_create_page_table(page_directory_t* dir, uint64_t index, uint64_t flags, 
                                     PhysicalMemoryManager* pmm) {  // 添加pmm参数
    if (!(dir->entries[index] & PAGE_PRESENT)) {
        // 分配新页表，传递pmm参数
        uint64_t phys_addr = pmmAllocatePage(pmm);
        if (!phys_addr) return NULL;
        
        // 清空新页表
        void* table = (void*)phys_addr;
        memset(table, 0, PAGE_SIZE);
        
        // 更新页目录项
        dir->entries[index] = phys_addr | flags | PAGE_PRESENT;
    }
    
    // 返回页表地址（物理地址转虚拟地址）
    return (void*)(dir->entries[index] & ~PAGE_MASK);
}

// 映射虚拟地址到物理地址
void map_page(VirtualMemoryManager* vmm, uint64_t virt_addr, uint64_t phys_addr, uint64_t flags) {
    // 获取各级页表索引
    uint64_t pml4_idx = get_pml4_index(virt_addr);
    uint64_t pdpt_idx = get_pdpt_index(virt_addr);
    uint64_t pd_idx = get_pd_index(virt_addr);
    uint64_t pt_idx = get_pt_index(virt_addr);
    
    // 获取或创建各级页表，传递pmm参数
    page_directory_t* pdpt = get_or_create_page_table(vmm->pml4, pml4_idx, flags, vmm->pmm);
    if (!pdpt) return;
    
    page_directory_t* pd = get_or_create_page_table(pdpt, pdpt_idx, flags, vmm->pmm);
    if (!pd) return;
    
    page_table_t* pt = get_or_create_page_table(pd, pd_idx, flags, vmm->pmm);
    if (!pt) return;
    
    // 映射页表项
    pt->entries[pt_idx] = (phys_addr & ~PAGE_MASK) | flags | PAGE_PRESENT;
    
    // 刷新TLB
    __asm__ volatile("invlpg (%0)" :: "r"(virt_addr) : "memory");
}

// 解除虚拟地址映射
void unmap_page(VirtualMemoryManager* vmm, uint64_t virt_addr) {
    // 获取各级页表索引
    uint64_t pml4_idx = get_pml4_index(virt_addr);
    uint64_t pdpt_idx = get_pdpt_index(virt_addr);
    uint64_t pd_idx = get_pd_index(virt_addr);
    uint64_t pt_idx = get_pt_index(virt_addr);
    
    // 检查PML4项是否存在
    if (!(vmm->pml4->entries[pml4_idx] & PAGE_PRESENT))
        return;
    
    // 获取各级页表
    page_directory_t* pdpt = (page_directory_t*)(vmm->pml4->entries[pml4_idx] & ~PAGE_MASK);
    if (!(pdpt->entries[pdpt_idx] & PAGE_PRESENT))
        return;
    
    page_directory_t* pd = (page_directory_t*)(pdpt->entries[pdpt_idx] & ~PAGE_MASK);
    if (!(pd->entries[pd_idx] & PAGE_PRESENT))
        return;
    
    page_table_t* pt = (page_table_t*)(pd->entries[pd_idx] & ~PAGE_MASK);
    
    // 解除映射
    pt->entries[pt_idx] = 0;
    
    // 刷新TLB
    __asm__ volatile("invlpg (%0)" :: "r"(virt_addr) : "memory");
}

// 获取虚拟地址对应的物理地址
uint64_t get_physical_address(VirtualMemoryManager* vmm, uint64_t virt_addr) {
    // 获取各级页表索引
    uint64_t pml4_idx = get_pml4_index(virt_addr);
    uint64_t pdpt_idx = get_pdpt_index(virt_addr);
    uint64_t pd_idx = get_pd_index(virt_addr);
    uint64_t pt_idx = get_pt_index(virt_addr);
    
    // 检查PML4项是否存在
    if (!(vmm->pml4->entries[pml4_idx] & PAGE_PRESENT))
        return 0;
    
    // 获取各级页表
    page_directory_t* pdpt = (page_directory_t*)(vmm->pml4->entries[pml4_idx] & ~PAGE_MASK);
    if (!(pdpt->entries[pdpt_idx] & PAGE_PRESENT))
        return 0;
    
    page_directory_t* pd = (page_directory_t*)(pdpt->entries[pdpt_idx] & ~PAGE_MASK);
    if (!(pd->entries[pd_idx] & PAGE_PRESENT))
        return 0;
    
    page_table_t* pt = (page_table_t*)(pd->entries[pd_idx] & ~PAGE_MASK);
    if (!(pt->entries[pt_idx] & PAGE_PRESENT))
        return 0;
    
    // 返回物理地址
    return (pt->entries[pt_idx] & ~PAGE_MASK) | (virt_addr & PAGE_MASK);
}

// 激活页表
void activate_page_table(page_directory_t* pml4) {
    uint64_t pml4_phys = (uint64_t)pml4;
    __asm__ volatile("mov %0, %%cr3" :: "r"(pml4_phys) : "memory");
}

// 分配并映射多个连续页
uint64_t allocate_and_map_pages(VirtualMemoryManager* vmm, uint64_t virt_addr, uint64_t count, uint64_t flags) {
    for (uint64_t i = 0; i < count; i++) {
        // 传递pmm参数
        uint64_t phys_addr = pmmAllocatePage(vmm->pmm);
        if (!phys_addr) {
            // 分配失败，回滚之前的映射
            for (uint64_t j = 0; j < i; j++) {
                unmap_page(vmm, virt_addr + j * PAGE_SIZE);
                // 修正pmmFreePage参数，先传pmm指针，再传地址
                pmmFreePage(vmm->pmm, virt_addr + j * PAGE_SIZE);
            }
            return 0;
        }
        
        map_page(vmm, virt_addr + i * PAGE_SIZE, phys_addr, flags);
    }
    
    return virt_addr;
}