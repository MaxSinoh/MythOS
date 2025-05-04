# MythOS
<br><img src="../materials/logo.png" width="64" height="64" alt="MythOS">
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
