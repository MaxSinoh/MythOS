//
//                       _oo0oo_
//                      o8888888o
//                      88' . '88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \|     |// '.
//                 / \|||  :  |||// "
//                / _||||| -:- |||||- "
//               |   | \\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          .''' '<  `.___\_<|>_/___.' >' '''.
//          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//          \  \ `_.   \_ __\ /__ _/   .-` /  /
//      =====`-.____`.___ \_____/___.-`___.-'=====
//                        `=---='
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//             ***GOD BLESS   ·   NEVER CRASH***
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//        COPYRIGHT (C) 2025 MAXSINOH & LUOYUDIAN
//                  ALL RIGHTS RESERVED
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//                  MYTHOS BOOTLOADER
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "../includes/firmware/uefi.h"

struct EFI_SYSTEM_TABLE *ST;
struct EFI_BOOT_SERVICES *BS;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
struct EFI_LOADED_IMAGE_PROTOCOL *LIP;
EFI_HANDLE IM;

struct EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
struct EFI_GUID lip_guid = {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
struct EFI_GUID sfsp_guid = {0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))


void putc(unsigned short c)
{
    unsigned short str[2] = L" ";
    str[0] = c;
    ST->ConOut->OutputString(ST->ConOut, str);
}

void puts(unsigned short *s)
{
    ST->ConOut->OutputString(ST->ConOut, s);
}

void puth(unsigned long long hex)
{
    unsigned short buf[20];
    unsigned short *p = buf;
    unsigned short ch;
    int i, flag = 0;

    *p++ = L'0';
    *p++ = L'x'; 

    if (hex == 0) *p++ = L'0'; 
    else {
        for (i = 28; i >= 0; i -= 4) { 
            ch = (hex >> i) & 0xF; 
            if (flag || ch > 0) {
                flag = 1; 
                ch += L'0'; // 0~9 => '0'~'9'
                if (ch > L'9') {
                    ch += 7; // 'A' - '9' = 7
                }
                *p++ = ch;
            }
        }
    }
    *p = L'\0';
    puts(buf); 
}

void memset(void *dst_, UINT8 value, UINTN size)
{
    UINT8 *dst = (UINT8 *) dst_;
    while (size-- > 0) *dst++ = value;
}

void *memcpy(void *dst_, const void *src_, UINTN size)
{
    UINT8 *dst = dst_;
    const UINT8 *src = src_;
    while (size-- > 0) *dst++ = *src++;
    return (void *) src_;
}

void *malloc(int buf_size)
{
    void *res;
    unsigned long long status;

    status = BS->AllocatePool(LIP ? LIP->ImageDataType : EfiLoaderData, buf_size, &res); 
    if (status < 0) return NULL;

    return res;
}

void free(void *buf)
{
    BS->FreePool(buf);
}

EFI_STATUS mallocAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    return BS->AllocatePages(AllocateAddress, LIP ? LIP->ImageDataType : EfiLoaderData, (size + 0xfff) / 0x1000, &addr);
}

void freeAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    BS->FreePages(addr, (size + 0xfff) / 0x1000);
}

void CalcLoadAddressRange(Elf64_Ehdr *ehdr, UINT64 *first, UINT64 *last)
{
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((UINT64) ehdr + ehdr->e_phoff); // The first program header address.
    *first = 0xffffffffffffffff; // UINT64 maximum value
    *last = 0; // UINT64 minimum value
    for (UINT16 i = 0; i < ehdr->e_phnum; i++) { // Traverse each program header.
        if (phdr[i].p_type != PT_LOAD) continue; // Only care about the LOAD segment
        *first = min(*first, phdr[i].p_vaddr);
        *last = max(*last, phdr[i].p_vaddr + phdr[i].p_memsz); // The beginning and end of each program header take the maximum value.
    }
}

void CopyLoadSegments(Elf64_Ehdr *ehdr)
{
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((UINT64) ehdr + ehdr->e_phoff); // The first program header address.
    for (UINT16 i = 0; i < ehdr->e_phnum; i++) { // Traverse each program header.
        if (phdr[i].p_type != PT_LOAD) continue; // Only care about the LOAD segment
        UINT64 segm_in_file = (UINT64) ehdr + phdr[i].p_offset; // The position of the segment in the file
        memcpy((VOID *) phdr[i].p_vaddr, (VOID *) segm_in_file, phdr[i].p_filesz); // Copy the size part of the file.
        UINTN remain_bytes = phdr[i].p_memsz - phdr[i].p_filesz; // The difference between the two
        memset((VOID *) (phdr[i].p_vaddr + phdr[i].p_filesz), 0, remain_bytes); // Assign a value of 0
    }
}

void efi_init(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
    ST = SystemTable;
    BS = SystemTable->BootServices;
    IM = ImageHandle;

    BS->SetWatchdogTimer(0, 0, 0, NULL); 
    BS->LocateProtocol(&gop_guid, NULL, (void **) &GOP);

    BS->OpenProtocol(ImageHandle, &lip_guid, (void **) &LIP, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL); 
    BS->OpenProtocol(LIP->DeviceHandle, &sfsp_guid, (void **) &SFSP, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

EFI_STATUS GetMMP(MEMORY_MAP *MemoryMap) {
    EFI_STATUS GetMemoryMapStatus = EFI_SUCCESS;
    MemoryMap->Buffer = malloc(MemoryMap->MapSize);
    while(  BS->GetMemoryMap(
                &MemoryMap->MapSize,
                (EFI_MEMORY_DESCRIPTOR *)MemoryMap->Buffer,
                &MemoryMap->MapKey,
                &MemoryMap->DescriptorSize,
                &MemoryMap->DescriptorVersion
            ) == EFI_BUFFER_TOO_SMALL) 
    {
        if (MemoryMap->Buffer) {
            free(MemoryMap->Buffer);
            MemoryMap->Buffer = NULL;
        }
        MemoryMap->Buffer = malloc(MemoryMap->MapSize);
    }
    if (!MemoryMap->Buffer) {
        GetMemoryMapStatus = EFI_OUT_OF_RESOURCES;
    }
    return GetMemoryMapStatus;
}

EFI_STATUS entryPoint(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
    efi_init(ImageHandle, SystemTable);

    ST->ConOut->ClearScreen(ST->ConOut);

    EFI_STATUS status;
    EFI_PHYSICAL_ADDRESS entry_addr;
    struct EFI_FILE_PROTOCOL *root, *kernel_file;
    UINTN kernel_size = 4194304;
    void *kernel_buffer = malloc(kernel_size);

    status = SFSP->OpenVolume(SFSP, &root);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while opening root dir");
        while (1);
    }
    status = root->Open(root, &kernel_file, L"\\kernel.elf", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while opening kernel file");
        while (1);
    }
    status = kernel_file->Read(kernel_file, &kernel_size, kernel_buffer);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while reading kernel file");
        while (1);
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) kernel_buffer;
    UINT64 kernel_first_addr, kernel_last_addr;
    CalcLoadAddressRange(ehdr, &kernel_first_addr, &kernel_last_addr);

    status = mallocAt(kernel_first_addr, kernel_last_addr - kernel_first_addr);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while allocating buffer for kernel");
        while (1);
    }

    CopyLoadSegments(ehdr);
    entry_addr = ehdr->e_entry;
    free(kernel_buffer);

    struct FrameBufferConfig config = {
        (UINT8 *) GOP->Mode->FrameBufferBase,
        GOP->Mode->Info->PixelsPerScanLine,
        GOP->Mode->Info->HorizontalResolution,
        GOP->Mode->Info->VerticalResolution
    };

    switch (GOP->Mode->Info->PixelFormat) {
        case PixelRedGreenBlueReserved8BitPerColor:
            config.pixel_format = kRGBR;
            break;
        case PixelBlueGreenRedReserved8BitPerColor:
            config.pixel_format = kBGRR;
            break;
        default:
            puts(L"FAILED: unsupported pixel format: ");
            puth(GOP->Mode->Info->PixelFormat);
            while (1);
    }

    BOOT_CONFIG BootConfig;
    BootConfig.MemoryMap.MapSize = 4096;
    BootConfig.MemoryMap.Buffer = NULL;
    BootConfig.MemoryMap.MapKey = 0;
    BootConfig.MemoryMap.DescriptorSize = 0;
    BootConfig.MemoryMap.DescriptorVersion = 0;

    GetMMP(&BootConfig.MemoryMap);

    typedef void (* __attribute__((sysv_abi)) Kernel)(const struct FrameBufferConfig *);
    Kernel kernel = (Kernel) entry_addr;
    kernel(&config); // Jump into kernel!
    while (1);
}