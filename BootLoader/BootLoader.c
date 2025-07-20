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

// 定义EFI系统表、引导服务和运行时服务结构体指针
struct EFI_SYSTEM_TABLE *ST;
struct EFI_BOOT_SERVICES *BS;
EFI_RUNTIME_SERVICES *RS;
// 定义EFI图形输出协议、简单文件系统协议和加载的镜像协议结构体指针
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
struct EFI_LOADED_IMAGE_PROTOCOL *LIP;

// 定义EFI句柄
EFI_HANDLE IM;

// 定义EFI图形输出协议GUID
struct EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
// 定义EFI加载的镜像协议GUID
struct EFI_GUID lip_guid = {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
// 定义EFI简单文件系统协议GUID
struct EFI_GUID sfsp_guid = {0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

// 定义求最小值的宏
#define min(a, b) ((a) < (b) ? (a) : (b))
// 定义求最大值的宏
#define max(a, b) ((a) < (b) ? (b) : (a))

/**
 * @brief 将一个字符输出到控制台
 * 将给定的无符号短整型字符变量c输出到控制台。
 * @param c 要输出的字符
 */
void putc(unsigned short c)
{
    // 定义一个长度为2的unsigned short数组str，并初始化为L" "
    unsigned short str[2] = L" ";
    // 将传入的字符c赋值给str数组的第一个元素
    str[0] = c;
    // 调用ST->ConOut->OutputString函数，将str数组作为输出字符串
    // ST是一个指向某结构的指针，ConOut是该结构中的一个成员，OutputString是ConOut的一个成员函数
    ST->ConOut->OutputString(ST->ConOut, str);
}

/**
 * @brief 将一个字符串输出到控制台
 * 将指向的字符串通过控制台输出函数输出到控制台。
 * @param s 指向要输出的字符串的指针
 */
void puts(unsigned short *s)
{
    // 调用ST结构体中ConOut成员的OutputString方法，输出字符串s
    // ST->ConOut->OutputString(ST->ConOut, s);
    ST->ConOut->OutputString(ST->ConOut, s);
}

/**
 * @brief 将无符号长整型数字转换为十六进制字符串并输出
 * 将给定的无符号长整型数字转换为十六进制字符串，并以小写形式输出。
 * @param hex 要转换的无符号长整型数字
 */
void puth(unsigned long long hex)
{
    // 定义一个长度为20的unsigned short数组buf
    unsigned short buf[20];
    // 定义一个指向buf的指针p
    unsigned short *p = buf;
    // 定义一个unsigned short变量ch
    unsigned short ch;
    int i, flag = 0;
    // 在buf的开头存入字符'0'
    *p++ = L'0';
    // 在buf中存入字符'x'
    *p++ = L'x'; 
    // 如果hex为0，则在buf中存入字符'0'
    if (hex == 0) *p++ = L'0'; 
    else {
        // 从最高位开始，每4位一组进行处理
        for (i = 28; i >= 0; i -= 4) { 
            // 取当前4位的值存入ch
            ch = (hex >> i) & 0xF; 
            // 如果已经有有效数字或者当前4位大于0
            if (flag || ch > 0) {
                // 设置标志位flag为1
                flag = 1; 
                // 将ch转换为对应的字符
                ch += L'0'; // 0~9 => '0'~'9'
                // 如果ch大于'9'，则转换为'A'~'F'
                if (ch > L'9') {
                    ch += 7; // 'A' - '9' = 7
                }
                // 将转换后的字符存入buf
                *p++ = ch;
            }
        }
    }
    // 在buf的末尾存入字符串结束符'\0'
    *p = L'\0';
    // 输出buf中的内容
    puts(buf); 
}

/**
 * @brief 将指定内存区域中的每个字节设置为指定的值
 * 将给定的内存区域中的每个字节设置为指定的值。内存区域由 dst_ 指针指向，大小为 size 字节。
 * @param dst_ 指向要设置的内存区域的指针
 * @param value 要设置的字节值
 * @param size 要设置的字节数
 */
void memset(void *dst_, UINT8 value, UINTN size)
{
    // 将 void* 类型的指针转换为 UINT8* 类型的指针
    UINT8 *dst = (UINT8 *) dst_;
    // 循环直到 size 为 0
    while (size-- > 0) 
    {
        // 将当前指针指向的内存位置的值设置为 value
        // 然后指针向后移动一位
        *dst++ = value;
    }
}

/**
 * @brief 复制内存块
 * 将源内存块从src_地址开始复制size字节到目标内存块dst_地址开始的位置。
 * @param dst_ 目标内存块的起始地址
 * @param src_ 源内存块的起始地址
 * @param size 要复制的字节数
 * @return 返回源内存块的起始地址
 */
void *memcpy(void *dst_, const void *src_, UINTN size)
{
    // 将 void* 类型的指针转换为 UINT8* 类型，以便按字节复制
    UINT8 *dst = dst_;
    // 将 const void* 类型的指针转换为 const UINT8* 类型，以便按字节复制
    const UINT8 *src = src_;
    // 循环复制每个字节，直到 size 为 0
    while (size-- > 0) 
    {
        // 复制每个字节
        *dst++ = *src++;
    }
    // 返回源指针，通常用于链式调用
    return (void *) src_;
}

/**
 * @brief 动态分配内存
 * 使用 UEFI 提供的内存分配函数来分配指定大小的内存空间。
 * @param buf_size 需要分配的内存大小（以字节为单位）
 * @return 分配的内存地址指针，如果分配失败则返回 NULL
 */
void *malloc(int buf_size)
{
    // 声明一个指向任意类型的指针res
    void *res;
    // 声明一个无符号长整型变量status，用于存储分配池的状态
    unsigned long long status;
    // 调用BS->AllocatePool函数进行内存分配，并将结果存储在status中
    // 如果LIP不为空，则使用LIP->ImageDataType作为分配类型，否则使用EfiLoaderData
    // 分配的内存大小为buf_size，分配的内存地址存储在res中
    status = BS->AllocatePool(LIP ? LIP->ImageDataType : EfiLoaderData, buf_size, &res); 
    // 如果status小于0，表示分配失败，返回NULL
    if (status < 0) return NULL;
    // 返回分配的内存地址
    return res;
}

/**
 * @brief 释放内存
 * 使用内存池释放给定指针所指向的内存。
 * @param buf 需要释放的内存指针
 */
void free(void *buf)
{
    // 调用BS对象的FreePool方法，释放buf指向的内存
    BS->FreePool(buf);
}

/**
 * @brief 在指定的物理地址分配内存
 * 该函数在指定的物理地址分配指定大小的内存。
 *
 * @param addr 指定的物理地址
 * @param size 要分配的内存大小（以字节为单位）
 * @return 如果分配成功，返回EFI_SUCCESS；否则返回相应的错误代码
 */
EFI_STATUS mallocAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 调用 BS->AllocatePages 函数来分配内存
    // AllocateAddress 参数表示按指定的物理地址分配内存
    // LIP ? LIP->ImageDataType : EfiLoaderData 是一个三元运算符，用于选择内存类型
    // LIP 存在时选择 LIP->ImageDataType，否则选择 EfiLoaderData
    // (size + 0xfff) / 0x1000 计算需要的页数，每页大小为 0x1000 字节
    // &addr 是用于接收分配的内存起始地址的指针
    return BS->AllocatePages(AllocateAddress, LIP ? LIP->ImageDataType : EfiLoaderData, (size + 0xfff) / 0x1000, &addr);
}

/**
 * @brief 释放指定物理地址和大小的内存
 * 根据给定的物理地址和大小，释放相应的内存空间。
 * @param addr 要释放的内存的物理地址
 * @param size 要释放的内存大小（以字节为单位）
 */
void freeAt(EFI_PHYSICAL_ADDRESS addr, UINTN size)
{
    // 使用BS（Boot Services）的FreePages函数释放内存页面
    BS->FreePages(addr, (size + 0xfff) / 0x1000);
    // (size + 0xfff) / 0x1000：将size向上取整到最近的4KB对齐边界，因为EFI规范中的页面大小是4KB
}

/**
 * @brief 计算 ELF 文件中所有加载段的地址范围
 * 该函数遍历 ELF 文件中的所有程序头，计算所有加载段的起始地址和结束地址范围。
 * @param ehdr ELF 文件头指针
 * @param first 用于存储计算出的最小起始地址
 * @param last 用于存储计算出的最大结束地址
 */
void CalcLoadAddressRange(Elf64_Ehdr *ehdr, UINT64 *first, UINT64 *last)
{
    // 获取程序头表指针
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((UINT64) ehdr + ehdr->e_phoff);
    // 初始化第一个地址范围的最大值
    *first = 0xffffffffffffffff;
    // 初始化最后一个地址范围的最小值
    *last = 0;
    // 遍历程序头表
    for (UINT16 i = 0; i < ehdr->e_phnum; i++) {
        // 如果程序头不是可加载类型，则跳过
        if (phdr[i].p_type != PT_LOAD) continue;
        // 更新第一个地址范围为当前最小值
        *first = min(*first, phdr[i].p_vaddr);
        // 更新最后一个地址范围为当前最大值
        *last = max(*last, phdr[i].p_vaddr + phdr[i].p_memsz);
    }
}

/**
 * @brief 将 ELF 文件的加载段复制到内存中
 * 该函数从 ELF 文件的头部信息开始，遍历 ELF 文件的所有程序头部表（Program Header Table）项。
 * 对于每个类型为 PT_LOAD 的段，它将段的内容从文件中复制到内存中，并填充剩余的内存空间为零。
 * @param ehdr ELF 文件头部信息的指针
 */
void CopyLoadSegments(Elf64_Ehdr *ehdr)
{
    // 获取程序头表指针
    Elf64_Phdr *phdr = (Elf64_Phdr *) ((UINT64) ehdr + ehdr->e_phoff);
    // 遍历程序头表中的每一个条目
    for (UINT16 i = 0; i < ehdr->e_phnum; i++) {
        // 如果当前条目不是可加载段，则跳过
        if (phdr[i].p_type != PT_LOAD) continue;
        // 计算段在文件中的起始地址
        UINT64 segm_in_file = (UINT64) ehdr + phdr[i].p_offset;
        // 将文件中的数据复制到目标地址
        memcpy((VOID *) phdr[i].p_vaddr, (VOID *) segm_in_file, phdr[i].p_filesz);
        // 计算剩余需要填充的字节数
        UINTN remain_bytes = phdr[i].p_memsz - phdr[i].p_filesz;
        // 使用0填充剩余的内存空间
        memset((VOID *) (phdr[i].p_vaddr + phdr[i].p_filesz), 0, remain_bytes);
    }
}

/**
 * @brief 初始化EFI环境
 * 该函数用于初始化EFI环境，包括设置看门狗定时器、定位图形输出协议、打开加载图像协议和打开简单文件系统协议。
 * @param ImageHandle 加载的EFI应用程序的图像句柄
 * @param SystemTable EFI系统表指针
 */
void efi_init(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
    // 将系统表指针保存到全局变量ST中
    ST = SystemTable;
    // 获取系统表中的启动服务表指针，并保存到全局变量BS中
    BS = SystemTable->BootServices;
    // 保存图像句柄到全局变量IM中
    IM = ImageHandle;
    // 关闭看门狗定时器，防止系统重启
    BS->SetWatchdogTimer(0, 0, 0, NULL); 
    // 查找GOP协议
    BS->LocateProtocol(&gop_guid, NULL, (void **) &GOP);
    // 打开LIP协议
    BS->OpenProtocol(ImageHandle, &lip_guid, (void **) &LIP, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL); 
    // 通过LIP协议的设备句柄打开SFSP协议
    BS->OpenProtocol(LIP->DeviceHandle, &sfsp_guid, (void **) &SFSP, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
}

/**
 * @brief 获取内存映射信息
 * 从UEFI引导服务中获取当前系统的内存映射信息，并将其存储在传入的MEMORY_MAP结构体中。
 * @param MemoryMap MEMORY_MAP类型的指针，用于存储获取的内存映射信息。
 * @return 返回EFI_STATUS类型的状态码，表示函数执行的结果。
 *         - EFI_SUCCESS: 成功获取内存映射信息。
 *         - EFI_OUT_OF_RESOURCES: 内存分配失败。
 */
EFI_STATUS GetMMP(MEMORY_MAP *MemoryMap) {
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
        if (MemoryMap->Buffer) {
            free(MemoryMap->Buffer);
            MemoryMap->Buffer = NULL;
        }
        // 根据新的 MapSize 重新分配缓冲区
        MemoryMap->Buffer = malloc(MemoryMap->MapSize);
    }
    // 如果缓冲区分配失败，则返回资源不足错误
    if (!MemoryMap->Buffer) {
        GetMemoryMapStatus = EFI_OUT_OF_RESOURCES;
    }
    return GetMemoryMapStatus;
}

// 退出引导服务
EFI_STATUS exitBootServices(EFI_HANDLE image_handle, MEMORY_MAP *memory_map) {
    EFI_STATUS status = BS->ExitBootServices(image_handle, memory_map->MapKey);
    if (status != EFI_SUCCESS) {
        return status;
    }
    // 释放内存映射缓冲区
    free(memory_map->Buffer);
    return EFI_SUCCESS;
}

/**
 * @brief 程序的入口点函数
 * 这个函数是EFI应用程序的入口点，负责初始化EFI环境，加载内核文件，设置帧缓冲区配置，并跳转到内核入口点执行。
 * @param ImageHandle EFI图像句柄，代表当前EFI应用程序
 * @param SystemTable 指向EFI系统表的指针，包含了系统级的服务和数据
 * @return EFI_STATUS 类型的返回值，表示函数执行的状态。如果成功，则返回EFI_SUCCESS
 */
EFI_STATUS entryPoint(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable)
{
    // 初始化EFI环境
    efi_init(ImageHandle, SystemTable);
    // 清屏
    ST->ConOut->ClearScreen(ST->ConOut);
    EFI_STATUS status;
    EFI_PHYSICAL_ADDRESS entry_addr;
    struct EFI_FILE_PROTOCOL *root, *kernel_file;
    UINTN kernel_size = 4194304;
    void *kernel_buffer = malloc(kernel_size);
    // 打开文件系统卷
    status = SFSP->OpenVolume(SFSP, &root);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while opening root dir");
        while (1);
    }
    // 打开内核文件
    status = root->Open(root, &kernel_file, L"\\FlameCore.elf", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while opening kernel file");

        while (1);
    }
    // 读取内核文件内容
    status = kernel_file->Read(kernel_file, &kernel_size, kernel_buffer);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while reading kernel file");
        while (1);
    }
    // 解析elf文件头部并计算内核加载地址范围
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) kernel_buffer;
    UINT64 kernel_first_addr, kernel_last_addr;
    CalcLoadAddressRange(ehdr, &kernel_first_addr, &kernel_last_addr);
    // 在指定地址范围分配内存
    status = mallocAt(kernel_first_addr, kernel_last_addr - kernel_first_addr);
    if (EFI_ERROR(status)) {
        puts(L"FAILED: error while allocating buffer for kernel");
        while (1);
    }
    // 复制加载段到指定内存地址
    CopyLoadSegments(ehdr);
    entry_addr = ehdr->e_entry;
    free(kernel_buffer);
    // 配置帧缓冲
    struct FrameBufferConfig config = {
        (UINT8 *) GOP->Mode->FrameBufferBase,
        GOP->Mode->Info->PixelsPerScanLine,
        GOP->Mode->Info->HorizontalResolution,
        GOP->Mode->Info->VerticalResolution
    };
    // 根据像素格式设置配置
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
    // 初始化启动配置
    BOOT_CONFIG BootConfig;
    BootConfig.MemoryMap.MapSize = 4096;
    BootConfig.MemoryMap.Buffer = NULL;
    BootConfig.MemoryMap.MapKey = 0;
    BootConfig.MemoryMap.DescriptorSize = 0;
    BootConfig.MemoryMap.DescriptorVersion = 0;
    // 获取内存映射
    GetMMP(&BootConfig.MemoryMap);
    // 定义内核函数类型
    typedef void (* __attribute__((sysv_abi)) Kernel)(const struct FrameBufferConfig *, const BOOT_CONFIG *);
    Kernel kernel = (Kernel) entry_addr;
    // 启动内核
    kernel(&config, (BOOT_CONFIG *) &BootConfig);
    // 退出引导服务
    exitBootServices(ImageHandle, &BootConfig.MemoryMap);
    struct EFI_TIME time;
    status = ST->RuntimeServices->EFI_GET_TIME(&time, NULL);
    if (status == EFI_SUCCESS) {
        puts(L"SUCCESS: kernel loaded and executed");
    } else {
        puts(L"FAILED: error while getting time");
    }
    while (1);
}