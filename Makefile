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

# Toolchain
GCC = gcc
ELF_GCC = x86_64-elf-gcc
LD = x86_64-elf-ld
QEMU = qemu-system-x86_64

# Build configuration
BUILD_MODE ?= debug
ifeq ($(BUILD_MODE),release)
	OPTIMIZATION = -O3
	DEBUG_FLAGS = 
else
	OPTIMIZATION = -O0
	DEBUG_FLAGS = -g
endif

# Flags
GCC_FLAGS = -I includes -nostdlib -nostdinc -fno-builtin -Wl,--subsystem,10 -e $(ENTRY_POINT) -o
ELF_GCC_BASE_FLAGS = -I includes -Wall -Werror -ffreestanding -fno-exceptions -std=c99
ELF_GCC_FLAGS = $(ELF_GCC_BASE_FLAGS) $(OPTIMIZATION) $(DEBUG_FLAGS) -MMD -MP -c -o
LD_FLAGS = -e $(LD_ENTRY) -z norelro -Ttext-segment 0x100000 --static -o
QEMU_FLAGS = -bios ./OVMF.fd -net none -drive file=fat:rw:$(ESP_DIR),index=0,format=vvfat

# Entry points
ENTRY_POINT = entryPoint
LD_ENTRY = FlameCoreMain

# Directories
SRC_DIR = FlameCore
OBJ_DIR = build/obj
BIN_DIR = bin
ESP_DIR = esp

# Source files
BOOT_LOADER_SRC = BootLoader/BootLoader.c
BOOT_LOADER_EFI = $(BIN_DIR)\BootLoader.efi

# Auto-discover FlameCore source files
FLAMECORE_SRCS = $(wildcard $(SRC_DIR)/*.c) \
                 $(wildcard $(SRC_DIR)/*/*.c) \
                 $(wildcard $(SRC_DIR)/*/*/*.c)

FLAMECORE_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(FLAMECORE_SRCS))
FLAMECORE_DEPS = $(FLAMECORE_OBJS:.o=.d)

FLAMECORE_ELF = $(BIN_DIR)\FlameCore.elf

# ESP paths
ESP_BOOTLOADER = $(ESP_DIR)\EFI\BOOT\BOOTX64.EFI
ESP_FLAMECORE = $(ESP_DIR)\FlameCore.elf

# Default target
all: info $(BIN_DIR) $(OBJ_DIR) $(ESP_DIR) efi objects link

# Create directories
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
	@echo Welcome to compile MythOS.
	@echo Build mode: $(BUILD_MODE)
	@echo Preparing for compiling...

clean:
	@echo Cleaning...
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(ESP_DIR)" rmdir /s /q "$(ESP_DIR)"
	@echo Cleaned.

efi: $(BIN_DIR) $(BOOT_LOADER_EFI)

$(BOOT_LOADER_EFI): $(BOOT_LOADER_SRC)
	@echo Compiling BootLoader...
	@$(GCC) $(GCC_FLAGS) $@ $<
	@echo BootLoader compiled.

objects: $(FLAMECORE_OBJS)

# Pattern rule for compiling .c files
$(OBJ_DIR)/%.o: %.c
	@echo Compiling $<...
	@if not exist "$(@D)" mkdir "$(@D)"
	@$(ELF_GCC) $(ELF_GCC_FLAGS) $@ $<
	@echo $< compiled.

# Include dependency files
-include $(FLAMECORE_DEPS)

link: $(FLAMECORE_ELF)

$(FLAMECORE_ELF): $(FLAMECORE_OBJS)
	@echo Linking...
	@$(LD) $(LD_FLAGS) $@ $^
	@echo Linking done.

install: $(FLAMECORE_ELF) $(BOOT_LOADER_EFI)
	@echo Copying BootLoader and kernel...
	@copy $(BOOT_LOADER_EFI) $(ESP_BOOTLOADER)
	@copy $(FLAMECORE_ELF) $(ESP_FLAMECORE)
	@echo Installation done.

run: all install
	@echo Running MythOS in QEMU virtual machine...
	@$(QEMU) $(QEMU_FLAGS)

debug: BUILD_MODE = debug
debug: all

release: BUILD_MODE = release
release: all

help:
	@echo Available targets:
	@echo   all       - Build everything (default)
	@echo   clean     - Clean build artifacts
	@echo   debug     - Build with debug symbols
	@echo   release   - Build with optimizations
	@echo   run       - Build and run in QEMU
	@echo   install   - Install to ESP directory
	@echo   help      - Show this help

.PHONY: all clean info efi objects link install run debug release help

# Show warning if no source files found
ifeq ($(FLAMECORE_SRCS),)
$(warning No source files found in $(SRC_DIR))
endif