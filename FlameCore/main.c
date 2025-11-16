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
#include <asm/idt/idt.h>
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
#define OSVERSION "0.3.6"
#define CORENAME "FlameCore"
#define COREVERSION "0.2.4"

// 使用静态变量替代未初始化指针
static Console main_console;
static PhysicalMemoryManager pmm;

int printk(const char *fmt, ...)
{
    va_list ap;
    int result;
    char buf[1024];
    va_start(ap, fmt);
    result = vsprintf(buf, fmt, ap);
    va_end(ap);
    putString(&main_console, buf);
    return result;
}

void FlameCoreMain(const struct FrameBufferConfig *fbc, BOOT_CONFIG *BootConfig)
{
    disableInterrupts();
    clearScreen(fbc);

    // 安全初始化控制台
    initConsole(&main_console, fbc, black, white);

    // 绘制logo（检查logo是否有效，通过检查BMP头魔数）
    if (((BMP_IMAGE_HEADER *)logo)->magic == 0x4D42) {  // "BM" in little endian
        drawBMP((struct FrameBufferConfig *)fbc, (BMP_IMAGE_HEADER *)logo,
                fbc->horizontal_resolution - 128 - 30,
                fbc->vertical_resolution - 128 - 20, 1);
    }

    initGDT();  // 初始化GDT
    initIDT();  // 初始化IDT

    // 获取内存映射信息（安全检查）
    if (BootConfig && BootConfig->MemoryMap.Buffer) {
        EFI_MEMORY_DESCRIPTOR* memory_map = BootConfig->MemoryMap.Buffer;
        uint64_t map_size = BootConfig->MemoryMap.MapSize;
        uint64_t desc_size = BootConfig->MemoryMap.DescriptorSize;

        // 初始化物理内存管理器
        initPMM(&pmm, memory_map, map_size, desc_size);

        // 测试内存分配
        uint64_t page_addr = pmmAllocatePage(&pmm);
        if (page_addr) {
            printk("Allocated page at: 0x%x\n", page_addr);
        } else {
            printk("ERROR: Memory allocation failed\n");
        }
        
        // 测试分配多个页面
        printk("Testing multiple page allocation...\n");
        for (int i = 0; i < 10; i++) {
            uint64_t addr = pmmAllocatePage(&pmm);
            if (addr) {
                printk("Page %d: 0x%x\n", i, addr);
            } else {
                printk("ERROR: Failed to allocate page %d\n", i);
                break;
            }
        }
    } else {
        printk("ERROR: Invalid memory map in BootConfig\n");
    }

    printk("\n%s [%s] %s [%s]\n", OSNAME, OSVERSION, CORENAME, COREVERSION);
    printk("Copyright (c) 2025 %s Project\n\n", OSNAME);
    
    // 显示内存使用情况
    if (pmm.bitmap) {
        printk("Memory: %d pages total, %d used\n", 
               pmm.total_pages, pmm.used_pages);
    }

    halt();
}