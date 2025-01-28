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
//                MYTHOS UEFI HEADER FILE
//
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _UEFI_H_
#define _UEFI_H_

typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned long long  UINT64;

typedef UINT64  UINTN; 
typedef UINTN   EFI_STATUS;
typedef void    VOID;
typedef UINT16  CHAR16;

typedef UINTN EFI_PHYSICAL_ADDRESS;
typedef UINTN EFI_VIRTUAL_ADDRESS;

typedef VOID *EFI_HANDLE;
typedef VOID *EFI_EVENT;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef unsigned long int   size_t; // compatible to ELF toolchain

typedef long long           int64_t;

#define EFI_ERROR_MASK 0x8000000000000000
#define EFIERR(a) (EFI_ERROR_MASK | a)
#define EFI_ERROR(status) (((long long) status) < 0)

#define EFI_SUCCESS                     0
#define EFI_LOAD_ERROR                  EFIERR(1)
#define EFI_INVALID_PARAMETER           EFIERR(2)
#define EFI_UNSUPPORTED                 EFIERR(3)
#define EFI_BAD_BUFFER_SIZE             EFIERR(4)
#define EFI_BUFFER_TOO_SMALL            EFIERR(5)
#define EFI_NOT_READY                   EFIERR(6)
#define EFI_DEVICE_ERROR                EFIERR(7)
#define EFI_WRITE_PROTECTED             EFIERR(8)
#define EFI_OUT_OF_RESOURCES            EFIERR(9)
#define EFI_VOLUME_CORRUPTED            EFIERR(10)
#define EFI_VOLUME_FULL                 EFIERR(11)
#define EFI_NO_MEDIA                    EFIERR(12)
#define EFI_MEDIA_CHANGED               EFIERR(13)
#define EFI_NOT_FOUND                   EFIERR(14)
#define EFI_ACCESS_DENIED               EFIERR(15)
#define EFI_NO_RESPONSE                 EFIERR(16)
#define EFI_NO_MAPPING                  EFIERR(17)
#define EFI_TIMEOUT                     EFIERR(18)
#define EFI_NOT_STARTED                 EFIERR(19)
#define EFI_ALREADY_STARTED             EFIERR(20)
#define EFI_ABORTED                     EFIERR(21)
#define EFI_ICMP_ERROR                  EFIERR(22)
#define EFI_TFTP_ERROR                  EFIERR(23)
#define EFI_PROTOCOL_ERROR              EFIERR(24)
#define EFI_INCOMPATIBLE_VERSION        EFIERR(25)
#define EFI_SECURITY_VIOLATION          EFIERR(26)
#define EFI_CRC_ERROR                   EFIERR(27)
#define EFI_END_OF_MEDIA                EFIERR(28)
#define EFI_END_OF_FILE                 EFIERR(31)
#define EFI_INVALID_LANGUAGE            EFIERR(32)
#define EFI_COMPROMISED_DATA            EFIERR(33)

#define NULL                            ((void *) 0)

#define PT_LOAD 1

typedef struct
{
    UINT32 p_type;        /* Segment type */
    UINT32 p_flags;       /* Segment flags */
    UINT64 p_offset;      /* Segment file offset */
    UINT64 p_vaddr;       /* Segment virtual address */
    UINT64 p_paddr;       /* Segment physical address */
    UINT64 p_filesz;      /* Segment size in file */
    UINT64 p_memsz;       /* Segment size in memory */
    UINT64 p_align;       /* Segment alignment */
} Elf64_Phdr;

typedef struct
{
    UINT8  e_ident[16];   /* Magic number and other info */
    UINT16 e_type;        /* Object file type */
    UINT16 e_machine;     /* Architecture */
    UINT32 e_version;     /* Object file version */
    UINT64 e_entry;       /* Entry point virtual address */
    UINT64 e_phoff;       /* Program header table file offset */
    UINT64 e_shoff;       /* Section header table file offset */
    UINT32 e_flags;       /* Processor-specific flags */
    UINT16 e_ehsize;      /* ELF header size in bytes */
    UINT16 e_phentsize;   /* Program header table entry size */
    UINT16 e_phnum;       /* Program header table entry count */
    UINT16 e_shentsize;   /* Section header table entry size */
    UINT16 e_shnum;       /* Section header table entry count */
    UINT16 e_shstrndx;    /* Section header string table index */
} Elf64_Ehdr;

struct EFI_INPUT_KEY {
    UINT16 ScanCode;
    CHAR16 UnicodeChar;
};

struct EFI_GUID {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];
};

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

enum EFI_ALLOCATE_TYPE {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
};

// UEFI memory type
enum EFI_MEMORY_TYPE 
{
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiMaxMemoryType
};

// memory map struct
typedef struct __packed
{
    UINTN MapSize;
    void *Buffer;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
} MEMORY_MAP;

typedef struct 
{
	UINT32 Type;
	UINT64 PhysicalStart;
	UINT64 PageSize;
} OS_MEMORY_DESCRIPTOR;

// UEFI memory struct
typedef struct 
{
  UINT32 Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  UINT64 NumberOfPages;
  UINT64 Attribute;
}  __attribute__((__aligned__(16))) EFI_MEMORY_DESCRIPTOR;

// boot config struct
typedef struct
{
    MEMORY_MAP MemoryMap;
} BOOT_CONFIG;

struct EFI_BOOT_SERVICES {
    char _buf1[24];

    // Task Priority Services
    UINTN _buf2[2];

    // Memory Services
    EFI_STATUS (*AllocatePages)(
        enum EFI_ALLOCATE_TYPE  Type,
        enum EFI_MEMORY_TYPE    MemoryType,
        UINTN                   Pages,
        EFI_PHYSICAL_ADDRESS    *Memory
    );
    EFI_STATUS (*FreePages)(
        EFI_PHYSICAL_ADDRESS    Memory,
        UINTN                   Pages
    );
    EFI_STATUS (*GetMemoryMap) (
        UINTN                           *MemoryMapSize,
        EFI_MEMORY_DESCRIPTOR           *MemoryMap,
        UINTN                           *MapKey,
        UINTN                           *DescriptorSize,
        UINT32                          *DescriptorVersion
    );
    EFI_STATUS (*AllocatePool)(
            enum EFI_MEMORY_TYPE    PoolType,
            UINTN                   Size,
            VOID                    **Buffer
    );
    EFI_STATUS (*FreePool)(VOID *Buffer);

    // Event & Timer Services
    UINTN _buf4[2];
    EFI_STATUS (*WaitForEvent)(
        UINTN       NumberOfEvents,
        EFI_EVENT   *Event,
        UINTN       *Index
    );
    UINTN _buf4_2[3];

    // Protocol Handler Services
    UINTN _buf5[9];

    // Image Services
    UINTN _buf6[5];

    // Miscellaneous Services
    UINTN _buf7[2];
    EFI_STATUS (*SetWatchdogTimer)(
        UINTN   Timeout,
        UINT64  WatchdogCode,
        UINTN   DataSize,
        CHAR16  *WatchdogData);

    // DriverSupport Services
    UINTN _buf8[2];

    // Open and Close Protocol Services
    EFI_STATUS (*OpenProtocol)(
        EFI_HANDLE          Handle,
        struct EFI_GUID     *Protocol,
        VOID                **Interface,
        EFI_HANDLE          AgentHandle,
        EFI_HANDLE          ControllerHandle,
        UINT32              Attributes
    );
    UINTN _buf9[2];

    // Library Services
    UINTN _buf10[2];
    EFI_STATUS (*LocateProtocol)(
        struct EFI_GUID *Protocol,
        VOID            *Registration,
        VOID            **Interface
    );
    UINTN _buf10_2[2];

    // 32-bit CRC Services
    UINTN _buf11;

    // Miscellaneous Services
    UINTN _buf12[3];
};

struct EFI_SYSTEM_TABLE {
    char _buf1[44];
    struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
        UINTN       Reset;
        EFI_STATUS (*ReadKeyStroke)(
            struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL   *This,
            struct EFI_INPUT_KEY                    *Key
        );
        EFI_EVENT   WaitForKey;
    } *ConIn;
    UINTN _buf2;
    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
        UINTN _buf;
        EFI_STATUS (*OutputString)(
            struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *This,
            CHAR16                                  *String
        );
        UINTN _buf2[4];
        EFI_STATUS (*ClearScreen)(
            struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
        );
    } *ConOut;
    UINTN _buf3[3];
    struct EFI_BOOT_SERVICES *BootServices;
};

struct EFI_SIMPLE_POINTER_STATE
{
    int     RelativeMovementX;
    int     RelativeMovementY;
    int     RelativeMovementZ;
    char    LeftButton;
    char    RightButton;
};

struct EFI_SIMPLE_POINTER_PROTOCOL 
{
    EFI_STATUS (*Reset)(
        struct EFI_SIMPLE_POINTER_PROTOCOL  *This,
        unsigned char                       ExtendedVerification
    );
    EFI_STATUS (*GetState)(
        struct EFI_SIMPLE_POINTER_PROTOCOL  *This,
        struct EFI_SIMPLE_POINTER_STATE     *State
    );
    void *WaitForInput;
};

struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL {
    UINT8 Blue;
    UINT8 Green;
    UINT8 Red;
    UINT8 Reserved;
};

enum EFI_GRAPHICS_PIXEL_FORMAT {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
};

struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    UINTN _buf[3];
    struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
        UINT32                  MaxMode;
        UINT32                  Mode;
        struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
            UINT32                          Version;
            UINT32                          HorizontalResolution;
            UINT32                          VerticalResolution;
            enum EFI_GRAPHICS_PIXEL_FORMAT  PixelFormat;
            UINT32                          PixelInformation[4];
            UINT32                          PixelsPerScanLine;
        } *Info;
        UINTN                   SizeOfInfo;
        EFI_PHYSICAL_ADDRESS    FrameBufferBase;
        UINTN                   FrameBufferSize;
    } *Mode;
};

struct EFI_LOADED_IMAGE_PROTOCOL {
    UINT32                      Revision;
    EFI_HANDLE                  ParentHandle;
    struct EFI_SYSTEM_TABLE     *SystemTable;
    EFI_HANDLE                  DeviceHandle;
    VOID                        *FilePath; 
    VOID                        *Reserved;
    UINT32                      LoadOptionsSize;
    VOID                        *LoadOptions;
    VOID                        *ImageBase;
    UINT64                      ImageSize;
    enum EFI_MEMORY_TYPE        ImageCodeType;
    enum EFI_MEMORY_TYPE        ImageDataType; 
    EFI_STATUS (*Unload)(EFI_HANDLE ImageHandle);
}; // LIP

struct EFI_FILE_INFO {
    unsigned char   _buf[80];
    CHAR16          FileName[]; 
};

#define EFI_FILE_MODE_READ      0x0000000000000001
#define EFI_FILE_MODE_WRITE     0x0000000000000002
#define EFI_FILE_MODE_CREATE    0x8000000000000000

struct EFI_FILE_PROTOCOL {
    UINTN Revision;
    EFI_STATUS (*Open)(struct EFI_FILE_PROTOCOL *This,
                   struct EFI_FILE_PROTOCOL **NewHandle,
                   CHAR16                   *FileName,
                   UINT64                   OpenMode,
                   UINT64                   Attributes
    );
    EFI_STATUS (*Close)(struct EFI_FILE_PROTOCOL *This);
    UINTN _buf2;
    EFI_STATUS (*Read)(struct EFI_FILE_PROTOCOL *This,
                   UINTN    *BufferSize,
                   VOID     *Buffer
    );
    EFI_STATUS (*Write)(struct EFI_FILE_PROTOCOL *This,
                    UINTN   *BufferSize,
                    VOID    *Buffer
    );
    UINTN _buf3[4];
    EFI_STATUS (*Flush)(struct EFI_FILE_PROTOCOL *This);
}; // EFI_FILE_PROTOCOL

struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    UINT64 Revision;
    EFI_STATUS (*OpenVolume)(
        struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *This,
        struct EFI_FILE_PROTOCOL                **Root
    );
}; // SFSP

extern struct EFI_SYSTEM_TABLE                  *ST;
extern struct EFI_GRAPHICS_OUTPUT_PROTOCOL      *GOP;
extern struct EFI_LOADED_IMAGE_PROTOCOL         *LIP;
extern struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *SFSP;
extern struct EFI_BOOT_SERVICES                 *BS;

#endif 