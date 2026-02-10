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

#include <fbc.h>
#include <gui/graphic/graphics.h>
#include <gui/graphic/color.h>


extern uint8_t system_font[];

int drawPixel(const struct FrameBufferConfig *config, int x, int y, PixelColor c)
{
    // 计算像素位置
    const int pixel_position = config->pixels_per_scan_line * y + x;
    // 判断像素格式
    if (config->pixel_format == kRGBR)
    {
        // RGB格式
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        *p = c.r;// 设置红色分量
        *(p+1) = c.g;// 设置绿色分量
        *(p+2) = c.b;// 设置蓝色分量
    } else if (config->pixel_format == kBGRR)
    {
        // BGR格式
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        *p = c.b;// 设置蓝色分量
        *(p+1) = c.g;// 设置绿色分量 
        *(p+2) = c.r;// 设置红色分量
    } else
    {
        return -1;
    }
    return 0;
}
// 绘制矩形
void drawRect(const struct FrameBufferConfig *config, int x, int y, int w, int h, PixelColor color)
{
    unsigned int i;
    // 上边
    for (i = x; i < (x + w); i++)
    {
        drawPixel(config, i, y, color);
    }
    // 下边
    for (i = x; i < (x + w); i++)
    {
        drawPixel(config, i, y + h - 1, color);
    }
    // 左边
    for (i = y; i < (y + h); i++)
    {
        drawPixel(config, x, i, color);
    }
    // 右边
    for (i = y; i < (y + h); i++)
    {
        drawPixel(config, x + w - 1, i, color);
    }
}
// 绘制直线
int drawLine(const struct FrameBufferConfig *config, int x0, int y0, int x1, int y1, PixelColor c)
{
    
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1); // x方向上的差值
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1); // y方向上的差值
    int sx = x0 < x1 ? 1 : -1;       // x方向上的步长
    int sy = y0 < y1 ? 1 : -1;       // y方向上的步长
    // 计算初始误差
    int err = dx - dy;
    while (1)
    {
        // 绘制当前像素点
        if (drawPixel(config, x0, y0, c) != 0)
        {
            return -1; // 如果 drawPixel 函数执行失败，则返回错误
        }
        if (x0 == x1 && y0 == y1)
        {
            break; // 如果已到达直线终点，则退出循环
        }
        // 计算误差的2倍
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
    return 0;
}
// 绘制字符
void drawChar(const struct FrameBufferConfig *config, const char c, int x, int y, PixelColor color, PixelColor back_color)
{
    uint8_t *font = system_font;// 字体指针
    #define FONT_WIDTH 9
    #define FONT_HEIGHT 16
    // 根据字符计算字体指针偏移量
    font += c * FONT_HEIGHT;
    // 用于存储位移运算的结果，避免在循环中重复计算
    uint8_t bit_mask = 0x80;
    for (int i = 0; i < FONT_HEIGHT; i++)
    {
        for (int j = 0; j < FONT_WIDTH; j++)
        {
            if (font[i] & bit_mask)
            {
                drawPixel(config, x + j, y + i, color);         // 绘制前景色
            } else
            {
                drawPixel(config, x + j, y + i, back_color);    // 绘制背景色
            }
            // 右移一位，准备检查下一个位
            bit_mask >>= 1;
        }
        // 重置位掩码，为下一行准备
        bit_mask = 0x80;
    }
}
// 绘制字符串
void drawStr(const struct FrameBufferConfig *config, const char *s, int x, int y, PixelColor color, PixelColor back_color)
{
    for (int i = 0; s[i]; i++)
    { 
        drawChar(config, s[i], x + 9 * i, y, color, back_color);// 绘制每个字符
    }
}
// 清屏函数
void clearScreen(const struct FrameBufferConfig *config)
{
    for (int x = 0; x < config->horizontal_resolution; x++)
    {
        for (int y = 0; y < config->vertical_resolution; y++)
        {
            drawPixel(config, x, y, white);
        }
    }
}