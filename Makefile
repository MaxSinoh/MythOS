# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#           /======================\\`
#           | /==================\`||;
#           | |        ||        |;||;
#           | |   ,,   ||   ,,   |;||;
#           | |   ||   ||   ||   |;||;
#           | |   ||   ||   ||   |;||;
#           | |   ||   ``   ||   |;||;
#           | |   ||        ||   |;||;
#           | \––––––––––––––––––/´||;
#           \––––––––––––––––––––––//,
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#       COPYRIGHT (c) 2025 MYTHOS PROJECT
#              ALL RIGHTS RESERVED
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SHELL = C:\Windows\System32\cmd.exe

# 工具
GCC = gcc
ELF_GCC = x86_64-elf-gcc
LD = x86_64-elf-ld
QEMU = qemu-system-x86_64

# 构建模式
BUILD_MODE ?= debug
ifeq ($(BUILD_MODE),release)
	OPTIMIZATION = -O3
	DEBUG_FLAGS = 
else
	OPTIMIZATION = -O0
	DEBUG_FLAGS = -g
endif

# 处理Windows控制台编码问题
ifeq ($(OS),Windows_NT)
	CHCP = chcp 65001 > nul
else
	CHCP = :
endif

# 编译和链接标志
GCC_FLAGS = -I includes -nostdlib -nostdinc -fno-builtin -Wl,--subsystem,10 -e $(ENTRY_POINT) -o
ELF_GCC_BASE_FLAGS = -I includes -Wall -Werror -ffreestanding -fno-exceptions -std=c99
ELF_GCC_FLAGS = $(ELF_GCC_BASE_FLAGS) $(OPTIMIZATION) $(DEBUG_FLAGS) -MMD -MP -c -o
LD_FLAGS = -e $(LD_ENTRY) -z norelro -Ttext-segment 0x100000 --static -o
QEMU_FLAGS = -bios ./OVMF.fd -net none -drive file=fat:rw:$(ESP_DIR),index=0,format=vvfat

# 入口点
ENTRY_POINT = entryPoint
LD_ENTRY = FlameCoreMain

# 目录
SRC_DIR = FlameCore
OBJ_DIR = build/obj
BIN_DIR = bin
ESP_DIR = esp

# BootLoader
BOOT_LOADER_SRC = BootLoader/BootLoader.c
BOOT_LOADER_EFI = $(BIN_DIR)\BootLoader.efi

# 源文件和对象文件
FLAMECORE_SRCS = $(wildcard $(SRC_DIR)/*.c) \
                 $(wildcard $(SRC_DIR)/*/*.c) \
                 $(wildcard $(SRC_DIR)/*/*/*.c)

FLAMECORE_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(FLAMECORE_SRCS))
FLAMECORE_DEPS = $(FLAMECORE_OBJS:.o=.d)

FLAMECORE_ELF = $(BIN_DIR)\FlameCore.elf

# ESP路径
ESP_BOOTLOADER = $(ESP_DIR)\EFI\BOOT\BOOTX64.EFI
ESP_FLAMECORE = $(ESP_DIR)\FlameCore.elf

# 默认目标
all: info $(BIN_DIR) $(OBJ_DIR) $(ESP_DIR) efi objects link

# 目录创建
$(BIN_DIR):
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	@if not exist "$(BIN_DIR)/EFI" mkdir "$(BIN_DIR)/EFI"
	@if not exist "$(BIN_DIR)/EFI/BOOT" mkdir "$(BIN_DIR)/EFI/BOOT"

$(OBJ_DIR):
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)" mkdir "$(OBJ_DIR)/$(SRC_DIR)"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/asm" mkdir "$(OBJ_DIR)/$(SRC_DIR)/asm"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/asm/gdt" mkdir "$(OBJ_DIR)/$(SRC_DIR)/asm/gdt"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/asm/hal" mkdir "$(OBJ_DIR)/$(SRC_DIR)/asm/hal"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/asm/idt" mkdir "$(OBJ_DIR)/$(SRC_DIR)/asm/idt"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/gui" mkdir "$(OBJ_DIR)/$(SRC_DIR)/gui"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/gui/console" mkdir "$(OBJ_DIR)/$(SRC_DIR)/gui/console"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/gui/graphic" mkdir "$(OBJ_DIR)/$(SRC_DIR)/gui/graphic"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/gui/view" mkdir "$(OBJ_DIR)/$(SRC_DIR)/gui/view"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/mem" mkdir "$(OBJ_DIR)/$(SRC_DIR)/mem"
	@if not exist "$(OBJ_DIR)/$(SRC_DIR)/std" mkdir "$(OBJ_DIR)/$(SRC_DIR)/std"

$(ESP_DIR):
	@if not exist "$(ESP_DIR)" mkdir "$(ESP_DIR)"
	@if not exist "$(ESP_DIR)/EFI" mkdir "$(ESP_DIR)/EFI"
	@if not exist "$(ESP_DIR)/EFI/BOOT" mkdir "$(ESP_DIR)/EFI/BOOT"

info:
	@echo 欢迎来到 MythOS 的构建系统！
	@echo 构建模式：$(BUILD_MODE)
	@echo 正在准备构建环境...

clean:
	@echo 正在清理构建产物...
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(ESP_DIR)" rmdir /s /q "$(ESP_DIR)"
	@echo 清理完成

efi: $(BIN_DIR) $(BOOT_LOADER_EFI)

$(BOOT_LOADER_EFI): $(BOOT_LOADER_SRC)
	@echo 正在编译BootLoader...
	@$(GCC) $(GCC_FLAGS) $@ $<
	@echo 编译完成

objects: $(FLAMECORE_OBJS)

# 编译源文件为对象文件，并生成依赖文件
$(OBJ_DIR)/%.o: %.c
	@echo 正在编译$<...
	@if not exist "$(@D)" mkdir "$(@D)"
	@$(ELF_GCC) $(ELF_GCC_FLAGS) $@ $<
	@echo $< 编译完成

# 包含依赖文件，确保在源文件更改时重新编译相关对象文件
-include $(FLAMECORE_DEPS)

link: $(FLAMECORE_ELF)

$(FLAMECORE_ELF): $(FLAMECORE_OBJS)
	@echo 正在链接...
	@$(LD) $(LD_FLAGS) $@ $^
	@echo 链接完成

install: $(FLAMECORE_ELF) $(BOOT_LOADER_EFI)
	@echo 正在复制BootLoader和内核文件...
	@copy $(BOOT_LOADER_EFI) $(ESP_BOOTLOADER)
	@copy $(FLAMECORE_ELF) $(ESP_FLAMECORE)
	@echo 安装完成

run: all install
	@echo 正在运行 MythOS 于 QEMU 中...
	@$(QEMU) $(QEMU_FLAGS)

debug: BUILD_MODE = debug
debug: all

release: BUILD_MODE = release
release: all

help:
	@echo 可用的目标:
	@echo   all       - 构建整个项目（默认目标）
	@echo   clean     - 清理构建产物
	@echo   efi       - 仅编译 BootLoader 的 EFI 文件
	@echo   objects   - 仅编译内核的对象文件
	@echo   link      - 仅链接生成内核的 ELF 文件
	@echo   -jn	      - 使用 n 个线程进行并行编译（例如: make -j4）
	@echo   debug     - 构建带有调试符号的版本
	@echo   release   - 构建优化过的版本
	@echo   run       - 构建并运行 MythOS 在 QEMU 中
	@echo   install   - 安装到 ESP 目录但不运行
	@echo   help      - 显示此帮助信息

.PHONY: all clean info efi objects link install run debug release help

# 检查是否找到源文件
ifeq ($(FLAMECORE_SRCS),)
$(warning 没有找到 $(SRC_DIR) 中的源文件)
endif