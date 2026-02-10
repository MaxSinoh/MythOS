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

#include <type.h>
#include <uefi.h>
#include <fbc.h>


// 定义全局变量
struct EFI_SYSTEM_TABLE *ST;
struct EFI_BOOT_SERVICES *BS;
EFI_RUNTIME_SERVICES *RS;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
struct EFI_LOADED_IMAGE_PROTOCOL *LIP;

EFI_HANDLE IM;      // 当前镜像的句柄

// GOP协议GUID
struct EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
// LIP协议GUID
struct EFI_GUID lip_guid = {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
// SFSP协议GUID
struct EFI_GUID sfsp_guid = {0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

// 输出单个字符到控制台
void putc(unsigned short c)
{
    unsigned short str[2] = L" ";
    str[0] = c;
    ST->ConOut->OutputString(ST->ConOut, str);
}
// 输出字符串到控制台
void puts(const CHAR16 *s)
{
    ST->ConOut->OutputString(ST->ConOut, (CHAR16*)s);
}
// 输出十六进制数到控制台
void puth(unsigned long long hex)
{
    unsigned short buf[20];
    unsigned short *p = buf;
    unsigned short ch;
    int i, flag = 0;
    *p++ = L'0';
    *p++ = L'x'; 
    if (hex == 0) *p++ = L'0'; 
    else
    {
        for (i = 28; i >= 0; i -= 4)
        { 
            ch = (hex >> i) & 0xF; 
            if (flag || ch > 0)
            {
                flag = 1; 
                ch += L'0';
                if (ch > L'9')
                {
                    ch += 7;
                }
                *p++ = ch;
            }
        }
    }
    *p = L'\0';
    puts(buf); 
}

// 内存操作函数
void memset(void *dst_, UINT8 value, UINTN size)
{
    UINT8 *dst = (UINT8 *) dst_;
    while (size-- > 0) 
    {
        *dst++ = value;
    }
}
// 内存复制函数
void *memcpy(void *dst_, const void *src_, UINTN size)
{
    UINT8 *dst = dst_;
    const UINT8 *src = src_;
    while (size-- > 0) 
    {
        *dst++ = *src++;
    }
    return (void *) src_;
}
// 内存比较函数
void *malloc(int buf_size)
{
    void *res;
    unsigned long long status;
    status = BS->AllocatePool(LIP ? LIP->ImageDataType : EfiLoaderData, buf_size, &res); 
    if (status < 0) return NULL;
    return res;
}
// 释放内存
void free(void *buf)
{
    BS->FreePool(buf);
}
// 在指定物理地址分配内存
EFI_STATUS mallocAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 使用BS（Boot Services）的AllocatePages函数分配内存页面
    return BS->AllocatePages(AllocateAddress, LIP ? LIP->ImageDataType : EfiLoaderData, (size + 0xfff) / 0x1000, &addr);
}
// 在指定物理地址释放内存
void freeAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 使用BS（Boot Services）的FreePages函数释放内存页面
    BS->FreePages(addr, (size + 0xfff) / 0x1000);
}
// 安全输出错误信息并重启系统
void safeHaltWithMessage(const CHAR16 *message)
{
    puts(message);
    puts(L"\nSystem halted. Press any key to reboot.");
    // 等待按键
    struct EFI_INPUT_KEY key;
    ST->ConIn->ReadKeyStroke(ST->ConIn, &key);
    // 系统重启
    ST->RuntimeServices->EFI_RESET_SYSTEM(EfiResetCold, EFI_SUCCESS, 0, NULL);
}
// 安全打开根目录
EFI_STATUS safeOpenRoot(struct EFI_FILE_PROTOCOL **root)
{
    EFI_STATUS status = SFSP->OpenVolume(SFSP, root);
    if (EFI_ERROR(status)) {
        puts(L"ERROR: Failed to open root directory");
    }
    return status;
}
// 安全打开内核文件
EFI_STATUS safeOpenKernel(struct EFI_FILE_PROTOCOL *root, struct EFI_FILE_PROTOCOL **kernel_file)
{
    // 尝试主路径
    EFI_STATUS status = root->Open(root, kernel_file, L"\\FlameCore.elf", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status))
    {
        puts(L"WARNING: Failed to open kernel from root, trying EFI path...");
        // 尝试备用路径
        status = root->Open(root, kernel_file, L"\\EFI\\BOOT\\FlameCore.elf", EFI_FILE_MODE_READ, 0);
        if (EFI_ERROR(status))
        {
            puts(L"ERROR: Failed to open kernel from alternate path");
        }
    }
    return status;
}
// 安全读取内核文件
void CalcLoadAddressRange(Elf64_Ehdr *ehdr, UINT64 *first, UINT64 *last)
{
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((UINT64) ehdr + ehdr->e_phoff); // 获取程序头表指针
    *first = 0xffffffffffffffff;    // 初始化为最大值
    *last = 0;                      // 初始化为最小值
    for (UINT16 i = 0; i < ehdr->e_phnum; i++)
    {
        if (phdr[i].p_type != PT_LOAD) continue;    // 只考虑可加载段
        *first = min(*first, phdr[i].p_vaddr);      // 更新起始地址
        *last = max(*last, phdr[i].p_vaddr + phdr[i].p_memsz);
    }
}
// 安全读取内核文件
void CopyLoadSegments(Elf64_Ehdr *ehdr)
{
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((UINT64) ehdr + ehdr->e_phoff);
    for (UINT16 i = 0; i < ehdr->e_phnum; i++)
    {
        if (phdr[i].p_type != PT_LOAD) continue;    // 只考虑可加载段
        UINT64 segm_in_file = (UINT64) ehdr + phdr[i].p_offset;  // 计算段在文件中的位置
        memcpy((VOID *) phdr[i].p_vaddr, (VOID *) segm_in_file, phdr[i].p_filesz); // 将段内容复制到内存中指定的虚拟地址
        UINTN remain_bytes = phdr[i].p_memsz - phdr[i].p_filesz; // 计算段在内存中剩余的大小
        memset((VOID *) (phdr[i].p_vaddr + phdr[i].p_filesz), 0, remain_bytes); // 将剩余部分清零
    }
}

// 初始化EFI环境
void efi_init(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
    ST = SystemTable;
    BS = SystemTable->BootServices;
    IM = ImageHandle;
    BS->SetWatchdogTimer(0, 0, 0, NULL);    // 关闭看门狗定时器，防止引导过程中系统重启
    BS->LocateProtocol(&gop_guid, NULL, (void **) &GOP);
    BS->OpenProtocol(ImageHandle, &lip_guid, (void **) &LIP, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL); 
    BS->OpenProtocol(LIP->DeviceHandle, &sfsp_guid, (void **) &SFSP, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}
// 获取内存映射
EFI_STATUS GetMMP(MEMORY_MAP *MemoryMap)
{
    EFI_STATUS GetMemoryMapStatus = EFI_SUCCESS;
    // 为内存映射分配初始缓冲区
    MemoryMap->Buffer = malloc(MemoryMap->MapSize);

    // 循环调用 BS->GetMemoryMap 直到获取到正确的内存映射信息
    while(  BS->GetMemoryMap(
                &MemoryMap->MapSize,
                (EFI_MEMORY_DESCRIPTOR *)MemoryMap->Buffer,
                &MemoryMap->MapKey,
                &MemoryMap->DescriptorSize,
                &MemoryMap->DescriptorVersion
            ) == EFI_BUFFER_TOO_SMALL) 
    {
        // 如果已有缓冲区，则释放它
        if (MemoryMap->Buffer)
        {
            free(MemoryMap->Buffer);
            MemoryMap->Buffer = NULL;
        }
        // 根据新的 MapSize 重新分配缓冲区
        MemoryMap->Buffer = malloc(MemoryMap->MapSize);
    }
    // 如果缓冲区分配失败，则返回资源不足错误
    if (!MemoryMap->Buffer)
    {
        GetMemoryMapStatus = EFI_OUT_OF_RESOURCES;
    }
    return GetMemoryMapStatus;
}
// 退出引导服务
EFI_STATUS exitBootServices(EFI_HANDLE image_handle, MEMORY_MAP *memory_map)
{
    EFI_STATUS status = BS->ExitBootServices(image_handle, memory_map->MapKey);
    if (status != EFI_SUCCESS)
    {
        return status;
    }
    // 释放内存映射缓冲区
    free(memory_map->Buffer);
    return EFI_SUCCESS;
}

// 引导入口点
EFI_STATUS entryPoint(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
    efi_init(ImageHandle, SystemTable);     // 初始化EFI环境
    ST->ConOut->ClearScreen(ST->ConOut);    // 清屏
    EFI_STATUS status;
    EFI_PHYSICAL_ADDRESS entry_addr;
    struct EFI_FILE_PROTOCOL *root, *kernel_file;
    UINTN kernel_size = 4194304;
    void *kernel_buffer = malloc(kernel_size);
    if (!kernel_buffer)
    {
        safeHaltWithMessage(L"CRITICAL: Failed to allocate kernel buffer");
    }
    // 安全打开文件系统
    status = safeOpenRoot(&root);
    if (EFI_ERROR(status))
    {
        safeHaltWithMessage(L"CRITICAL: Cannot access file system");
    }
    // 安全打开内核文件
    status = safeOpenKernel(root, &kernel_file);
    if (EFI_ERROR(status))
    {
        safeHaltWithMessage(L"CRITICAL: Kernel file not found");
    }
    // 读取内核文件内容
    status = kernel_file->Read(kernel_file, &kernel_size, kernel_buffer);
    if (EFI_ERROR(status))
    {
        safeHaltWithMessage(L"CRITICAL: Error while reading kernel file");
    }
    // 解析elf文件头部并计算内核加载地址范围
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) kernel_buffer;
    UINT64 kernel_first_addr, kernel_last_addr;
    CalcLoadAddressRange(ehdr, &kernel_first_addr, &kernel_last_addr);
    // 在指定地址范围分配内存
    status = mallocAt(kernel_first_addr, kernel_last_addr - kernel_first_addr);
    if (EFI_ERROR(status))
    {
        safeHaltWithMessage(L"CRITICAL: Error while allocating buffer for kernel");
    }
    // 复制加载段到指定内存地址
    CopyLoadSegments(ehdr);
    entry_addr = ehdr->e_entry;
    free(kernel_buffer);
    // 配置帧缓冲
    struct FrameBufferConfig config =
    {
        (UINT8 *) GOP->Mode->FrameBufferBase,
        GOP->Mode->Info->PixelsPerScanLine,
        GOP->Mode->Info->HorizontalResolution,
        GOP->Mode->Info->VerticalResolution
    };
    // 根据像素格式设置配置
    switch (GOP->Mode->Info->PixelFormat)
    {
        case PixelRedGreenBlueReserved8BitPerColor:
        {
            config.pixel_format = kRGBR;
            break;
        }
        case PixelBlueGreenRedReserved8BitPerColor:
        {
            config.pixel_format = kBGRR;
            break;
        }
        default:
        {
            puts(L"FAILED: unsupported pixel format: ");
            puth(GOP->Mode->Info->PixelFormat);
            safeHaltWithMessage(L"CRITICAL: Unsupported pixel format");
        }
    }
    
    // 初始化启动配置
    BOOT_CONFIG BootConfig;
    BootConfig.MemoryMap.MapSize = 4096;
    BootConfig.MemoryMap.Buffer = NULL;
    BootConfig.MemoryMap.MapKey = 0;
    BootConfig.MemoryMap.DescriptorSize = 0;
    BootConfig.MemoryMap.DescriptorVersion = 0;
    
    // 获取内存映射
    status = GetMMP(&BootConfig.MemoryMap);
    if (EFI_ERROR(status))
    {
        safeHaltWithMessage(L"CRITICAL: Failed to get memory map");
    }
    // 定义内核函数类型
    typedef void (* __attribute__((sysv_abi)) Kernel)(const struct FrameBufferConfig *, const BOOT_CONFIG *);
    Kernel kernel = (Kernel) entry_addr;
    // 启动内核
    kernel(&config, (BOOT_CONFIG *) &BootConfig);
    // 如果内核返回，说明执行失败
    safeHaltWithMessage(L"CRITICAL: Kernel execution failed");
    
    return EFI_SUCCESS;
}