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

#include <type.h>

typedef struct 
{
	uint16_t magic;
	uint32_t fileSize;
	uint32_t reserved;
	uint32_t bmpDataOffset;
	uint32_t bmpInfoSize;
	uint32_t frameWidth;
	uint32_t frameHeight;
	uint16_t reservedValue;
	uint16_t bitsPerPixel;
	uint32_t compressionMode;
	uint32_t frameSize;
	uint32_t horizontalResolution;
	uint32_t verticalResolution;
	uint32_t usedColorCount;
	uint32_t importantColorCount;
} __attribute__((packed)) BMP_IMAGE_HEADER;

void drawBMP(struct FrameBufferConfig *config, BMP_IMAGE_HEADER *bmp, uint32_t x, uint32_t y, int isTransparent);

#endif