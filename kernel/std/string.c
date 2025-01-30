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
//                    MYTHOS STRING
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <uefi.h>
#include <type.h>
#include <std/string.h>

struct EFI_BOOT_SERVICES *BS;
struct EFI_LOADED_IMAGE_PROTOCOL *LIP;

/**
 * @brief 将指定内存区域的内容设置为指定的值
 *
 * 将从 dst_ 指向的内存地址开始，size 字节的内存区域的内容全部设置为 value。
 *
 * @param dst_ 指向需要设置的内存区域的指针
 * @param value 需要设置的字节值
 * @param size 需要设置的字节数
 */
void memset(void *dst_, UINT8 value, UINTN size)
{
    // 将void*类型的指针dst_转换为UINT8*类型的指针dst
    UINT8 *dst = (UINT8 *) dst_;
    // 当size大于0时，执行循环
    while (size-- > 0) 
    {
        // 将指针dst指向的内存位置的值设置为value
        *dst++ = value;
    }
}

/**
 * @brief 复制内存数据
 *
 * 将指定大小的数据从源地址复制到目标地址。
 *
 * @param dst_ 目标地址指针
 * @param src_ 源地址指针
 * @param size 要复制的数据大小（以字节为单位）
 *
 * @return 返回源地址指针
 */
void *memcpy(void *dst_, const void *src_, UINTN size)
{
    // 将目标指针和源指针转换为 UINT8 类型
    UINT8 *dst = dst_;
    const UINT8 *src = src_;
    // 当剩余大小大于0时，循环复制数据
    while (size-- > 0) 
    {
        // 将源指针指向的数据复制到目标指针指向的位置
        *dst++ = *src++;
    }
    // 返回源指针
    return (void *) src_;
}

/**
 * @brief 动态分配内存
 *
 * 根据给定的缓冲区大小，动态分配内存，并返回分配的内存指针。
 *
 * @param buf_size 要分配的缓冲区大小（以字节为单位）
 *
 * @return 指向分配的内存的指针，如果分配失败则返回NULL
 */
void *malloc(int buf_size)
{
    // 声明一个指向void的指针变量res
    void *res;
    // 声明一个unsigned long long类型的变量status
    unsigned long long status;
    // 调用BS的AllocatePool函数分配内存，并将返回的状态赋值给status
    // 参数LIP->ImageDataType或EfiLoaderData根据LIP是否为空决定，buf_size为要分配的内存大小，res为分配的内存地址
    status = BS->AllocatePool(LIP ? LIP->ImageDataType : EfiLoaderData, buf_size, &res); 
    // 如果status小于0，表示分配内存失败，返回NULL
    if (status < 0) return NULL;
    // 返回分配的内存地址
    return res;
}

/**
 * @brief 释放内存
 *
 * 释放由BS->AllocatePool函数分配的内存块。
 *
 * @param buf 要释放的内存块指针
 */
void free(void *buf)
{
    // 调用BS对象的FreePool方法，释放buf指向的内存
    BS->FreePool(buf);
}

/**
 * @brief 在指定物理地址分配内存
 *
 * 在EFI环境下，根据指定的物理地址和大小分配内存。
 *
 * @param addr 指定分配内存的物理地址
 * @param size 需要分配的内存大小（以字节为单位）
 * @return 分配成功返回EFI_SUCCESS，否则返回错误代码
 */
EFI_STATUS mallocAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 调用BS->AllocatePages函数来分配内存
    // AllocateAddress参数表示分配地址由调用者指定
    // LIP ? LIP->ImageDataType : EfiLoaderData 是条件表达式，用于选择内存类型
    // (size + 0xfff) / 0x1000 是将所需分配的大小向上舍入到最接近的4KB倍数
    // &addr 是分配内存的物理地址指针
    return BS->AllocatePages(AllocateAddress, LIP ? LIP->ImageDataType : EfiLoaderData, (size + 0xfff) / 0x1000, &addr);
}

/**
 * @brief 释放指定的物理内存
 *
 * 释放从给定地址开始的指定大小的物理内存。
 *
 * @param addr 要释放的物理内存起始地址
 * @param size 要释放的物理内存大小（以字节为单位）
 */
void freeAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 调用BS->FreePages函数释放内存
    // 参数1：要释放的内存起始地址
    // 参数2：要释放的页数（通过计算得出）
    BS->FreePages(addr, (size + 0xfff) / 0x1000);
}

char *strcpy(char *dst, const char *src)
{
    char *r = dst; // 原指针，按要求返回
    while ((*dst++ = *src++)); // 小trick：copy当前字节，两指针加一，判断是否为\0，写到一行里了
    return r; // 返回
}

int strlen(const char *str)
{
    const char *p = str; // 原指针，用于计算长度
    while (*p++); // 不断后移，到0为止
    return (p - str - 1); // 0的地址-首地址-0所占的1字节长度就是字符串长度
}