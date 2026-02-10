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

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} PixelColor;

int drawPixel(const struct FrameBufferConfig *config, int x, int y, PixelColor c);
void drawRect(const struct FrameBufferConfig *config, int x, int y, int w, int h, PixelColor color);
int drawLine(const struct FrameBufferConfig *config, int x1, int y1, int x2, int y2, PixelColor color);
void drawChar(const struct FrameBufferConfig *config, const char c, int x, int y, PixelColor color, PixelColor back_color);
void drawStr(const struct FrameBufferConfig *config, const char *s, int x, int y, PixelColor color, PixelColor back_color);
void clearScreen(const struct FrameBufferConfig *config);

#endif