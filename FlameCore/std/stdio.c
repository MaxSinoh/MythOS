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
    int arg_int;  // 使用带符号类型明确处理负数
    uint32_t arg_uint; // 用于无符号类型（如%x）
    char *arg_str;

    while (index_char) {
        if (index_char != '%') {
            *(buf_ptr++) = index_char;
            index_char = *(++index_ptr);
            continue;
        }

        index_char = *(++index_ptr);
        if (!index_char) break; // 处理格式字符串以%结尾的情况

        switch (index_char) {
            case 'x':
                arg_uint = va_arg(ap, uint32_t); // %x应为无符号数
                itoa(arg_uint, &buf_ptr, 16);
                index_ptr++;
                break;
            case 'd':
                arg_int = va_arg(ap, int);
                if (arg_int < 0) {
                    *buf_ptr++ = '-';
                    arg_uint = (uint32_t)(-arg_int); // 避免溢出（简化处理）
                } else {
                    arg_uint = (uint32_t)arg_int;
                }
                itoa(arg_uint, &buf_ptr, 10);
                index_ptr++;
                break;
            case 'c':
                // 正确处理char类型（提升为int传递）
                *(buf_ptr++) = (char)va_arg(ap, int);
                index_ptr++;
                break;
            case 's':
                arg_str = va_arg(ap, char *);
                if (arg_str == NULL) {
                    arg_str = "(null)"; // 处理空指针
                }
                strcpy(buf_ptr, arg_str);
                buf_ptr += strlen(arg_str);
                index_ptr++;
                break;
            default:
                // 处理未知格式符，直接输出%和字符
                *(buf_ptr++) = '%';
                *(buf_ptr++) = index_char;
                index_ptr++;
                break;
        }
        index_char = *index_ptr; // 更新当前字符
    }
    *buf_ptr = '\0'; // 确保字符串终止
    return (int)(buf_ptr - str); // 更高效的长度计算
}