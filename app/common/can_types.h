#pragma once

#include <stdint.h>

typedef struct
{
    const char* timestamp;
    int maj_ver;
    int min_ver;
    int build;
} can_msg_version_t;

#define CAN_VERSION_MFI 1