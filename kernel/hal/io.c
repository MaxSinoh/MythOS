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
//                     MYTHOS IO
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <hal/io.h>

/**
 * @brief 向指定端口输出一个字节的数据
 *
 * 使用内联汇编将字节数据写入指定的I/O端口。
 *
 * @param port I/O端口的地址
 * @param value 要写入端口的字节数据
 */
void outb(uint16_t port, uint8_t value)
{
	// 使用内联汇编将value写入到指定的port端口
	__asm__ volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

/**
 * @brief 从指定的I/O端口读取一个字节
 *
 * 从指定的I/O端口读取一个字节的数据。
 *
 * @param port I/O端口号
 * @return 从指定I/O端口读取到的字节数据
 */
uint8_t inb(uint16_t port)
{
	// 声明一个uint8_t类型的变量ret，用于存储从指定端口读取的数据
	uint8_t ret;
	// 使用内联汇编指令从指定端口读取数据，并将读取的数据存储在变量ret中
	// "%1"对应输入参数port，"dN"表示port是一个无符号16位整数，并存储在DX寄存器中
	// "%0"对应输出参数ret，"=a"表示将结果存储在EAX寄存器中，并赋值给ret
	__asm__ volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	// 返回从指定端口读取的数据
	return ret;
}

/**
 * @brief 向指定端口写入16位数据
 *
 * 使用内联汇编指令将16位数据写入指定的I/O端口。
 *
 * @param port I/O端口地址
 * @param value 要写入的数据
 */
void outw(uint16_t port, uint16_t value)
{
	// 使用内联汇编将值写入指定的端口
	__asm__ volatile("outw %1, %0" : : "dN" (port), "a" (value));
}


/**
 * @brief 从指定的I/O端口读取16位数据
 *
 * 使用内联汇编指令从指定的I/O端口读取16位数据。
 *
 * @param port I/O端口号
 * @return 读取到的16位数据
 */
uint16_t inw(uint16_t port)
{
	// 声明一个uint16_t类型的变量ret用于存储返回值
	uint16_t ret;
	// 使用内联汇编从指定端口读取数据到ret中
	__asm__ volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	// 返回读取到的数据
	return ret;
}

/**
 * @brief 向指定的I/O端口写入32位值
 *
 * 使用内联汇编将32位无符号整数写入指定的I/O端口。
 *
 * @param port 要写入的I/O端口号
 * @param value 要写入的值
 */
void outl(uint16_t port,uint32_t value)
{
	// 使用内联汇编进行输出操作
	__asm__ volatile("outl %1, %0" : : "dN"(port), "a"(value));
}

/**
 * @brief 从指定端口读取32位数据
 *
 * 使用内联汇编从指定的I/O端口读取32位数据。
 *
 * @param port 输入端口地址
 * @return 从指定端口读取的32位数据
 */
uint32_t inl(uint16_t port)
{
	uint32_t ret;
	// 使用内联汇编读取I/O端口
	__asm__ volatile("inl %1, %0" : "=a"(ret) : "dN"(port));
	return ret;
}

/**
 * @brief 从指定的端口地址读取多个字数据到缓冲区
 *
 * 使用内联汇编指令从指定的端口地址读取多个字数据（2字节）到缓冲区中。
 *
 * @param port 端口地址
 * @param buf 指向接收数据的缓冲区的指针
 * @param n 要读取的字数（2字节单位）
 */
void insw(uint16_t port, void *buf, unsigned long n)
{
	// 使用内嵌汇编指令
	__asm__ volatile("cld; rep; insw"
				 	 // 输出操作数：buf的地址，"D"表示edi寄存器，"+D"表示该寄存器是读写操作
                 	 : "+D"(buf),
				     // 输出操作数：n的值，"c"表示ecx寄存器，"+c"表示该寄存器是读写操作
                 	 "+c"(n)
				 	 // 输入操作数：port的值，"d"表示dx寄存器
                 	 : "d"(port)
	);
}

/**
 * @brief 向指定端口输出字符串
 *
 * 该函数通过内联汇编将给定缓冲区中的数据通过指定端口输出。
 *
 * @param port 输出端口地址
 * @param buf 待输出的缓冲区指针
 * @param n 待输出的字节数
 */
void outsw(uint16_t port,const void *buf,unsigned long n)
{
    // 使用内联汇编进行字符串操作
	__asm__ volatile("cld; rep; outsw"
                     : "+S"(buf),  // 输出操作数，buf地址被更新
                     "+c"(n)       // 输出操作数，n被更新
                     : "d"(port)   // 输入操作数，port作为目标端口
	);
}

/**
 * @brief 使用内联汇编将指定端口的数据批量读取到指定内存地址中
 *
 * 通过内联汇编的方式，将指定端口的数据批量读取到指定内存地址中。
 *
 * @param port 端口号
 * @param addr 要读取数据的目标内存地址
 * @param cnt 要读取的数据块数量
 */
void insl(uint32_t port,void *addr,int cnt)
{
    // 使用内联汇编代码进行字符串加载操作
    __asm__ volatile(
        // 设置方向标志为递增
        "cld;"
        "repne; insl;"                          // 使用repne前缀执行重复加载操作，直到ecx寄存器中的计数减到0
        : "=D" (addr), "=c" (cnt)               // 输出部分，将edi寄存器和ecx寄存器的值赋给addr和cnt
        : "d" (port), "0" (addr), "1" (cnt)     // 输入部分，将port的值赋给edx寄存器，将addr和cnt的值分别赋给edi和ecx寄存器
        : "memory", "cc"                        // 被修改的部分，包括内存和条件码
    );
}

/**
 * @brief 使用内联汇编实现将指定内存区域的数据通过指定的端口连续输出
 *
 * 使用内联汇编指令`outsl`，通过指定的端口将数据连续输出。
 *
 * @param port 输出端口号
 * @param addr 指向要输出数据的内存区域的指针
 * @param cnt 要输出的数据单元数量
 */
void outsl(uint32_t port,const void *addr,int cnt)
{
	// 使用内联汇编实现outsl指令
	__asm__ volatile("cld;"                                 // 清除方向标志，设置字符串操作为正向
                     "repne; outsl;"                        // 使用repne前缀重复执行outsl指令，直到ecx寄存器为0
                     : "=S" (addr), "=c" (cnt)              // 输出部分，将ecx和esi寄存器的值输出到addr和cnt变量中
                     : "d" (port), "0" (addr), "1" (cnt)    // 输入部分，将port、addr和cnt变量的值输入到edx、esi和ecx寄存器中
                     : "memory", "cc");                     // 告诉编译器这段代码可能会修改内存和条件码
}


/**
 * @brief 启用中断
 *
 * 该函数用于启用CPU的中断功能。
 *
 * @return 无返回值
 */
void enable_interrupts(void)
{
	// 启用中断
	__asm__ volatile("sti"); // 使用汇编指令sti启用中断
}

/**
 * @brief 禁用中断
 *
 * 该函数用于禁用CPU的中断。在执行某些关键代码段时，为了防止被中断打断，需要暂时禁用中断。
 *
 * @note 该函数使用内联汇编指令 "cli" 来禁用中断。
 *
 * @warning 调用此函数后，必须确保在适当的时候重新启用中断，以避免造成系统挂起或崩溃。
 */
void disable_interrupts(void)
{
    // 禁用中断
	__asm__ volatile("cli" ::: "memory");
}

/**
 * @brief 停止系统执行
 *
 * 此函数会禁用中断，并使系统进入一个无限循环，使用HLT指令暂停CPU执行。
 * 调用此函数后，系统将无法继续执行其他任务，通常用于系统崩溃或需要立即停止执行的情况。
 */
void halt(void)
{
	// 禁用中断
	disable_interrupts();
	// 进入一个无限循环
	while(1) 
	{
		// 调用汇编指令hlt，使CPU进入暂停状态
		__asm__("hlt");
	}
}