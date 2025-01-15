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
OBJCP = x86_64-elf-objcopy
LD = x86_64-elf-ld
QEMU = qemu-system-x86_64

GCC_FLAGS = -nostdlib -nostdinc -fno-builtin -Wl,--subsystem,10 -e $(ENTRY_POINT) -o
ELF_GCC_FLAGS = -O2 -Wall -g -ffreestanding -fno-exceptions -std=c99 -c -o
OBJCP_FLAGS = -I binary -O elf64-x86-64 -B i386:x86-64
LD_FLAGS = -e $(LD_ENTRY) -z norelro -Ttext-segment 0x100000 --static -o
QEMU_FLAGS = -bios ./OVMF.fd -net none -drive file=fat:rw:$(ESP),index=0,format=vvfat

ENTRY_POINT = entryPoint
LD_ENTRY = KernelMain

BOOT_LOADER = .\BootLoader\BootLoader.c
BOOT_LOADER_EFI = .\bin\BootLoader.efi
KERNEL = .\kernel\main.c
KERNEL_O = .\kernel\main.o
KERNEL_ELF = .\bin\kernel.elf
GRAPHICS = .\kernel\gui\graphics.c
GRAPHICS_O = .\kernel\gui\graphics.o

ESP = .\esp
ESP_BOOTLOADER = $(ESP)\EFI\BOOT\BOOTX64.EFI
ESP_KERNEL = $(ESP)\kernel.elf

all: info objects efi link done

clean:
	@echo Cleaning...
	@del $(BOOT_LOADER_EFI)
	@del $(KERNEL_ELF)
	@del $(KERNEL_O)
	@del $(GRAPHICS_O)
	@echo Cleaned.

info:
	@echo Welcome to compile MythOS.
	@echo Preparing for compiling...

efi:
	@echo Compiling BootLoader...
	@$(GCC) $(GCC_FLAGS) $(BOOT_LOADER_EFI) $(BOOT_LOADER)
	@echo Compiled BootLoader.

objects:
	@echo Compiling kernel...
	@$(ELF_GCC) $(KERNEL) $(ELF_GCC_FLAGS) $(KERNEL_O)
	@echo Compiled kernel.
	@echo Compiling graphics...
	@$(ELF_GCC) $(GRAPHICS) $(ELF_GCC_FLAGS) $(GRAPHICS_O)
	@echo Compiled graphics.

link:
	@echo Linking...
	@$(LD) $(LD_FLAGS) $(KERNEL_ELF) $(KERNEL_O) $(GRAPHICS_O)
	@echo Linked.

done:
	@echo Compiled MythOS.
	@echo Done.

run: all
	@echo Running MythOS in qemu virtual machine...
	@copy .\bin\BootLoader.efi .\esp\EFI\BOOT\bootx64.efi
	@copy .\bin\kernel.elf .\esp\kernel.elf
	@$(QEMU) $(QEMU_FLAGS)