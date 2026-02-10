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

#ifndef _BMP_H_
#define _BMP_H_

#include <type.h>

typedef struct 
{
	uint16_t magic;				// BMP文件的魔数，通常为0x4D42（'BM'）
	uint32_t fileSize;			// BMP文件的总大小
	uint32_t reserved;
	uint32_t bmpDataOffset;		// 图像数据偏移量
	uint32_t bmpInfoSize;		// 信息头大小，通常为40字节
	uint32_t frameWidth;		// 宽度
	uint32_t frameHeight;		// 高度
	uint16_t reservedValue;
	uint16_t bitsPerPixel;		// 每像素位数（24和32）
	uint32_t compressionMode;	// 压缩方式，0表示不压缩
	uint32_t frameSize;			// 图像数据大小，可能为0（当不压缩时）
	uint32_t horizontalResolution;		// 水平分辨率
	uint32_t verticalResolution;		// 垂直分辨率
	uint32_t usedColorCount;			// 使用的颜色数，0表示使用所有颜色
	uint32_t importantColorCount;		// 重要颜色数，0表示所有颜色都重要
} __attribute__((packed)) BMP_IMAGE_HEADER;

void drawBMP(struct FrameBufferConfig *config, BMP_IMAGE_HEADER *bmp, uint32_t x, uint32_t y, int isTransparent);

#endif