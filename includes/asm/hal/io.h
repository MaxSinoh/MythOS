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
//                MYTHOS IO HEADER FILE
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _IO_H_
#define _IO_H_

#include <type.h>

void outb(uint16_t port,uint8_t value);
uint8_t inb(uint16_t port);

void outw(uint16_t port,uint16_t value);
uint16_t inw(uint16_t port);

void outl(uint16_t port,uint32_t value);
uint32_t inl(uint16_t port);

void insw(uint16_t port,void *buf,unsigned long n);
void outsw(uint16_t port,const void *buf,unsigned long n);

void insl(uint32_t port,void *addr,int cnt);
void outsl(uint32_t port,const void *addr,int cnt);

void enableInterrupts(void);
void disableInterrupts(void);
void halt(void);

#endif