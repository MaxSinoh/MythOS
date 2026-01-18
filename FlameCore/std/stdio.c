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

int vsprintf(char *str, const char *format, va_list ap)
{
    char *buf_ptr = str;
    const char *index_ptr = format;
    char index_char = *index_ptr;

    while (index_char) {
        if (index_char != '%') {
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
        int precision = -1; // -1 means no precision
        int is_ll = 0;

        // 标志
        while (1) {
            if (index_char == '-') {
                left_align = 1;
                index_char = *(++index_ptr);
            } else if (index_char == '0') {
                zero_pad = 1;
                index_char = *(++index_ptr);
            } else {
                break;
            }
        }

        // 宽度
        while (index_char >= '0' && index_char <= '9') {
            width = width * 10 + (index_char - '0');
            index_char = *(++index_ptr);
        }

        // 精度
        if (index_char == '.') {
            index_char = *(++index_ptr);
            precision = 0;
            while (index_char >= '0' && index_char <= '9') {
                precision = precision * 10 + (index_char - '0');
                index_char = *(++index_ptr);
            }
        }

        // 长度
        if (index_char == 'l' && *(index_ptr + 1) == 'l') {
            is_ll = 1;
            index_char = *(index_ptr + 2);
            index_ptr += 2;
        }

        // 处理格式符
        switch (index_char) {
            case 'x':
            case 'o':
            case 'b':
            case 'u': {
                uint8_t base = (index_char == 'x') ? 16 : (index_char == 'o') ? 8 : (index_char == 'b') ? 2 : 10;
                char numbuf[64];
                int len;
                if (is_ll) {
                    unsigned long long val64 = va_arg(ap, unsigned long long);
                    len = uitoa64((uint64_t)val64, numbuf, base);
                } else {
                    uint32_t val32 = va_arg(ap, uint32_t);
                    len = uitoa(val32, numbuf, base);
                }
                // 应用精度（最小位数）
                if (precision > len) {
                    int pad = precision - len;
                    char ch = '0';
                    for (int i = 0; i < pad; i++) *(buf_ptr++) = ch;
                    len += pad;
                }
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0) {
                    char ch = zero_pad && !left_align ? '0' : ' ';
                    if (!left_align) {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                    if (left_align) {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else {
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len;
                }
                index_ptr++;
            } break;
            case 'd': {
                int val = va_arg(ap, int);
                char numbuf[32];
                int len;
                int negative = 0;
                if (val < 0) {
                    negative = 1;
                    val = -val;
                }
                len = uitoa((uint32_t)val, numbuf, 10);
                if (negative) len++;
                // 应用精度
                if (precision > (len - negative)) {
                    int pad = precision - (len - negative);
                    char ch = '0';
                    for (int i = 0; i < pad; i++) *(buf_ptr++) = ch;
                    len += pad;
                }
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0) {
                    char ch = zero_pad && !left_align ? '0' : ' ';
                    if (!left_align) {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    if (negative) *(buf_ptr++) = '-';
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len - negative;
                    if (left_align) {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else {
                    if (negative) *(buf_ptr++) = '-';
                    strcpy(buf_ptr, numbuf);
                    buf_ptr += len - negative;
                }
                index_ptr++;
            } break;
            case 'c':
                *(buf_ptr++) = (char)va_arg(ap, int);
                index_ptr++;
                break;
            case 's': {
                char *arg_str = va_arg(ap, char *);
                if (arg_str == NULL) arg_str = "(null)";
                int len = strlen(arg_str);
                if (precision >= 0 && precision < len) len = precision;
                // 应用宽度
                int total_pad = width - len;
                if (total_pad > 0) {
                    char ch = ' ';
                    if (!left_align) {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                    for (int i = 0; i < len; i++) *(buf_ptr++) = arg_str[i];
                    if (left_align) {
                        for (int i = 0; i < total_pad; i++) *(buf_ptr++) = ch;
                    }
                } else {
                    for (int i = 0; i < len; i++) *(buf_ptr++) = arg_str[i];
                }
                index_ptr++;
            } break;
            case 'p': {
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
            default:
                *(buf_ptr++) = '%';
                *(buf_ptr++) = index_char;
                index_ptr++;
                break;
        }
        index_char = *index_ptr;
    }
    *buf_ptr = '\0';
    return (int)(buf_ptr - str);
}