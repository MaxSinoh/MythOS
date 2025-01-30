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

#define IDT_GATE(num, selector) \
    ((unsigned long)selector << 16 | (unsigned long)(num) << 8 | 0x8E)

void initIDT(void)
{
    unsigned long IDT[256];
    int i;

    for (i = 0; i < 256; i++) {
        IDT[i] = IDT_GATE(i, 0x08);
    }

    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTSet(int num, unsigned long selector)
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    IDT[num] = IDT_GATE(num, selector);
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTClear(int num)
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    IDT[num] = 0;
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTSetAll(unsigned long selector)
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    for (int i = 0; i < 256; i++) {
        IDT[i] = IDT_GATE(i, selector);
    }
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTClearAll(void)
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    for (int i = 0; i < 256; i++) {
        IDT[i] = 0;
    }
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTSetHandler(int num, void (*handler)(void))
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    IDT[num] = IDT_GATE(num, (unsigned long)handler);
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTClearHandler(int num)
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    IDT[num] = 0;
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTSetHandlerAll(void (*handler)(void))
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    for (int i = 0; i < 256; i++) {
        IDT[i] = IDT_GATE(i, (unsigned long)handler);
    }
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}

void IDTClearHandlerAll(void)
{
    unsigned long IDT[256];

    __asm__ volatile("sidt (%0)" ::"r"(IDT));
    for (int i = 0; i < 256; i++) {
        IDT[i] = 0;
    }
    __asm__ volatile("lidt (%0)" ::"r"(IDT));
}