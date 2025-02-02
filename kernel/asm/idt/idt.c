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
//                      MYTHOS IDT
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <asm/idt/idt.h>

// 中断门描述符格式宏定义
#define IDT_GATE(num, selector) ((unsigned long)selector << 16 | (unsigned long)(num) << 8 | 0x8E)
// 中断门描述符格式：中断向量号 | 代码段选择子 | 中断类型（0x8E）

// 中断处理函数数组
InterruptHandler interruptHandlers[256] = {0};

void setIDTEntry(IDTEntry *entry, uint16_t offset, uint16_t selector, uint16_t type_attr)
{
    entry->offset_low = offset & 0xFFFF;
    entry->selector = selector;
    entry->type_attr = type_attr;
    entry->offset_high = (offset >> 16) & 0xFFFF;
}

/**
 * @brief 初始化中断描述符表（IDT）
 *
 * 该函数负责初始化中断描述符表（IDT），并加载到CPU中。
 *
 * @return 无返回值
 */
void initIDT(void) {
    // 定义IDT表和IDT指针结构体
    unsigned long idt[256];
 // 遍历数组，为每个中断向量分配一个空的中断门
    for (int i = 0; i < 256; i++) {
        // 使用IDT_GATE宏生成一个中断门描述符，并将其存储在idt数组中
        // 参数i表示中断向量号，0x08表示中断门类型
        idt[i] = IDT_GATE(i, 0x08);
    }
    // 使用lidt指令加载中断描述符表到CPU
    // "%0"是一个占位符，用于在汇编代码中引用C语言变量
    // "r"表示输入操作数，这里将idt数组的地址传递给汇编代码
    // "volatile"关键字告诉编译器，该汇编代码不能被优化掉
    __asm__ volatile("lidt (%0)" ::"r"(idt));
}

/**
 * @brief 注册中断处理函数
 *
 * 将指定的中断处理函数注册到中断处理函数数组中。
 *
 * @param index 中断索引
 * @param handler 中断处理函数
 */
void registerInterruptHandler(uint8_t index, InterruptHandler handler) {
    // 将中断处理程序handler注册到中断处理程序数组中对应索引index的位置
    interruptHandlers[index] = handler;
}