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
//                  MYTHOS BMP VIEWER
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <type.h>
#include <gui/graphic/graphics.h>
#include <gui/view/bmp.h>

void drawBMP(struct FrameBufferConfig *config, BMP_IMAGE_HEADER *bmp, uint32_t x, uint32_t y, int isTransparent)
{
    if (bmp->magic != 0x4d42)
    {
        return;
    }

    uint8_t *data = (uint8_t *)bmp + bmp->bmpDataOffset;

    if (bmp->bitsPerPixel == 24)
    {
        for (uint32_t yOffset = 0; yOffset < bmp->frameHeight; ++yOffset)
        {
            for (uint32_t xOffset = 0; xOffset < bmp->frameWidth; ++xOffset)
            {
                uint32_t offset = (yOffset * bmp->frameWidth + xOffset) * 3;
                uint8_t b = data[offset + 0];
                uint8_t g = data[offset + 1];
                uint8_t r = data[offset + 2];
                uint32_t color = (r << 16) | (g << 8) | b;
                if (isTransparent && color == 0)
                {
                    continue;
                }
                PixelColor p_color = {r, g, b};
                drawPixel(config, x + xOffset, y + bmp->frameHeight - 1 - yOffset, p_color);
            }
        }
    }
    else if (bmp->bitsPerPixel == 32)
    {
        for (uint32_t yOffset = 0; yOffset < bmp->frameHeight; ++yOffset)
        {
            for (uint32_t xOffset = 0; xOffset < bmp->frameWidth; ++xOffset)
            {
                uint32_t offset = (yOffset * bmp->frameWidth + xOffset) * 4;
                uint8_t b = data[offset + 0];
                uint8_t g = data[offset + 1];
                uint8_t r = data[offset + 2];
                uint8_t a = data[offset + 3];
                if (isTransparent && a == 0)
                {
                    continue;
                }
                PixelColor p_color = {r, g, b};
                drawPixel(config, x + xOffset, y + bmp->frameHeight - 1 - yOffset, p_color);
            }
        }
    }
    else
    {
        return;
    }
}