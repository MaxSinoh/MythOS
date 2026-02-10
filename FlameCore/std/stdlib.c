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

#include <std/stdlib.h>


// 把无符号整数转换为字符串，递归实现，结果写入buf_ptr_addr指向的缓冲区，并更新buf_ptr_addr指向下一个写入位置
void itoa(uint32_t value, char **buf_ptr_addr, uint8_t base)
{
    uint32_t m = value % base; // 最低位
    uint32_t i = value / base; // 剩余位
    if (i)
    {
        itoa(i, buf_ptr_addr, base); // 如果有剩余位，先处理剩余位
    }
    if (m < 10)
    {
        *((*buf_ptr_addr)++) = m + '0'; // 0~9，把m加上'0'即可
        // 先将m对应的数字写入，再将buf_ptr_addr指向的指针前移一位
    } else
    {
        *((*buf_ptr_addr)++) = m - 10 + 'A'; // 10~15，把m减去10再加上'A'即可
    }
}

// 64位无符号整数转换为字符串，递归实现，结果写入buf_ptr_addr指向的缓冲区，并更新buf_ptr_addr指向下一个写入位置
void itoa64(uint64_t value, char **buf_ptr_addr, uint8_t base)
{
    uint64_t m = value % base; // 最低位
    uint64_t i = value / base; // 剩余位
    if (i)
    {
        itoa64(i, buf_ptr_addr, base);
    }
    if (m < 10)
    {
        *((*buf_ptr_addr)++) = (char)(m + '0');
    } else
    {
        *((*buf_ptr_addr)++) = (char)(m - 10 + 'A');
    }
}
// 无符号整数转换为字符串，非递归实现，结果写入buf，返回字符串长度
int uitoa(uint32_t value, char *buf, uint8_t base)
{
    char tmp[33];
    int pos = 0;
    if (value == 0)
    {
        tmp[pos++] = '0';
    } else
    {
        while (value)
        {
            uint32_t d = value % base;
            tmp[pos++] = (d < 10) ? (char)('0' + d) : (char)('A' + (d - 10));
            value /= base;
        }
    }
    // 反转字符串
    for (int i = 0; i < pos; i++) buf[i] = tmp[pos - 1 - i];
    buf[pos] = '\0';
    return pos;
}
// 64位无符号整数转换为字符串，非递归实现，结果写入buf，返回字符串长度
int uitoa64(uint64_t value, char *buf, uint8_t base)
{
    char tmp[65];
    int pos = 0;
    if (value == 0)
    {
        tmp[pos++] = '0';
    } else
    {
        while (value)
        {
            uint64_t d = value % base;
            tmp[pos++] = (d < 10) ? (char)('0' + d) : (char)('A' + (d - 10));
            value /= base;
        }
    }
    for (int i = 0; i < pos; i++) buf[i] = tmp[pos - 1 - i];
    buf[pos] = '\0';
    return pos;
}