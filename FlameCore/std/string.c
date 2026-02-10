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

#include <uefi.h>
#include <type.h>
#include <std/string.h>

struct EFI_BOOT_SERVICES *BS;
struct EFI_LOADED_IMAGE_PROTOCOL *LIP;

// 内存操作函数实现
void memset(void *dst_, UINT8 value, UINTN size)
{
    // 将void*类型的指针dst_转换为UINT8*类型的指针dst
    UINT8 *dst = (UINT8 *) dst_;
    while (size-- > 0) 
    {
        // 将指针dst指向的内存位置的值设置为value
        *dst++ = value;
    }
}
// memcpy 函数实现
void *memcpy(void *dst_, const void *src_, UINTN size)
{
    UINT8 *dst = dst_;
    const UINT8 *src = src_;
    while (size-- > 0) 
    {
        // 将源指针指向的数据复制到目标指针指向的位置
        *dst++ = *src++;
    }
    // 返回源指针
    return (void *) src_;
}
// 内存比较函数实现
void *malloc(int buf_size)
{
    void *res;
    unsigned long long status;
    // 调用BS对象的AllocatePool方法分配内存
    status = BS->AllocatePool(LIP ? LIP->ImageDataType : EfiLoaderData, buf_size, &res); 
    // 如果分配失败，返回NULL
    if (status < 0) return NULL;
    // 返回分配的内存地址
    return res;
}
// 内存释放函数实现
void free(void *buf)
{
    // 调用BS对象的FreePool方法释放内存
    BS->FreePool(buf);
}
// 在指定物理地址分配内存
EFI_STATUS mallocAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 调用BS->AllocatePages函数分配内存
    return BS->AllocatePages(AllocateAddress, LIP ? LIP->ImageDataType : EfiLoaderData, (size + 0xfff) / 0x1000, &addr);
}
// 在指定物理地址释放内存
void freeAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 调用BS->FreePages函数释放内存
    BS->FreePages(addr, (size + 0xfff) / 0x1000);
}
// 字符串复制函数实现
char *strcpy(char *dst, const char *src)
{
    char *r = dst;      // 保存原始目标指针，以便返回
    while ((*dst++ = *src++)); // 逐个字符复制，直到遇到字符串结束符'\0'，此时dst和src都指向下一个位置
    return r;
}
// 字符串长度函数实现
int strlen(const char *str)
{
    const char *p = str;    // 定义一个指针p，初始化为字符串的起始地址
    while (*p++);           // 遍历字符串直到遇到结束符'\0'，此时p指向字符串末尾的下一个位置
    return (p - str - 1);   // 返回字符串长度，即p与str之间的距离减去1（因为p多走了一步）
}