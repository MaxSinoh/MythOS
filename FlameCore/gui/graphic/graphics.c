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

/**
 * @brief 在指定位置绘制一个像素点
 *
 * 在给定的帧缓冲区配置下，在指定位置绘制一个像素点，根据像素格式设置像素颜色。
 *
 * @param config 指向帧缓冲区配置的指针
 * @param x 像素点的x坐标
 * @param y 像素点的y坐标
 * @param c 像素点的颜色
 *
 * @return 成功绘制返回0，如果像素格式不被支持则返回-1
 */
int drawPixel(const struct FrameBufferConfig *config, int x, int y, PixelColor c) {
    // 计算像素位置
    const int pixel_position = config->pixels_per_scan_line * y + x;

    // 判断像素格式
    if (config->pixel_format == kRGBR) {
        // 如果是RGB格式
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        // 设置红色分量
        *p = c.r;
        // 设置绿色分量
        *(p+1) = c.g;
        // 设置蓝色分量
        *(p+2) = c.b;
    } else if (config->pixel_format == kBGRR) {
        // 如果是BGR格式
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        // 设置蓝色分量
        *p = c.b;
        // 设置绿色分量
        *(p+1) = c.g;
        // 设置红色分量
        *(p+2) = c.r;
    } else {
        // 如果不是RGB或BGR格式，返回-1
        return -1;
    }
    return 0;
}

/**
 * @brief 在指定位置绘制一个矩形
 *
 * 使用给定的帧缓冲区配置在指定位置绘制一个矩形。
 *
 * @param config 指向帧缓冲区配置的指针
 * @param x 矩形左上角的x坐标
 * @param y 矩形左上角的y坐标
 * @param w 矩形的宽度
 * @param h 矩形的高度
 * @param color 矩形的颜色
 */
void drawRect(const struct FrameBufferConfig *config, int x, int y, int w, int h, PixelColor color)
{
    unsigned int i;
    // 绘制矩形的上边
    for (i = x; i < (x + w); i++) {
        drawPixel(config, i, y, color);
    }
    // 绘制矩形的下边
    for (i = x; i < (x + w); i++) {
        drawPixel(config, i, y + h - 1, color);
    }
    // 绘制矩形的左边
    for (i = y; i < (y + h); i++) {
        drawPixel(config, x, i, color);
    }
    // 绘制矩形的右边
    for (i = y; i < (y + h); i++) {
        drawPixel(config, x + w - 1, i, color);
    }
}

/**
 * @brief 在帧缓冲区中绘制一条直线
 *
 * 根据给定的起点和终点坐标，在指定的帧缓冲区中绘制一条直线，使用指定的颜色。
 *
 * @param config 指向帧缓冲区配置的指针
 * @param x0 直线的起点 x 坐标
 * @param y0 直线的起点 y 坐标
 * @param x1 直线的终点 x 坐标
 * @param y1 直线的终点 y 坐标
 * @param c 直线的颜色
 *
 * @return 如果成功绘制直线，则返回 0；如果 drawPixel 函数执行失败，则返回 -1
 */
int drawLine(const struct FrameBufferConfig *config, int x0, int y0, int x1, int y1, PixelColor c) {
    // 计算x方向上的差值
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    // 计算y方向上的差值
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    // 计算x方向上的步长
    int sx = x0 < x1 ? 1 : -1;
    // 计算y方向上的步长
    int sy = y0 < y1 ? 1 : -1;
    // 计算初始误差
    int err = dx - dy;
    while (1) {
        // 绘制当前像素点
        if (drawPixel(config, x0, y0, c) != 0) {
            return -1; // 如果 drawPixel 函数执行失败，则返回错误
        }
        if (x0 == x1 && y0 == y1) {
            break; // 如果已到达直线终点，则退出循环
        }
        // 计算误差的2倍
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

/**
 * @brief 在帧缓冲区中绘制单个字符
 *
 * 根据给定的配置、字符、位置以及颜色信息，在帧缓冲区中绘制单个字符。
 *
 * @param config 指向帧缓冲区配置的指针
 * @param c 要绘制的字符
 * @param x 字符在帧缓冲区中的水平位置
 * @param y 字符在帧缓冲区中的垂直位置
 * @param color 字符的前景色
 * @param back_color 字符的背景色
 */
void drawChar(const struct FrameBufferConfig *config, const char c, int x, int y, PixelColor color, PixelColor back_color)
{
    // 获取字体指针
    uint8_t *font = system_font;
    // 字体宽度和高度是固定的，使用宏或常量来提高代码的可维护性
    #define FONT_WIDTH 9
    #define FONT_HEIGHT 16
    
    // 根据字符计算字体指针偏移量
    font += c * FONT_HEIGHT;
    
    // 用于存储位移运算的结果，避免在循环中重复计算
    uint8_t bit_mask = 0x80;
    
    // 遍历字体的每一行
    for (int i = 0; i < FONT_HEIGHT; i++) {
        // 遍历字体的每一列
        for (int j = 0; j < FONT_WIDTH; j++) {
            // 判断当前位是否为1
            if (font[i] & bit_mask) {
                // 绘制前景色
                drawPixel(config, x + j, y + i, color);
            } else {
                // 绘制背景色
                drawPixel(config, x + j, y + i, back_color);
            }
            // 右移一位，准备检查下一个位
            bit_mask >>= 1;
        }
        // 重置位掩码，为下一行准备
        bit_mask = 0x80;
    }
}

/**
 * @brief 在帧缓冲区中绘制字符串
 *
 * 根据给定的帧缓冲区配置、字符串、坐标和颜色信息，在帧缓冲区中绘制字符串。
 *
 * @param config 指向帧缓冲区配置的指针
 * @param s 要绘制的字符串
 * @param x 字符串绘制的起始x坐标
 * @param y 字符串绘制的起始y坐标
 * @param color 字符串绘制使用的颜色
 * @param back_color 字符串绘制使用的背景颜色
 */
void drawStr(const struct FrameBufferConfig *config, const char *s, int x, int y, PixelColor color, PixelColor back_color)
{
    // 遍历字符串中的每个字符
    for (int i = 0; s[i]; i++) { 
        // 调用drawChar函数绘制每个字符
        // 参数依次为：配置、字符、x坐标、y坐标、前景色、背景色
        drawChar(config, s[i], x + 9 * i, y, color, back_color);
    }
}

/**
 * @brief 清除屏幕
 *
 * 使用指定的帧缓冲区配置来清除屏幕，将屏幕上的所有像素点设置为白色。
 *
 * @param config 指向 FrameBufferConfig 结构体的指针，该结构体包含了帧缓冲区的配置信息。
 */
void clearScreen(const struct FrameBufferConfig *config)
{
    // 遍历水平分辨率范围内的每一个点
    for (int x = 0; x < config->horizontal_resolution; x++) {
        // 遍历垂直分辨率范围内的每一个点
        for (int y = 0; y < config->vertical_resolution; y++) {
            // 绘制一个白色像素点
            drawPixel(config, x, y, white);
        }
    }
}