#ifdef PLATFORM_PC
void can_init() {}
bool HAL_FDCAN_IsRxBufferMessageAvailable( void ) { return false; }
bool HAL_FDCAN_GetRxMessage( void ) { return false; }
void can_send_dat(uint32_t cmd, void* dat, uint8_t len) {} 
#endif