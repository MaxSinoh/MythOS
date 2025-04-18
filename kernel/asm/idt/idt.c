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
#include <asm/interrupts/default.h>

// 定义 IDT 表和寄存器
struct IDTentry idt[256];  // 中断描述符表，最多支持 256 个中断项
struct idt_ptr idt_reg;  // 指向 IDT 表的结构体


// 函数声明
void set_idt_entry(uint8_t num, uint32_t handler, uint16_t sel, uint8_t flags);

// 初始化 IDT
void initIDT(void) {
    // 初始化 IDT 指针
    idt_reg.limit = sizeof(idt) - 1;
    idt_reg.base = (uint32_t)&idt;

    // 填充默认中断处理程序
    for (int i = 0; i < 256; ++i) {
        set_idt_entry(i, (uint32_t)default_interrupt_handler, 0x08, 0x8E);
    }

    // 加载 IDT
    __asm__ volatile("lidt (%0)" : : "r"(&idt_reg));
}

// 设置单个 IDT 条目
void set_idt_entry(uint8_t num, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = (uint16_t)(handler & 0xFFFF);
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (uint16_t)((handler >> 16) & 0xFFFF);
}