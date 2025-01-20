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

void kernelMain(const struct FrameBufferConfig *fbc, BOOT_CONFIG *BootConfig) {
    clearScreen(fbc);
    drawStr(fbc, "Welcome to use MythOS!", 30, 30, black, white);
    drawStr(fbc, "Copyright (c) 2025 MaxSinoh", 30, 50, black, white);
    drawStr(fbc, "Open source address: https://github.com/MaxSinoh/MythOS/", 30, 70, black, white);
    for (int x = 100; x < 200; x++) {
        for (int y = 100; y < 200; y++) {
            drawPixel(fbc, x, y, yellow);
        }
    }
    drawRect(fbc, 100, 100, 100, 100, black);
    drawRect(fbc, 111, 111, 78, 78, black);
    drawLine(fbc, 150, 111, 150, 170, black);
    drawLine(fbc, 130, 130, 130, 188, black);
    drawLine(fbc, 170, 130, 170, 188, black);
    
    while (1);
}