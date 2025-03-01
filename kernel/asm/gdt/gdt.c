//
//                       _oo0oo_
//                      o8888888o
//                      88' . '88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \|     |// '.
//                 / \|||  :  |||// "
//                / _||||| -:- |||||- "
//               |   | \\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          .''' '<  `.___\_<|>_/___.' >' '''.
//          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//          \  \ `_.   \_ __\ /__ _/   .-` /  /
//      =====`-.____`.___ \_____/___.-`___.-'=====
//                        `=---='
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//             ***GOD BLESS   ·   NEVER CRASH***
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        COPYRIGHT (C) 2025 MAXSINOH & LUOYUDIAN
//                  ALL RIGHTS RESERVED
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//                     MYTHOS GDT
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <asm/gdt/gdt.h>

#define GDT_LENGTH 5            // GDT 表中的描述符数量
#define dataSelector 0x10       // 数据段选择子，用于数据段和堆栈段的访问

GDT_entry_t GDTentries[GDT_LENGTH];
GDT_ptr_t GDTPtr;

/**
 * @brief 设置全局描述符表(GDT)中的门描述符
 *
 * 该函数用于设置全局描述符表(GDT)中的门描述符。它根据提供的参数设置基地址、限制、访问权限和粒度。
 *
 * @param num 描述符在GDT中的索引号
 * @param base 描述符的基地址
 * @param limit 描述符的限制值
 * @param access 描述符的访问权限
 * @param gran 描述符的粒度
 */
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
/**
 * @brief 初始化全局描述符表（GDT）
 *
 * 此函数用于初始化全局描述符表（GDT），设置各个描述符以支持操作系统运行。
 *
 * 首先，设置 GDT 表的界限（limit）和基地址（base）。然后，调用 GDInstall 函数
 * 设置不同的描述符，包括空描述符、代码段描述符、数据段描述符、用户模式代码段描述符
 * 和用户模式数据段描述符。
 *
 * 接着，使用 LGDT 指令将 GDT 表的地址和界限加载到 GDT 寄存器中。最后，使用 MOV 指令
 * 将数据段选择子（dsel）加载到 DS、FS、GS、ES 和 SS 寄存器中，以完成 GDT 的初始化。
 */
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
}