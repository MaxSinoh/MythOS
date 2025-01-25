#
#                       _oo0oo_
#                      o8888888o
#                      88' . '88
#                      (| -_- |)
#                      0\  =  /0
#                    ___/`---'\___
#                  .' \|     |// '.
#                 / \|||  :  |||// "
#                / _||||| -:- |||||- "
#               |   | \\  -  /// |   |
#               | \_|  ''\---/''  |_/ |
#               \  .-\__  '-'  ___/-. /
#             ___'. .'  /--.--\  `. .'___
#          .''' '<  `.___\_<|>_/___.' >' '''.
#          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
#          \  \ `_.   \_ __\ /__ _/   .-` /  /
#      =====`-.____`.___ \_____/___.-`___.-'=====
#                        `=---='
#
#      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#             ***GOD BLESS   ·   NEVER CRASH***
#
#      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#        COPYRIGHT (C) 2025 MAXSINOH & LUOYUDIAN
#                  ALL RIGHTS RESERVED
#
#      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#                  MYTHOS MAKEFILE
#
#      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

GCC = gcc
ELF_GCC = x86_64-elf-gcc
LD = x86_64-elf-ld
QEMU = qemu-system-x86_64

GCC_FLAGS = -I includes -nostdlib -nostdinc -fno-builtin -Wl,--subsystem,10 -e $(ENTRY_POINT) -o
ELF_GCC_FLAGS = -I includes -O2 -Wall -g -ffreestanding -fno-exceptions -std=c99 -c -o
LD_FLAGS = -e $(LD_ENTRY) -z norelro -Ttext-segment 0x100000 --static -o
QEMU_FLAGS = -bios ./OVMF.fd -net none -drive file=fat:rw:$(ESP),index=0,format=vvfat

ENTRY_POINT = entryPoint
LD_ENTRY = kernelMain

BOOT_LOADER = .\BootLoader\BootLoader.c
BOOT_LOADER_EFI = .\bin\BootLoader.efi
KERNEL = .\kernel\main.c
KERNEL_O = .\kernel\main.o
KERNEL_ELF = .\bin\kernel.elf
GRAPHICS = .\kernel\gui\graphics.c
GRAPHICS_O = .\kernel\gui\graphics.o
FONT = .\kernel\gui\font.c
FONT_O = .\kernel\gui\font.o
COLOR = .\kernel\gui\color.c
COLOR_O = .\kernel\gui\color.o
BMP = .\kernel\view\bmp.c
BMP_O = .\kernel\view\bmp.o
CONSOLE = .\kernel\console\console.c
CONSOLE_O = .\kernel\console\console.o
STRING = .\kernel\string\string.c
STRING_O = .\kernel\string\string.o
STDLIB = .\kernel\std\stdlib.c
STDLIB_O = .\kernel\std\stdlib.o
STDIO = .\kernel\std\stdio.c
STDIO_O = .\kernel\std\stdio.o

ESP = .\esp
ESP_BOOTLOADER = $(ESP)\EFI\BOOT\BOOTX64.EFI
ESP_KERNEL = $(ESP)\kernel.elf

all: info clean objects efi link done

clean:
	@echo Cleaning...
	@del $(BOOT_LOADER_EFI)
	@del $(KERNEL_ELF)
	@del $(KERNEL_O)
	@del $(GRAPHICS_O)
	@del $(FONT_O)
	@del $(COLOR_O)
	@del $(BMP_O)
	@del $(CONSOLE_O)
	@del $(STRING_O)
	@del $(STDLIB_O)
	@del $(STDIO_O)
	@echo Cleaned.

info:
	@echo Welcome to compile MythOS.
	@echo Preparing for compiling...

efi:
	@echo Compiling BootLoader...
	@$(GCC) $(GCC_FLAGS) $(BOOT_LOADER_EFI) $(BOOT_LOADER)
	@echo Done.

objects:
	@echo Compiling kernel...
	@$(ELF_GCC) $(KERNEL) $(ELF_GCC_FLAGS) $(KERNEL_O)
	@echo Done.
	@echo Compiling font...
	@$(ELF_GCC) $(FONT) $(ELF_GCC_FLAGS) $(FONT_O)
	@echo Done.
	@echo Compiling graphics...
	@$(ELF_GCC) $(GRAPHICS) $(ELF_GCC_FLAGS) $(GRAPHICS_O)
	@echo Done.
	@echo Compiling color...
	@$(ELF_GCC) $(COLOR) $(ELF_GCC_FLAGS) $(COLOR_O)
	@echo Done.
	@echo Compiling bmp...
	@$(ELF_GCC) $(BMP) $(ELF_GCC_FLAGS) $(BMP_O)
	@echo Done.
	@echo Compiling console...
	@$(ELF_GCC) $(CONSOLE) $(ELF_GCC_FLAGS) $(CONSOLE_O)
	@echo Done.
	@echo Compiling string...
	@$(ELF_GCC) $(STRING) $(ELF_GCC_FLAGS) $(STRING_O)
	@echo Done.
	@echo Compiling stdlib...
	@$(ELF_GCC) $(STDLIB) $(ELF_GCC_FLAGS) $(STDLIB_O)
	@echo Done.
	@echo Compiling stdio...
	@$(ELF_GCC) $(STDIO) $(ELF_GCC_FLAGS) $(STDIO_O)
	@echo Done.

link:
	@echo Linking...
	@$(LD) $(LD_FLAGS) $(KERNEL_ELF) $(KERNEL_O) $(GRAPHICS_O) $(FONT_O) $(COLOR_O) $(BMP_O) $(CONSOLE_O) $(STRING_O) $(STDLIB_O) $(STDIO_O)
	@echo Done.

done:
	@echo All done.

run: all
	@echo Copying BootLoader and kernel...
	@copy .\bin\BootLoader.efi .\esp\EFI\BOOT\bootx64.efi
	@copy .\bin\kernel.elf .\esp\kernel.elf
	@echo Done.
	@echo Running MythOS in QEMU virtual machine...
	@$(QEMU) $(QEMU_FLAGS)