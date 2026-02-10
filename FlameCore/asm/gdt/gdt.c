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

#include <asm/gdt/gdt.h>
#include <gui/printk.h>

#define GDT_LENGTH 5            // GDT 表中的描述符数量
#define dataSelector 0x10       // 数据段选择子，用于数据段和堆栈段的访问

GDT_entry_t GDTentries[GDT_LENGTH];
GDT_ptr_t GDTPtr;

void GDTInstall(int64_t num,uint64_t base,uint64_t limit,uint8_t access,uint8_t gran)
{
    GDTentries[num].base_low		= (base & 0xFFFF);          // 基地址的低16位
	GDTentries[num].base_middle	    = (base >> 16) & 0xFF;      // 基地址的中8位
	GDTentries[num].base_high		= (base >> 24) & 0xFF;      // 基地址的高8位
	GDTentries[num].limit_low		= (limit & 0xFFFF);         // 限制的低16位
	GDTentries[num].granularity	    = (limit >> 16) & 0x0F;     // 粒度的低4位
	GDTentries[num].granularity	    |= gran & 0xF0;             // 粒度的高4位
	GDTentries[num].access			= access;                   // 访问权限

}

void initGDT(void)
{
	GDTPtr.limit= sizeof(GDT_entry_t) * GDT_LENGTH - 1; // GDT 表的大小 - 1
	GDTPtr.base	= (uint64_t)&GDTentries;	            // 指向 GDT 表的首地址
	GDTInstall(0, 0, 0,          0,    0);              // 空描述符，用于特权级别转换时使用
	GDTInstall(1, 0, 0xFFFFFFFF, 0x9B, 0xA0);	        // 代码段
	GDTInstall(2, 0, 0xFFFFFFFF, 0x93, 0xA0);	        // 数据段
	GDTInstall(3, 0, 0xFFFFFFFF, 0xFB, 0xA0);	        // 用户模式代码段
	GDTInstall(4, 0, 0xFFFFFFFF, 0xF3, 0xA0);	        // 用户模式数据段
	__asm__ volatile                    // 使用汇编语言加载 GDT 寄存器并设置数据段选择子
	(
		".intel_syntax noprefix\n"
		"lgdt [GDTPtr]\n"               // 将 GDT 表加载到 GDT 寄存器中
		".att_syntax prefix\n"
		:
		: [GDTPtr] "*p" (&GDTPtr)       // 将 GDT 表的地址传递给汇编代码
        : "memory"                      // 告诉编译器，这段代码会修改内存中的数据
	);
	__asm__ volatile                    // 使用汇编语言设置数据段选择子
	(
		"mov %[dsel], %%ds\n"           // 将数据段选择子加载到 DS、FS、GS、ES 和 SS 寄存器中
		"mov %[dsel], %%fs\n"
		"mov %[dsel], %%gs\n"
		"mov %[dsel], %%es\n"
		"mov %[dsel], %%ss\n"
		:
		: [dsel] "rm" (dataSelector)    // 将数据段选择子传递给汇编代码
        : "memory"                      // 告诉编译器，这段代码会修改内存中的数据
	);  
	printk("GDT Initialized\n");
}