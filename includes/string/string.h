#ifndef _STRING_H_
#define _STRING_H_

#include "../firmware/uefi.h"

void memset(void *dst_, UINT8 value, UINTN size);
void *memcpy(void *dst_, const void *src_, UINTN size);
void *malloc(int buf_size);

#endif