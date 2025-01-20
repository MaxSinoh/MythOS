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
//                     MYTHOS GRAPHICS
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "../../includes/firmware/uefi.h"
#include "../../includes/gui/graphics.h"
#include "../../includes/gui/color.h"

extern uint8_t system_font[];

int drawPixel(const struct FrameBufferConfig *config, int x, int y, PixelColor c) {
    const int pixel_position = config->pixels_per_scan_line * y + x;
    if (config->pixel_format == kRGBR) {
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        *p = c.r;
        *(p+1) = c.g;
        *(p+2) = c.b;
    } else if (config->pixel_format == kBGRR) {
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        *p = c.b;
        *(p+1) = c.g;
        *(p+2) = c.r;
    } else {
        return -1;
    }
    return 0;
}

void drawRect(const struct FrameBufferConfig *config, int x, int y, int w, int h, PixelColor color)
{
    unsigned int i;

    for (i = x; i < (x + w); i++) {
        drawPixel(config, i, y, color); // up
    }
    for (i = x; i < (x + w); i++) {
        drawPixel(config, i, y + h - 1, color); // down
    }

    for (i = y; i < (y + h); i++) {
        drawPixel(config, x, i, color); // left
    }

    for (i = y; i < (y + h); i++) {
        drawPixel(config, x + w - 1, i, color); // right
    }
}

int drawLine(const struct FrameBufferConfig *config, int x0, int y0, int x1, int y1, PixelColor c) {
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        // 绘制当前像素点
        if (drawPixel(config, x0, y0, c) != 0) {
            return -1; // 如果 drawPixel 函数执行失败，则返回错误
        }

        if (x0 == x1 && y0 == y1) {
            break; // 如果已到达直线终点，则退出循环
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    return 0; // 函数执行成功
}

void drawChar(const struct FrameBufferConfig *config, char c, int x, int y, PixelColor color, PixelColor back_color)
{
    uint8_t *font = system_font;
    font += c * 16;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 9; j++) {
            if (font[i] & (0x80 >> j)) {
                // Draw foreground color
                drawPixel(config, x + j, y + i, color);
            } else {
                // Draw background color
                drawPixel(config, x + j, y + i, back_color);
            }
        }
    }
}

void drawStr(const struct FrameBufferConfig *config, char *s, int x, int y, PixelColor color, PixelColor back_color)
{
    for (int i = 0; s[i]; i++) { // Traverse each character of the string to see if it is a null character.
        drawChar(config, s[i], x + 9 * i, y, color, back_color); // Based on (x,y), offset I characters (font 8*16, i.e. i*8 pixels) to draw characters.
    }
}

void clearScreen(const struct FrameBufferConfig *config)
{
    for (int x = 0; x < config->horizontal_resolution; x++) {
        for (int y = 0; y < config->vertical_resolution; y++) {
            drawPixel(config, x, y, white);
        }
    }
}