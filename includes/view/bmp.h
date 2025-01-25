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
//                MYTHOS BMP HEADER FILE
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _BMP_H_
#define _BMP_H_

#include "../firmware/uefi.h"

// 定义BMP颜色映射结构体
struct BMP_COLOR_MAP
{
    UINT8 Blue;         // 蓝色分量
    UINT8 Green;        // 绿色分量
    UINT8 Red;          // 红色分量
    UINT8 Reserved;     // 保留字段，通常为0
};

// 定义BMP图像头部结构体
struct BMP_IMAGE_HEADER
{
    char    CharB;          // 文件类型标识符B（'B'）
    char    CharM;          // 文件类型标识符M（'M'）
    UINT32  Size;           // 文件大小（字节为单位）
    UINT16  Reserved[2];    // 保留字段，通常为0
    UINT32  ImageOffset;    // 图像数据的起始位置（字节为单位）
    UINT32  HeaderSize;     // 本头部的大小（字节为单位）
    UINT32  PixelWidth;     // 图像的宽度（像素为单位）
    UINT32  PixelHeight;    // 图像的高度（像素为单位）
    UINT16  Planes;         // 目标设备的平面数，通常为1
    UINT16  BitsPerPixel;   // 每个像素的位数
    UINT32  CompressionType;// 压缩类型
    UINT32  ImageSize;      // 图像大小（字节为单位），当使用RGB时，可以设置为0
    UINT32  XpixelsPerMeter;// 水平分辨率（每米像素数）
    UINT32  YpixelsPerMeter;// 垂直分辨率（每米像素数）
    UINT32  NumberOfColors; // 实际使用的颜色表中的颜色索引数
    UINT32  ImportantColors;// 重要颜色数，通常为0
};

// 在指定的帧缓冲区上绘制BMP图像的函数声明
// 参数包括帧缓冲区配置、图像左上角的x和y坐标、BMP图像头部指针以及是否透明的标志
void drawBMP(const struct FrameBufferConfig *config, uint32_t x, uint32_t y, struct BMP_IMAGE_HEADER *bmp);

#endif