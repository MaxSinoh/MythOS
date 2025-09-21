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
PhysicalMemoryManager pmm;  // 声明PMM实例

int printk(const char *fmt, ...)
{
    va_list ap;
    int result;
    char buf[1024];
    va_start(ap, fmt);
    result = vsprintf(buf, fmt, ap);
    va_end(ap);
    putString(console, buf);
    return result;
}

void FlameCoreMain(const struct FrameBufferConfig *fbc, BOOT_CONFIG *BootConfig)
{
    disableInterrupts();
    clearScreen(fbc);

    // 初始化控制台
    console->config = fbc;
    console->fg_color = black;
    console->bg_color = white;
    memset(console->buffer, 0, sizeof(console->buffer));
    console->cursor_row = 0;
    console->cursor_column = 0;

    // 绘制logo
    drawBMP((struct FrameBufferConfig *)fbc, (BMP_IMAGE_HEADER *)logo,
            fbc->horizontal_resolution - 128 - 30,
            fbc->vertical_resolution - 128 - 20, 1);

    initGDT();  // 初始化GDT

    // 获取内存映射信息
    EFI_MEMORY_DESCRIPTOR* memory_map = BootConfig->MemoryMap.Buffer;
    uint64_t map_size = BootConfig->MemoryMap.MapSize;
    uint64_t desc_size = BootConfig->MemoryMap.DescriptorSize;
    
    // 初始化物理内存管理器（传递pmm的地址）
    initPMM(&pmm, memory_map, map_size, desc_size);
    
    // 分配一个物理页（传递pmm的地址）
    uint64_t page_addr = pmmAllocatePage(&pmm);
    printk("Allocated page at: 0x%x\n", page_addr);
    printk("%s [%s] %s [%s]\n", OSNAME, OSVERSION, CORENAME, COREVERSION);
    printk("Copyright (c) 2025 %s Project", OSNAME);
    
    halt();
}