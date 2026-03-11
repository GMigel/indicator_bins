#ifdef PLATFORM_PC
#define HAL_TICK_FREQ_DEFAULT 5
#endif

#ifdef PLATFORM_PC
#include <cstdint>
#include <cstdio>
#include <array>
#include "app.h"
#include "gui.h"

// Fake systime for simulation
extern volatile uint32_t systime;

// Dummy CAN buffer size and constants
constexpr int CAN_TOTAL_BUFNR = 16;
constexpr int CAN_BUFNR_ROLL1 = 0;
constexpr int CAN_BUFNR_ROLL2 = 1;
constexpr int CAN_BUFNR_PITCH1 = 2;
constexpr int CAN_BUFNR_PITCH2 = 3;
constexpr int CAN_BUFNR_HEADING1 = 4;
constexpr int CAN_BUFNR_HEADING2 = 5;
constexpr int CAN_BUFNR_PRES1 = 6;
constexpr int CAN_BUFNR_PRES2 = 7;
constexpr int CAN_BUFNR_AIRSPEED1 = 8;
constexpr int CAN_BUFNR_AIRSPEED2 = 9;
constexpr int CAN_BUFNR_VERTSPEED1 = 10;
constexpr int CAN_BUFNR_VERTSPEED2 = 11;

// Dummy CAN value struct
struct can_val_t {
    struct { bool bins_ok; bool valid; } state;
    float value;
};

// Fake CAN and RS422 functions
void can_init() { printf("[PC] CAN init\n"); }
void rs422_serve() { /* do nothing */ }

// Generate fake CAN messages
/*
void can_serve()
{
    static std::array<uint32_t, CAN_TOTAL_BUFNR> rx_time {};
    static std::array<float, CAN_TOTAL_BUFNR> rx_val {};

    // Simulate some values oscillating with systime
    for(int i=0; i<CAN_TOTAL_BUFNR; i++) {
        rx_time[i] = systime;
        rx_val[i] = (float)((i+1) * 10 + (systime % 100));
    }

    // Roll
    gui_state.valid_roll = true;
    gui_state.roll = rx_val[CAN_BUFNR_ROLL1]/180.0f * 3.141592653f;

    // Pitch
    gui_state.valid_pitch = true;
    gui_state.pitch = rx_val[CAN_BUFNR_PITCH1]/180.0f * 3.141592653f;

    // Heading
    gui_state.valid_heading = true;
    gui_state.heading = rx_val[CAN_BUFNR_HEADING1]/180.0f * 3.141592653f;

    // Altitude
    gui_state.valid_altitude = true;
    gui_state.altitude = rx_val[CAN_BUFNR_PRES1];

    // Airspeed
    gui_state.valid_speed = true;
    gui_state.speed = rx_val[CAN_BUFNR_AIRSPEED1];

    // Vert speed
    gui_state.valid_ver_speed = true;
    gui_state.vert_speed = rx_val[CAN_BUFNR_VERTSPEED1];
}
*/

// Dummy brightness functions
int disp_bright_get() { return 128; }
void disp_bright_set(uint8_t val) { (void)val; }

// Dummy input init
void input_init() { printf("[PC] Input init\n"); }
void disp_bright_init() { printf("[PC] Display brightness init\n"); }

// Dummy can_send_dat
struct can_msg_version_t {
    const char* timestamp;
    int maj_ver;
    int min_ver;
    int build;
};

constexpr int CAN_VERSION_MFI = 1;

void can_send_dat(int id, const void* data, size_t len) {
    (void)id; (void)data; (void)len;
    // Just print something for debugging
    printf("[PC] CAN send %zu bytes\n", len);
}

#endif // PLATFORM_PC