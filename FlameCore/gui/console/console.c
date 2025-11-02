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

#include <gui/console/console.h>
#include <std/string.h>

/**
 * 初始化控制台
 * @param console 控制台对象指针
 * @param config 帧缓冲区配置
 * @param fg_color 前景色
 * @param bg_color 背景色
 */
void initConsole(Console *console, const struct FrameBufferConfig *config, PixelColor fg_color, PixelColor bg_color) {
    if (!console || !config) return;
    
    console->config = config;
    console->fg_color = fg_color;
    console->bg_color = bg_color;
    console->cursor_row = 0;
    console->cursor_column = 0;
    
    // 清空缓冲区
    for (int i = 0; i <= ROWS; i++) {
        for (int j = 0; j <= COLUMNS; j++) {
            console->buffer[i][j] = '\0';
        }
    }
}

/**
 * 在控制台上打印字符串
 * @param console 控制台对象指针
 * @param s 要打印的字符串
 */
void putString(Console *console, const char *s) {
    if (!console || !s) return;
    
    while (*s) {
        // 如果字符是换行符
        if (*s == '\n')
        {
            // 调用newLine函数换行
            newLine(console);
        } 
        // 如果字符是回车符
        else if (*s == '\r') {
            // 将光标列重置为0
            console->cursor_column = 0;
        }
        // 如果字符是制表符
        else if (*s == '\t') {
            // 制表符应该移动到下一个制表位，而不是回到开头
            int spaces = 8 - (console->cursor_column % 8);
            for (int i = 0; i < spaces && console->cursor_column < COLUMNS - 1; i++) {
                console->buffer[console->cursor_row][console->cursor_column] = ' ';
                console->cursor_column++;
            }
        }
        else if (console->cursor_column < COLUMNS - 1) {
            // 只绘制可打印字符
            if (*s >= 32 && *s <= 126) {
                // 创建一个临时的单字符字符串
                char tmp[2] = {*s, '\0'};
                // 在控制台上绘制字符串
                drawStr(console->config, tmp, 9 * console->cursor_column, 16 * console->cursor_row, console->fg_color, console->bg_color);
                // 将字符存储到缓冲区
                console->buffer[console->cursor_row][console->cursor_column] = *s;
                // 光标向右移动一列
                console->cursor_column++;
            }
        }
        s++;
    }
}

/**
 * 在控制台上换行
 * @param console 控制台对象指针
 */
void newLine(Console *console) {
    if (!console) return;
    
    // 将光标列重置为0
    console->cursor_column = 0;

    // 如果当前行小于最大行数减1，则光标向下移动一行
    if (console->cursor_row < ROWS - 1) {
        console->cursor_row++;
    } else {
        // 遍历屏幕上的每个像素点，将其背景色设置为控制台背景色
        for (int y = 0; y < 16 * ROWS; y++) {
            for (int x = 0; x < 9 * COLUMNS; x++) {
                drawPixel(console->config, x, y, console->bg_color);
            }
        }

        // 将缓冲区的内容向上移动一行
        for (int row = 0; row < ROWS - 1; row++) {
            memcpy(console->buffer[row], console->buffer[row + 1], COLUMNS + 1);
            // 绘制字符串到新的位置
            drawStr(console->config, console->buffer[row], 0, 16 * row, console->fg_color, console->bg_color);
        }

        // 清空最后一行缓冲区
        memset(console->buffer[ROWS - 1], 0, COLUMNS + 1);
    }
}