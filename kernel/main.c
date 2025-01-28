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

#include <universal.h>

Console *console;
int printk(const char *fmt, ...)
{
    va_list ap;         // 可变参数列表
    int result;         // 返回值
    char buf[1024];     // 临时缓冲区
    va_start(ap, fmt);  // 初始化ap
    result = vsprintf(buf, fmt, ap); // 调用vsprintf格式化
    va_end(ap);         // 回收ap
    putString(console, buf); // 输出到控制台
    return result;      // 返回结果
}

/**
 * @brief 主函数，用于在帧缓冲区上绘制图形和文本
 * 该函数接收一个帧缓冲区配置指针和一个引导配置指针，并在帧缓冲区上绘制一系列图形和文本。
 * @param fbc 指向帧缓冲区配置的指针
 * @param BootConfig 引导配置结构体指针
 */
void kernelMain(const struct FrameBufferConfig *fbc, BOOT_CONFIG *BootConfig)
{
    disableInterrupts();    // 禁用中断
    clearScreen(fbc);       // 清除屏幕

    console->config = fbc;       // 设置控制台帧缓冲区配置
    console->fg_color = black;   // 设置控制台前景色为黑色
    console->bg_color = white;   // 设置控制台背景色为白色
    memset(                      // 将控制台缓冲区内存清零
        console->buffer, 0,
        sizeof(console->buffer)
    );    
    console->cursor_row = 0;     // 设置控制台光标行位置为0
    console->cursor_column = 0;  // 设置控制台光标列位置为0

    drawBMP(                     // 绘制logo图片到帧缓冲区
        (struct FrameBufferConfig *)fbc,
        (BMP_IMAGE_HEADER *)logo,
        fbc->horizontal_resolution - 128 - 20,
        fbc->vertical_resolution - 128 -10, 0
    );
    initgdt();                   // 初始化全局描述符表
    
    printk("Welcome to MythOS\n");                      // 打印欢迎信息
    printk("Copyright (C) 2025 MaxSinoh & LuoYuDian");  // 打印版权信息
    
    halt();
}