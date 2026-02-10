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

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <fbc.h>
#include <gui/graphic/graphics.h>

#define ROWS 50     // 行数
#define COLUMNS 90  // 列数

typedef struct
{
    const struct FrameBufferConfig *config; // 指向帧缓冲配置的指针
    PixelColor fg_color;    // 前景色
    PixelColor bg_color;    // 背景色
    char buffer[ROWS + 1][COLUMNS + 1];     // 字符缓冲区，存储当前显示的字符
    int cursor_row;         // 光标当前行
    int cursor_column;      // 光标当前列
} Console;

// 初始化控制台
void initConsole(Console *console, const struct FrameBufferConfig *config, PixelColor fg_color, PixelColor bg_color);
void putString(Console *console, const char *s);    // 输出字符串
void newLine(Console *console);     // 换行

#endif