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

GCC = gcc
ELF_GCC = x86_64-elf-gcc
LD = x86_64-elf-ld
QEMU = qemu-system-x86_64

GCC_FLAGS = -I includes -nostdlib -nostdinc -fno-builtin -Wl,--subsystem,10 -e $(ENTRY_POINT) -o
ELF_GCC_FLAGS = -I includes -O2 -Wall -g -ffreestanding -fno-exceptions -std=c99 -c -o
LD_FLAGS = -e $(LD_ENTRY) -z norelro -Ttext-segment 0x100000 --static -o
QEMU_FLAGS = -bios ./OVMF.fd -net none -drive file=fat:rw:$(ESP),index=0,format=vvfat

ENTRY_POINT = entryPoint
LD_ENTRY = FlameCoreMain

BOOT_LOADER = .\BootLoader\BootLoader.c
BOOT_LOADER_EFI = .\bin\BootLoader.efi
FLAMECORE = .\FlameCore\main.c
FLAMECORE_O = .\FlameCore\main.o
FLAMECORE_ELF = .\bin\FlameCore.elf
GRAPHICS = .\FlameCore\gui\graphic\graphics.c
GRAPHICS_O = .\FlameCore\gui\graphic\graphics.o
FONT = .\FlameCore\gui\graphic\font.c
FONT_O = .\FlameCore\gui\graphic\font.o
COLOR = .\FlameCore\gui\graphic\color.c
COLOR_O = .\FlameCore\gui\graphic\color.o
BMP = .\FlameCore\gui\view\bmp.c
BMP_O = .\FlameCore\gui\view\bmp.o
CONSOLE = .\FlameCore\gui\console\console.c
CONSOLE_O = .\FlameCore\gui\console\console.o
STRING = .\FlameCore\std\string.c
STRING_O = .\FlameCore\std\string.o
STDLIB = .\FlameCore\std\stdlib.c
STDLIB_O = .\FlameCore\std\stdlib.o
STDIO = .\FlameCore\std\stdio.c
STDIO_O = .\FlameCore\std\stdio.o
IO = .\FlameCore\asm\hal\io.c
IO_O = .\FlameCore\asm\hal\io.o
GDT = .\FlameCore\asm\gdt\gdt.c
GDT_O = .\FlameCore\asm\gdt\gdt.o

ESP = .\esp
ESP_BOOTLOADER = $(ESP)\EFI\BOOT\BOOTX64.EFI
ESP_FLAMECORE = $(ESP)\FlameCore.elf

all: info clean objects efi link

clean:
	@echo Cleaning...
	@del .\\bin
	@del .\\esp
	@echo Cleaned.

info:
	@echo Welcome to compile MythOS.
	@echo Preparing for compiling...
	-@mkdir .\\bin\\EFI\\BOOT
	-@mkdir .\\esp\\EFI\\BOOT

efi:
	@echo Compiling BootLoader...
	@$(GCC) $(GCC_FLAGS) $(BOOT_LOADER_EFI) $(BOOT_LOADER)
	@echo Done.

objects:
	@echo Compiling FLameCore...
	@$(ELF_GCC) $(FLAMECORE) $(ELF_GCC_FLAGS) $(FLAMECORE_O)
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
	@echo Compiling io...
	@$(ELF_GCC) $(IO) $(ELF_GCC_FLAGS) $(IO_O)
	@echo Done.
	@echo Compiling gdt...
	@$(ELF_GCC) $(GDT) $(ELF_GCC_FLAGS) $(GDT_O)
	@echo Done.

link:
	@echo Linking...
	@$(LD) $(LD_FLAGS) $(FLAMECORE_ELF) $(FLAMECORE_O) $(GRAPHICS_O) $(FONT_O) $(COLOR_O) $(BMP_O) $(CONSOLE_O) $(STRING_O) $(STDLIB_O) $(STDIO_O) $(IO_O) $(GDT_O)
	@echo Done.
	@echo Copying BootLoader and kernel...
	@copy .\bin\BootLoader.efi ESP_BOOTLOADER
	@copy .\bin\FlameCore.elf ESP_FLAMECORE
	@del .\\bin
	@echo Done.

done:
	@echo All done.

run: all done qemu

qemu:
	@echo Running MythOS in QEMU virtual machine...
	@$(QEMU) $(QEMU_FLAGS)