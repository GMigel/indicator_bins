#pragma once

#ifdef PLATFORM_PC

#include <cstdint>
#include <cstdio>

// PC CAN stubs implemented in app_pc_stubs.cpp

// // Заглушка структуры версии
// struct can_msg_version_t {
//     const char* timestamp;
//     uint8_t maj_ver;
//     uint8_t min_ver;
//     uint8_t build;
// };

// Заглушка констант CAN
// constexpr int CAN_VERSION_MFI = 1;

// Заглушка функции отправки CAN
// inline void can_send_dat(int id, const void* data, size_t len) {
//     (void)id; (void)data; (void)len;
//     // для отладки
//     // printf("[PC] CAN send %zu bytes\n", len);
// }

#endif // PLATFORM_PC