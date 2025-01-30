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

struct IDTEntry {
    UINT16 offset_low;
    UINT16 selector;
    UINT8 zero;
    UINT8 type_attr;
    UINT16 offset_high;
} __attribute__((packed));

struct IDTTable {
    struct IDTEntry entry[256];
} __attribute__((aligned(4096)));

void initIDT();
void IDTSet(int num, unsigned long selector);
void IDTClear(int num);
void IDTSetAll(unsigned long selector);
void IDTClearAll(void);
void IDTSetHandler(int num, void (*handler)(void));
void IDTClearHandler(int num);
void IDTSetHandlerAll(void (*handler)(void));
void IDTClearHandlerAll(void);


#endif