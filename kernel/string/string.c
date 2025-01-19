#include "../../includes/firmware/uefi.h"

struct EFI_BOOT_SERVICES *BS;
struct EFI_LOADED_IMAGE_PROTOCOL *LIP;

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