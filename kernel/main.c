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
//                  MYTHOS KERNEL MAIN
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <firmware/uefi.h>
#include <gui/graphics.h>
#include <gui/color.h>
#include <console/console.h>
#include <std/stdio.h>
#include <string/string.h>
#include <view/bmp.h>
#include <std/stdarg.h>

Console *console;
int printk(const char *fmt, ...);

/**
 * @brief 主函数，用于在帧缓冲区上绘制图形和文本
 * 该函数接收一个帧缓冲区配置指针和一个引导配置指针，并在帧缓冲区上绘制一系列图形和文本。
 * @param fbc 指向帧缓冲区配置的指针
 * @param BootConfig 引导配置结构体指针
 */
void kernelMain(const struct FrameBufferConfig *fbc, BOOT_CONFIG *BootConfig)
{
    // 清屏
    clearScreen(fbc);
    // 在指定区域内绘制黄色像素点
    for (int x = 100; x < 200; x++) {
        for (int y = 100; y < 200; y++) {
            // 绘制像素点
            drawPixel(fbc, x, y, yellow);
        }
    }
    // 绘制黑色矩形
    drawRect(fbc, 100, 100, 100, 100, black);
    // 绘制另一个黑色矩形
    drawRect(fbc, 111, 111, 78, 78, black);
    // 绘制黑色直线
    drawLine(fbc, 150, 111, 150, 170, black);
    // 绘制另一条黑色直线
    drawLine(fbc, 130, 130, 130, 188, black);
    // 绘制第三条黑色直线
    drawLine(fbc, 170, 130, 170, 188, black);

    console->config = fbc;
    console->fg_color = black;
    console->bg_color = white;
    memset(console->buffer, 0, sizeof(console->buffer));
    console->cursor_row = 0;
    console->cursor_column = 0;

    printk("Welcome to MythOS!\n");
    printk("All rights reserved.\n");

    while (1);
}
int printk(const char *fmt, ...)
{
    va_list ap; // 可变参数列表
    int result;
    char buf[1024]; // 临时缓冲区
    va_start(ap, fmt); // 初始化ap
    result = vsprintf(buf, fmt, ap); // 调用vsprintf格式化
    va_end(ap); // 回收ap
    putString(console, buf); // 输出到控制台
    return result; // 返回结果
}