# MythOS
<br><img src="./materials/logo.png" width="64" height="64" alt="MythOS">
- 一个基于UEFI的现代操作系统，采用x86-64架构。
- 版权所有 © 2025 MythOS Project.
- 保留所有权利。
---
## 工具链
- mingw64 gcc
- x86_64-elf-tools
- qemu-system-x86_64
- GNU Make
---
## 编译
#### 如果要编译但不运行，可以输入：
```
make all
```
#### 之后你将得到一些对象文件，一个EFI文件和一个elf文件。
---
## 运行
#### 如果要编译并运行MythOS，可以输入：
```
make run
```
#### 然后你可以编译并在QEMU中运行MythOS。
---
## 真机启动
### Tip: 前提是你有一台支持UEFI的真机。且你需要一个空的U盘。
- 将U盘格式化为FAT32。
- 将'esp'文件夹中的'EFI'文件夹和'kernel.elf'文件复制到U盘中。
- 用U盘启动你的真机。
- 之后你将看到MythOS的屏幕。
---
# MythOS
<br><img src="./materials/logo.png" width="64" height="64" alt="MythOS">
- A modern operating system with x86-64 architecture based on UEFI.
- Copyright (c) 2025 MythOS Project.
- All rights reserved.
---
## Tools
- mingw64 gcc
- x86_64-elf-tools
- qemu-system-x86_64
- GNU Make
---
## Compile
#### If you want to compiler but not run it, you can type:
```
make all
```
#### Then you will get some object files, one EFI file and one elf file.
---
## Run
#### If you want to compile MythOS and run it, you can type:
```
make run
```
#### Then you can compile MythOS and run it in the QEMU.
---
## Boot from real machine
### Tip: The premise is that you have a real machine and it supports UEFI.And you need an empty USB flash drive.
- Format the USB flash drive to FAT32.
- Copy the 'EFI' folder and the 'kernel.elf' file in the 'esp' folder to the USB flash drive.
- Boot your real machine with the USB flash drive.
- Then you will see the screen of MythOS.
---
