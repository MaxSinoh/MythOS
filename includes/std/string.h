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

#ifndef _STRING_H_
#define _STRING_H_

#include <type.h>

void memset(void *dst_, UINT8 value, UINTN size);
void *memcpy(void *dst_, const void *src_, UINTN size);
void *malloc(int buf_size);
void free(void *buf);
EFI_STATUS mallocAt(EFI_PHYSICAL_ADDRESS addr, UINTN size);
void freeAt(EFI_PHYSICAL_ADDRESS addr, UINTN size);
char *strcpy(char *dst, const char *src); // 拷贝字符串
int strlen(const char *str);             // 计算字符串长度

#endif