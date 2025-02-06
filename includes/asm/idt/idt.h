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
//              MYTHOS IDT HEADER FILE
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _IDT_H_
#define _IDT_H_

#include <type.h>

typedef struct
{
    // 手动压入
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t code;
    // 自动压入
    uint32_t eip, cs, eflags;
    uint32_t esp3, ss3; 
} interrupt_frame_t;

// 定义中断门描述符结构
typedef struct {
    uint16_t offset_low;      // 中断处理函数偏移地址低16位
    uint16_t selector;        // 代码段选择子
    uint16_t type_attr;       // 中断门类型和属性
    uint16_t offset_high;     // 中断处理函数偏移地址高32位
} IDTEntry;

// 定义IDT指针结构
typedef struct {
    uint16_t limit;           // IDT表的大小 - 1
    uint64_t base;            // IDT表的基地址
} IDTPtr;

// 定义中断处理函数类型
typedef void (*InterruptHandler)(void);
// 设置中断门描述符
void setInterruptGate(int vector, uint32_t offset, uint32_t selector, uint16_t type_attr);
// 初始化IDT
void initIDT(void);

#endif