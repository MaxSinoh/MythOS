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
#include <mem/vmm.h>

#define OSNAME "MythOS"
#define OSVERSION "0.3.3"
#define CORENAME "FlameCore"
#define COREVERSION "0.1.2"

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
    disableInterrupts();
    clearScreen(fbc);
    initPMM(BootConfig->MemoryMap.Buffer, BootConfig->MemoryMap.MapSize, BootConfig->MemoryMap.DescriptorSize);
    initVMM(); // 开启分页
    void* virt_page = (void*)0x10000;
    console = (Console*)pmmAllocatePage(); // 从堆分配控制台结构
    console->config = fbc;
    console->fg_color = black;
    console->bg_color = white;
    memset(console->buffer, 0, sizeof(console->buffer));
    console->cursor_row = 0;
    console->cursor_column = 0;
    drawBMP((struct FrameBufferConfig *)fbc, (BMP_IMAGE_HEADER *)logo, 
            fbc->horizontal_resolution - 128 - 30,
            fbc->vertical_resolution - 128 - 20, 1);
    initGDT();
    printk("Mapped virtual: %x -> physical: %x\n", (uint64_t)virt_page, (uint64_t)vtop(virt_page));
    printk("%s [%s] %s [%s]\n", OSNAME, OSVERSION, CORENAME, COREVERSION);
    printk("Copyright (c) 2025 %s Project", OSNAME);
    halt();
}