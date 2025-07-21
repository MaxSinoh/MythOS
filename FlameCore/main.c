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

#include <uefi.h>
#include <type.h>
#include <fbc.h>
#include <asm/hal/io.h>
#include <asm/gdt/gdt.h>
#include <gui/console/console.h>
#include <gui/graphic/color.h>
#include <gui/view/bmp.h>
#include <gui/printk.h>
#include <image/logo.h>
#include <std/stdarg.h>
#include <std/stdio.h>
#include <std/string.h>
#include <mem/pmm.h>

#define OSNAME "MythOS"
#define OSVERSION "0.3.3"
#define CORENAME "FlameCore"
#define COREVERSION "0.2.2"

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
void FlameCoreMain(const struct FrameBufferConfig *fbc, BOOT_CONFIG *BootConfig)
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
        fbc->vertical_resolution - 128 -10, 1
    );
    initGDT();                   // 初始化全局描述符表

    EFI_MEMORY_DESCRIPTOR* memory_map = BootConfig->MemoryMap.Buffer;
    uint64_t map_size = BootConfig->MemoryMap.MapSize;
    uint64_t desc_size = BootConfig->MemoryMap.DescriptorSize;
    
    // 初始化物理内存管理器
    initPMM(memory_map, map_size, desc_size);
    
    // 分配一个物理页
    uint64_t page_addr = pmmAllocatePage();
    printk("Allocated page at: 0x%x\n", page_addr);
    printk("%s [%s] %s [%s]\n", OSNAME, OSVERSION, CORENAME, COREVERSION);  // 打印版权信息
    // printk("Copyright (c) 2025 %s Project", OSNAME);
    
    halt();                      // 停止执行
}