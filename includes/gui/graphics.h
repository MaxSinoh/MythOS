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
//               MYTHOS GRAPHICS HEADER FILE
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "../firmware/uefi.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} PixelColor;

int drawPixel(const struct FrameBufferConfig *config, int x, int y, PixelColor c);
void drawRect(const struct FrameBufferConfig *config, int x, int y, int w, int h, PixelColor color);
int drawLine(const struct FrameBufferConfig *config, int x1, int y1, int x2, int y2, PixelColor color);
void drawChar(const struct FrameBufferConfig *config, char c, int x, int y, PixelColor color, PixelColor back_color);
void drawStr(const struct FrameBufferConfig *config, char *s, int x, int y, PixelColor color, PixelColor back_color);
void clearScreen(const struct FrameBufferConfig *config);

#endif