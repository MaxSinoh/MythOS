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
#include <std/stdio.h>
#include <std/stdarg.h>
#include <std/string.h>
#include <fbc.h>
#include <gui/graphic/color.h>

struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

int vsprintf(char *str, const char *format, va_list ap)
{
    char *buf_ptr = str; // 指针指向str首地址
    const char *index_ptr = format; // 指向format首地址
    char index_char = *index_ptr; // format当前字符值，随时更新
    int arg_int; // int参数
    char *arg_str; // str参数

    while (index_char) { // 循环直到字符串尾
        if (index_char != '%') {
            *(buf_ptr++) = index_char; // 当前字符更新至str
            index_char = *(++index_ptr); // 获取下一个字符
            continue; // 跳过后续处理
        }
        // 是%，由于只支持%s %c %x %d直接获取%后面字符
        index_char = *(++index_ptr);
        switch (index_char) { // 枚举s c x d四种情况
            case 'x':
                arg_int = va_arg(ap, int); // 获取下一个int参数
                itoa(arg_int, &buf_ptr, 16); // 转化为16进制，直接写到str里
                index_char = *(++index_ptr); // 下一个
                break;
            case 'd': // 十进制与十六进制几乎相同
                arg_int = va_arg(ap, int); // 获取下一个int参数
                if (arg_int < 0) { // 如果是负的
                    arg_int = 0 - arg_int; // 取相反数
                    *buf_ptr++ = '-'; // 加一个负号
                }
                itoa(arg_int, &buf_ptr, 10); // 转化为10进制，直接写到str里
                index_char = *(++index_ptr); // 下一个
                break;
            case 'c':
                *(buf_ptr++) = va_arg(ap, int); // 把char参数直接写进去
                index_char = *(++index_ptr); // 下一个
                break;
            case 's':
                arg_str = va_arg(ap, char *); // 获取str类型参数
                strcpy(buf_ptr, arg_str); // strcpy复制，后面会写
                buf_ptr += strlen(arg_str); // 移位，不然指针会错，strlen后面也会写
                index_char = *(++index_ptr); // 下一个
                break;
        }
    }
    return strlen(str); // 返回替换后长度，也是标准内容
}