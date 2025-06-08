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

#define ROWS 25  // 定义控制台行数
#define COLUMNS 90  // 定义控制台列数

typedef struct {  // 定义一个控制台结构体
    const struct FrameBufferConfig *config;  // 指向帧缓冲配置的指针
    PixelColor fg_color;  // 前景色
    PixelColor bg_color;  // 背景色
    char buffer[ROWS + 1][COLUMNS + 1];  // 控制台缓冲区，多一行一列用于存储结束符
    int cursor_row;  // 光标当前行
    int cursor_column;  // 光标当前列
} Console;

// 初始化控制台
void initConsole(Console *console, const struct FrameBufferConfig *config, PixelColor fg_color, PixelColor bg_color);
// 在控制台输出字符串
void putString(Console *console, const char *s);
// 在控制台换行
void newLine(Console *console);

#endif