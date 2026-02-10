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

#ifndef _FBC_H_
#define _FBC_H_

#include <type.h>

enum PixelFormat
{
    kRGBR, 
    kBGRR
}; 

struct FrameBufferConfig
{
    uint8_t  *frame_buffer;             // 帧缓冲区地址
    uint32_t pixels_per_scan_line;      // 每行像素数
    uint32_t horizontal_resolution;     // 水平分辨率
    uint32_t vertical_resolution;       // 垂直分辨率
    enum     PixelFormat pixel_format;  // 像素格式
};

#endif