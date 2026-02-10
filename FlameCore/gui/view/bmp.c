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

#include <type.h>
#include <gui/graphic/graphics.h>
#include <gui/view/bmp.h>


// 绘制BMP图像
void drawBMP(struct FrameBufferConfig *config, BMP_IMAGE_HEADER *bmp, uint32_t x, uint32_t y, int isTransparent)
{
    if (bmp->magic != 0x4d42)   // 魔数检查，0x4d42对应ASCII的'BM'
    {
        return;
    }
    uint8_t *data = (uint8_t *)bmp + bmp->bmpDataOffset;
    if (bmp->bitsPerPixel == 24)    // 24位BMP图像
    {
        for (uint32_t yOffset = 0; yOffset < bmp->frameHeight; ++yOffset)
        {
            for (uint32_t xOffset = 0; xOffset < bmp->frameWidth; ++xOffset)
            {
                uint32_t offset = (yOffset * bmp->frameWidth + xOffset) * 3;    // 每个像素占3字节（BGR）
                uint8_t b = data[offset + 0];               // BMP文件中的颜色顺序是BGR
                uint8_t g = data[offset + 1];
                uint8_t r = data[offset + 2];
                uint32_t color = (r << 16) | (g << 8) | b;  // 将RGB颜色组合成一个32位整数
                if (isTransparent && color == 0)    // 如果启用了透明处理，并且颜色为0（通常表示黑色或透明），则跳过绘制
                {
                    continue;
                }
                PixelColor p_color = {r, g, b};
                drawPixel(config, x + xOffset, y + bmp->frameHeight - 1 - yOffset, p_color);
            }
        }
    }
    else if (bmp->bitsPerPixel == 32)   // 32位BMP图像
    {
        for (uint32_t yOffset = 0; yOffset < bmp->frameHeight; ++yOffset)
        {
            for (uint32_t xOffset = 0; xOffset < bmp->frameWidth; ++xOffset)
            {
                uint32_t offset = (yOffset * bmp->frameWidth + xOffset) * 4;    // 每个像素占4字节（BGRA）
                uint8_t b = data[offset + 0];   // BMP文件中的颜色顺序是BGRA
                uint8_t g = data[offset + 1];
                uint8_t r = data[offset + 2];
                uint8_t a = data[offset + 3];
                if (isTransparent && a == 0)    // 如果启用了透明处理，并且alpha通道为0（完全透明），则跳过绘制
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