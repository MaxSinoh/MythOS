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

// 中断处理函数数组
InterruptHandler interruptHandlers[256] = {0};

/**
 * @brief 设置中断描述符表（IDT）条目
 *
 * 该函数用于设置中断描述符表中的一个条目，包括中断处理程序的偏移量、段选择器、中断堆栈表索引、类型属性和保留字段。
 *
 * @param entry 指向要设置的IDT条目的指针
 * @param offset 中断处理程序的偏移量
 * @param selector 段选择器
 * @param ist 中断堆栈表索引
 * @param type_attr 类型属性
 */
static void setIDTEntry(IDTEntry *entry, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attr) {
    // 设置偏移量的低16位
    entry->offset_low = (uint16_t)(offset & 0xFFFF);
    // 设置段选择子
    entry->selector = selector;
    // 设置中断栈表索引
    entry->ist = ist;
    // 设置类型属性
    entry->type_attr = type_attr;
    // 设置偏移量的中间16位
    entry->offset_middle = (uint16_t)((offset >> 16) & 0xFFFF);
    // 设置偏移量的高32位
    entry->offset_high = (uint32_t)(offset >> 32);
    // 保留字段设置为0
    entry->reserved = 0;
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
    IDTEntry idt[256];
    // 初始化IDT指针结构体
    IDTPtr idtPtr;
    // 初始化IDT表项
    // 循环初始化每个IDT表项
    for (int i = 0; i < 256; i++) {
        // 设置每个IDT表项的属性
        setIDTEntry(&idt[i], (uint64_t)interruptHandlers[i], 0x08, 0, 0x8E);
    }
    // 设置IDT指针
    // 设置IDT表的大小
    idtPtr.limit = sizeof(IDTEntry) * 256 - 1;
    // 设置IDT表的基地址
    idtPtr.base = (uint64_t)&idt;
    // 加载IDT
    // 使用内联汇编加载IDT表
    __asm__ volatile("lidt (%0)" ::"r"(&idtPtr));
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