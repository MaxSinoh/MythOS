# MythOS
<br><img src="../docs/logo.png" width="64" height="64" alt="MythOS">

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
## Make的使用
#### 构建整个项目，包括编译 BootLoader、内核，并安装到 ESP 目录中，但不运行虚拟机：
```
make
```
```
make all
```
##### 指定线程数来加速编译，例如使用4个线程进行编译：
```
make -j4
``` 
#### 仅编译 BootLoader 的 EFI 文件，而不编译内核或运行虚拟机：
```
make efi
```
#### 仅编译内核的对象文件，而不链接生成 ELF 文件、安装到 ESP 目录或运行虚拟机：
```
make objects
```
#### 仅链接生成内核的 ELF 文件，而不安装到 ESP 目录或运行虚拟机：
```
make link
```
####  编译带有调试符号的版本：
```
make debug
```
#### 编译优化过的版本：
```
make release
```
#### 安装到 ESP 目录但不运行：
```
make install
```
#### 构建项目、安装到 ESP 目录，并在 QEMU 中运行 MythOS：
```
make run
```
#### 删除生成的二进制文件、对象文件和 ESP 目录：
```
make clean
```
#### 查看所有可用的目标和它们的描述：
```
make help
```
---
## 真机启动
### Tip: 前提是你有一台支持UEFI的真机。且你需要一个空的U盘。
- 将U盘格式化为FAT32。
- 将'esp'文件夹中的'EFI'文件夹和'FlameCore.elf'文件复制到U盘中。
- 用U盘启动你的真机。
- 之后你将看到MythOS的屏幕。
---
## 代码行数
- 总计 : 36 文件,  6442 代码, 1339 注释, 428 空行, 共 8209 行
---
