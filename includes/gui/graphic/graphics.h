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

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <fbc.h>
#include <type.h>

typedef struct {
    // 红色分量
    uint8_t r;
    // 绿色分量
    uint8_t g;
    // 蓝色分量
    uint8_t b;
} PixelColor;

// 在指定位置绘制一个像素点
int drawPixel(const struct FrameBufferConfig *config, int x, int y, PixelColor c);

// 在指定位置绘制一个矩形
void drawRect(const struct FrameBufferConfig *config, int x, int y, int w, int h, PixelColor color);

// 在指定两点之间绘制一条直线
int drawLine(const struct FrameBufferConfig *config, int x1, int y1, int x2, int y2, PixelColor color);

// 在指定位置绘制一个字符
void drawChar(const struct FrameBufferConfig *config, const char c, int x, int y, PixelColor color, PixelColor back_color);

// 在指定位置绘制一个字符串
void drawStr(const struct FrameBufferConfig *config, const char *s, int x, int y, PixelColor color, PixelColor back_color);

// 清除屏幕内容
void clearScreen(const struct FrameBufferConfig *config);

#endif