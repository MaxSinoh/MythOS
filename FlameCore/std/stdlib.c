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

void itoa(uint32_t value, char **buf_ptr_addr, uint8_t base)
{
    uint32_t m = value % base; // 最低位
    uint32_t i = value / base; // 剩余位
    if (i) {
        itoa(i, buf_ptr_addr, base); // 如果有剩余位，先处理剩余位
    }

    if (m < 10) {
        *((*buf_ptr_addr)++) = m + '0'; // 0~9，把m加上'0'即可
        // 这是一个常见技巧，利用到ASCII的0~9是连续的
        // *((*buf_ptr_addr)++)的意思是：先将m对应的数字写入，再将buf_ptr_addr指向的指针前移一位
    } else {
        *((*buf_ptr_addr)++) = m - 10 + 'A'; // 'A'~'F'，10对应A，所以先减10再加'A'，同样用A~F连续
    }
}

// 64-bit unsigned integer to string (recursive)
void itoa64(uint64_t value, char **buf_ptr_addr, uint8_t base)
{
    uint64_t m = value % base; // lowest digit
    uint64_t i = value / base; // remaining
    if (i) {
        itoa64(i, buf_ptr_addr, base);
    }

    if (m < 10) {
        *((*buf_ptr_addr)++) = (char)(m + '0');
    } else {
        *((*buf_ptr_addr)++) = (char)(m - 10 + 'A');
    }
}

// Unsigned integer to string (non-recursive) into provided buffer. Returns length.
int uitoa(uint32_t value, char *buf, uint8_t base)
{
    char tmp[33];
    int pos = 0;
    if (value == 0) {
        tmp[pos++] = '0';
    } else {
        while (value) {
            uint32_t d = value % base;
            tmp[pos++] = (d < 10) ? (char)('0' + d) : (char)('A' + (d - 10));
            value /= base;
        }
    }
    // reverse into buf
    for (int i = 0; i < pos; i++) buf[i] = tmp[pos - 1 - i];
    buf[pos] = '\0';
    return pos;
}

int uitoa64(uint64_t value, char *buf, uint8_t base)
{
    char tmp[65];
    int pos = 0;
    if (value == 0) {
        tmp[pos++] = '0';
    } else {
        while (value) {
            uint64_t d = value % base;
            tmp[pos++] = (d < 10) ? (char)('0' + d) : (char)('A' + (d - 10));
            value /= base;
        }
    }
    for (int i = 0; i < pos; i++) buf[i] = tmp[pos - 1 - i];
    buf[pos] = '\0';
    return pos;
}