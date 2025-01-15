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
//                     MYTHOS GRAPHICS
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "../../includes/firmware/uefi.h"
#include "../../includes/gui/graphics.h"


int WritePixel(const struct FrameBufferConfig *config, int x, int y, PixelColor c) {
    const int pixel_position = config->pixels_per_scan_line * y + x;
    if (config->pixel_format == kRGBR) {
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        *p = c.r;
        *(p+1) = c.g;
        *(p+2) = c.b;
    } else if (config->pixel_format == kBGRR) {
        uint8_t *p = &config->frame_buffer[4 * pixel_position];
        *p = c.b;
        *(p+1) = c.g;
        *(p+2) = c.r;
    } else {
        return -1;
    }
    return 0;
}