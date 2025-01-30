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
//                 MYTHOS FBC HEADER FILE
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _FBC_H_
#define _FBC_H_

#include <type.h>

enum PixelFormat {
    // RGB颜色格式，红色在前，绿色居中，蓝色在后
    kRGBR, 
    // BGR颜色格式，蓝色在前，绿色居中，红色在后
    kBGRR
}; 

struct FrameBufferConfig {
    // 指向帧缓冲区的指针
    uint8_t  *frame_buffer;
    // 每扫描线的像素数量
    uint32_t pixels_per_scan_line;
    // 水平分辨率
    uint32_t horizontal_resolution;
    // 垂直分辨率
    uint32_t vertical_resolution;
    // 像素格式
    enum     PixelFormat pixel_format;
};

#endif