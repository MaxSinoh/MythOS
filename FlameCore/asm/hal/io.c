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

#include <asm/hal/io.h>


void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile("outb %1, %0" : : "dN" (port), "a" (value));
}
uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
void outw(uint16_t port, uint16_t value)
{
	__asm__ volatile("outw %1, %0" : : "dN" (port), "a" (value));
}
uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
void outl(uint16_t port,uint32_t value)
{
	__asm__ volatile("outl %1, %0" : : "dN"(port), "a"(value));
}
uint32_t inl(uint16_t port)
{
	uint32_t ret;
	__asm__ volatile("inl %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}
void insw(uint16_t port, void *buf, unsigned long n)
{
	__asm__ volatile("cld; rep; insw"
                 	 : "+D"(buf),
                 	 "+c"(n)
                 	 : "d"(port)
	);
}
void outsw(uint16_t port,const void *buf,unsigned long n)
{
	__asm__ volatile("cld; rep; outsw"
                     : "+S"(buf),
                     "+c"(n)
                     : "d"(port)
	);
}
void insl(uint32_t port,void *addr,int cnt)
{
    __asm__ volatile(
        // 设置方向标志为递增
        "cld;"
        "repne; insl;"							// 使用repne前缀重复执行insl指令，直到ecx寄存器为0
        : "=D" (addr), "=c" (cnt)				// 输出部分，将ecx和edi寄存器的值输出到addr和cnt变量中
        : "d" (port), "0" (addr), "1" (cnt)		// 输入部分，将port、addr和cnt变量的值输入到edx、edi和ecx寄存器中	
        : "memory", "cc"						// 告诉编译器这段代码可能会修改内存和条件码
    );
}
void outsl(uint32_t port,const void *addr,int cnt)
{
	__asm__ volatile("cld;"                                 // 清除方向标志，设置字符串操作为正向
                     "repne; outsl;"                        // 使用repne前缀重复执行outsl指令，直到ecx寄存器为0
                     : "=S" (addr), "=c" (cnt)              // 输出部分，将ecx和esi寄存器的值输出到addr和cnt变量中
                     : "d" (port), "0" (addr), "1" (cnt)    // 输入部分，将port、addr和cnt变量的值输入到edx、esi和ecx寄存器中
                     : "memory", "cc");                     // 告诉编译器这段代码可能会修改内存和条件码
}

void enableInterrupts(void)
{
	__asm__ volatile("sti"); // 启用中断
}
void disableInterrupts(void)
{
    // 禁用中断
	__asm__ volatile("cli" ::: "memory");
}

void halt(void)
{
	disableInterrupts();// 禁用中断
	while(1) 
	{
		// 使CPU进入暂停状态
		__asm__("hlt");
	}
}