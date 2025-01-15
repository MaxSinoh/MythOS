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

#include "../includes/firmware/uefi.h"
#include "../includes/gui/graphics.h"
#include "../includes/gui/color.h"

void KernelMain(const struct FrameBufferConfig *fbc) {
    // Draw a darker light gray screen.
    for (int x = 0; x < fbc->horizontal_resolution; x++) {
        for (int y = 0; y < fbc->vertical_resolution; y++) {
            WritePixel(fbc, x, y, yellow);
        }
    }
    // Draw a shallow dark gray taskbar (the taskbar is 30 pixels high and located at the bottom of the screen)
    int taskbarHeight = 30; // Taskbar height in pixels
    for (int x = 0; x < fbc->horizontal_resolution; x++) {
        for (int y = fbc->vertical_resolution - taskbarHeight; y < fbc->vertical_resolution; y++) {
            WritePixel(fbc, x, y, darkLightGray);
        }
    }
    while (1) {
        __asm__("hlt");
    }
}