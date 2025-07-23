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

#ifndef _STDARG_H_
#define _STDARG_H_

// 定义va_list类型，用于存储可变参数列表的信息
typedef __builtin_va_list va_list;

// 开始处理可变参数列表
// v: 指向va_list类型的指针
// l: 最后一个固定参数，用于确定可变参数的开始位置
#define va_start(v,l)  __builtin_va_start(v,l)

// 结束处理可变参数列表
// v: 指向va_list类型的指针
#define va_end(v)     __builtin_va_end(v)

// 获取可变参数列表中的下一个参数
// v: 指向va_list类型的指针
// l: 参数的期望类型
#define va_arg(v,l)    __builtin_va_arg(v,l)

// 复制一个va_list到另一个va_list
// d: 目标va_list类型的指针
// s: 源va_list类型的指针
#define va_copy(d,s)  __builtin_va_copy(d,s)

#endif