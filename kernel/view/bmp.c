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

#include "../../includes/firmware/uefi.h"
#include "../../includes/gui/graphics.h"
#include "../../includes/view/bmp.h"

/**
 * @brief 在帧缓冲区中绘制BMP图像
 *
 * 根据给定的帧缓冲区配置和BMP图像数据，在帧缓冲区中绘制BMP图像。
 *
 * @param config 指向帧缓冲区配置的指针
 * @param x 图像在帧缓冲区中的起始x坐标
 * @param y 图像在帧缓冲区中的起始y坐标
 * @param bmp 指向BMP图像头信息的指针
 * @param isTransparent 是否透明处理图像，为1时透明处理，为0时不处理
 */
void drawBMP(const struct FrameBufferConfig *config, uint32_t x, uint32_t y, struct BMP_IMAGE_HEADER *bmp)
{
    // 定位到BMP图像的像素数据位置
    uint8_t *data = (uint8_t *)bmp + bmp->ImageOffset;

    // 如果BMP图像的位深为24位
    if (bmp->BitsPerPixel == 24)
    {
        // 遍历图像的每一行和每一列
        for (uint32_t yOffset = 0; yOffset < bmp->PixelHeight; ++yOffset)
        {
            for (uint32_t xOffset = 0; xOffset < bmp->PixelWidth; ++xOffset)
            {
                // 计算当前像素在数据中的偏移量
                uint32_t offset = (yOffset * bmp->PixelWidth + xOffset) * 3;
                // 获取当前像素的蓝色分量
                uint8_t b = data[offset + 0];
                // 获取当前像素的绿色分量
                uint8_t g = data[offset + 1];
                // 获取当前像素的红色分量
                uint8_t r = data[offset + 2];
                // 将RGB分量合并成一个32位的颜色值
                uint32_t color = (r << 16) | (g << 8) | b;
                // 创建一个PixelColor结构体实例，用于存储颜色值
                PixelColor pixelColor;
                pixelColor.r = (color >> 16) & 0xFF;
                pixelColor.g = (color >> 8) & 0xFF;
                pixelColor.b = color & 0xFF;
                // 绘制当前像素
                drawPixel(config, x + xOffset, y + bmp->PixelHeight - 1 - yOffset, pixelColor);
            }
        }
    }
    // 如果BMP图像的位深为32位
    else if (bmp->BitsPerPixel == 32)
    {
        // 遍历图像的每一行和每一列
        for (uint32_t yOffset = 0; yOffset < bmp->PixelHeight; ++yOffset)
        {
            for (uint32_t xOffset = 0; xOffset < bmp->PixelWidth; ++xOffset)
            {
                // 计算当前像素在数据中的偏移量
                uint32_t offset = (yOffset * bmp->PixelWidth + xOffset) * 4;
                // 获取当前像素的蓝色分量
                uint8_t b = data[offset + 0];
                // 获取当前像素的绿色分量
                uint8_t g = data[offset + 1];
                // 获取当前像素的红色分量
                uint8_t r = data[offset + 2];
                // 将RGB分量合并成一个32位的颜色值
                uint32_t color = (r << 16) | (g << 8) | b;
                // 创建一个PixelColor结构体实例，用于存储颜色值
                PixelColor pixelColor;
                pixelColor.r = (color >> 16) & 0xFF;
                pixelColor.g = (color >> 8) & 0xFF;
                pixelColor.b = color & 0xFF;
                // 绘制当前像素
                drawPixel(config, x + xOffset, y + bmp->PixelHeight - 1 - yOffset, pixelColor);
            }
        }
    }
}