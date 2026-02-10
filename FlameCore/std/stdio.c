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
#include <std/stdlib.h>
#include <std/stdio.h>
#include <std/stdarg.h>
#include <std/string.h>
#include <fbc.h>
#include <gui/graphic/color.h>

struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

// 简单实现一个 vsprintf，支持 %d、%u、%x、%o、%b、%c、%s、%p，支持宽度和精度
int vsprintf(char *str, const char *format, va_list ap)
{
    char *buf_ptr = str;
    const char *index_ptr = format;
    char index_char = *index_ptr;
    while (index_char)
    {
        if (index_char != '%')      // 普通字符直接复制
        {
            *(buf_ptr++) = index_char;
            index_char = *(++index_ptr);
            continue;
        }
        index_char = *(++index_ptr);
        if (!index_char) break;
        // 解析标志、宽度、精度、长度
        int left_align = 0;
        int zero_pad = 0;
        int width = 0;
        int precision = -1; // 默认不限制精度
        int is_ll = 0;
        // 标志
        while (1)
        {
            if (index_char == '-')          // 左对齐
            {
                left_align = 1;
                index_char = *(++index_ptr);
            } else if (index_char == '0')   // 零填充（仅当不左对齐时有效）
            {
                zero_pad = 1;
                index_char = *(++index_ptr);
            } else {
                break;
            }
        }
        // 宽度
        while (index_char >= '0' && index_char <= '9')
        {
            width = width * 10 + (index_char - '0');
            index_char = *(++index_ptr);
        }
        // 精度
        if (index_char == '.')
        {
            index_char = *(++index_ptr);
            precision = 0;
            while (index_char >= '0' && index_char <= '9') {
                precision = precision * 10 + (index_char - '0');
                index_char = *(++index_ptr);
            }
        }
        // 长度
        if (index_char == 'l' && *(index_ptr + 1) == 'l')
        {
            is_ll = 1;
            index_char = *(index_ptr + 2);
            index_ptr += 2;
        }
        // 处理格式符
        switch (index_char)
        {
            case 'x':   // 输出十六进制整数，应用宽度和精度
            {
                uint8_t base = 16;
                char numbuf[64];
                int len;
                if (is_ll)
                {
                    unsigned long long val64 = va_arg(ap, unsigned long long);
                    len = uitoa64((uint64_t)val64, numbuf, base);
                } else
                {
                    uint32_t val32 = va_arg(ap, uint32_t);
                    len = uitoa(val32, numbuf, base);
                }
                // 应用精度（最小位数）
                if (precision > len)
                {
                    int pad = precision - len;
                    char ch = '0';
                    for (int i = 0; i < pad; i++) *(buf_ptr++) = ch;
                    len += pad;
                }
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0)
                {
                    char ch = zero_pad && !left_align ? '0' : ' ';
                    if (!left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                    if (left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else
                {
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                }
                index_ptr++;
            }
            case 'o':   // 输出八进制整数，应用宽度和精度
            {
                uint8_t base = 8;
                char numbuf[64];
                int len;
                if (is_ll)
                {
                    unsigned long long val64 = va_arg(ap, unsigned long long);
                    len = uitoa64((uint64_t)val64, numbuf, base);
                } else
                {
                    uint32_t val32 = va_arg(ap, uint32_t);
                    len = uitoa(val32, numbuf, base);
                }
                // 应用精度（最小位数）
                if (precision > len)
                {
                    int pad = precision - len;
                    char ch = '0';
                    for (int i = 0; i < pad; i++) *(buf_ptr++) = ch;
                    len += pad;
                }
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0)
                {
                    char ch = zero_pad && !left_align ? '0' : ' ';
                    if (!left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                    if (left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else
                {
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                }
                index_ptr++;
            }
            case 'b':   // 输出二进制整数，应用宽度和精度
            {
                uint8_t base = 2;
                char numbuf[64];
                int len;
                if (is_ll)
                {
                    unsigned long long val64 = va_arg(ap, unsigned long long);
                    len = uitoa64((uint64_t)val64, numbuf, base);
                } else
                {
                    uint32_t val32 = va_arg(ap, uint32_t);
                    len = uitoa(val32, numbuf, base);
                }
                // 应用精度（最小位数）
                if (precision > len)
                {
                    int pad = precision - len;
                    char ch = '0';
                    for (int i = 0; i < pad; i++) *(buf_ptr++) = ch;
                    len += pad;
                }
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0)
                {
                    char ch = zero_pad && !left_align ? '0' : ' ';
                    if (!left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                    if (left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else
                {
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                }
                index_ptr++;
            }
            case 'u':   // 输出无符号整数，应用宽度和精度
            {
                uint8_t base = (index_char == 'x') ? 16 : (index_char == 'o') ? 8 : (index_char == 'b') ? 2 : 10;
                char numbuf[64];
                int len;
                if (is_ll)
                {
                    unsigned long long val64 = va_arg(ap, unsigned long long);
                    len = uitoa64((uint64_t)val64, numbuf, base);
                } else
                {
                    uint32_t val32 = va_arg(ap, uint32_t);
                    len = uitoa(val32, numbuf, base);
                }
                // 应用精度（最小位数）
                if (precision > len)
                {
                    int pad = precision - len;
                    char ch = '0';
                    for (int i = 0; i < pad; i++) *(buf_ptr++) = ch;
                    len += pad;
                }
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0)
                {
                    char ch = zero_pad && !left_align ? '0' : ' ';
                    if (!left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                    if (left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else
                {
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                }
                index_ptr++;
            } break;
            case 'd':   // 输出有符号十进制整数，应用宽度和精度
            {
                int val = va_arg(ap, int);
                char numbuf[32];
                int len;
                int negative = 0;
                if (val < 0)
                {
                    negative = 1;
                    val = -val;
                }
                len = uitoa((uint32_t)val, numbuf, 10);
                if (negative) len++;
                // 应用精度
                if (precision > (len - negative))
                {
                    int pad = precision - (len - negative);
                    char ch = '0';
                    for (int i = 0; i < pad; i++) *(buf_ptr++) = ch;
                    len += pad;
                }
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0)
                {
                    char ch = zero_pad && !left_align ? '0' : ' ';
                    if (!left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    if (negative) *(buf_ptr++) = '-';
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len - negative;
                    if (left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else
                {
                    if (negative) *(buf_ptr++) = '-';
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len - negative;
                }
                index_ptr++;
            } break;
            case 'c':   // 输出单个字符，应用宽度
                *(buf_ptr++) = (char)va_arg(ap, int);
                index_ptr++;
                break;
            case 's':   // 输出字符串，应用宽度和精度
            {
                char *arg_str = va_arg(ap, char *);
                if (arg_str == NULL) arg_str = "(null)";
                int len = strlen(arg_str);
                if (precision >= 0 && precision < len) len = precision;
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0)
                {
                    char ch = ' ';
                    if (!left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    for (int i = 0; i < len; i++) *(buf_ptr++) = arg_str[i];
                    if (left_align)
                    {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else
                {
                    for (int i = 0; i < len; i++) *(buf_ptr++) = arg_str[i];
                }
                index_ptr++;
            } break;
            case 'p':   // 输出指针地址，固定宽度 16 位，16 进制，前缀 0x
            {
                void *p = va_arg(ap, void *);
                uint64_t addr = (uint64_t)p;
                char numbuf[32];
                int len = uitoa64(addr, numbuf, 16);
                // 固定宽度 16 位，零填充
                *(buf_ptr++) = '0';
                *(buf_ptr++) = 'x';
                int pad = 16 - len;
                for (int i = 0; i < pad; i++) *(buf_ptr++) = '0';
                strcpy(buf_ptr, numbuf);
                buf_ptr += len;
                index_ptr++;
            } break;
            default:    // 不支持的格式符，直接原样输出
            {
                *(buf_ptr++) = '%';
                *(buf_ptr++) = index_char;
                index_ptr++;
                break;
            }
        }
        index_char = *index_ptr;
    }
    *buf_ptr = '\0';
    return (int)(buf_ptr - str);    // 返回写入的字符数（不包括结尾的 null 字符）
}