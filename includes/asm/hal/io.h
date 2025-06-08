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