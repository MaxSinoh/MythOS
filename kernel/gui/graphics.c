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