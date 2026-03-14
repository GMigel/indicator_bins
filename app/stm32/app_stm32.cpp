#include "app.h"
#include "app_cpp.h"
#include "main.h"

#include <math.h>
#include <cstdio>
#include <array>

#include "air_signals.h"
#include "build_time.h"
#include "RotaryEncoder.h"
#include "gui.h"
#include "can_port.h"
#include "disp_bright.h"
#include "input_hal.h"
#include "rs422_port.h"
#include "fdcan.h"

using std::array;

#if APP_DEBUG
	#define debug_print rs422_printf
#else
  #define debug_print(...)
#endif

//------------------------------------------------------------------------------
void app_run()
{
#if APP_DEBUG
	uint32_t refresh_cntr = 0;
	uint32_t prev_time = 0;
#endif

	can_init();
 	gui_init(&gui_state);
    disp_bright_init();
 	input_init();

 	while(1)
 	{
 	  can_serve();
      gui_refresh(&gui_state);
  	  lv_timer_handler();

#if APP_DEBUG
  	refresh_cntr++;
  	uint32_t dt = systime - prev_time;
  	if(dt >= 1000)
  	{
  		prev_time = systime;
  		uint32_t rate = refresh_cntr*10000/dt;
  		refresh_cntr = 0;
  		__disable_irq();
  		debug_print("FPS %d.%01d\r", rate/10, rate%10);
  		__enable_irq();
  	}
#endif
 	}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void can_serve()
{
  static std::array<uint32_t, CAN_TOTAL_BUFNR> rx_time {}; // initialized by zeros
  static std::array<float, CAN_TOTAL_BUFNR> rx_val {};
  enum
  {
    ATTITUDE_TIMEOUT = 100,
    AIR_TIMEOUT = 500
  };

  for(int i=0; i<CAN_TOTAL_BUFNR; i++)
  {
    FDCAN_RxHeaderTypeDef rx_hdr;
    uint8_t buf[8];
    if(HAL_FDCAN_IsRxBufferMessageAvailable(&can, i) && (HAL_FDCAN_GetRxMessage(&can, i, &rx_hdr, buf) == HAL_OK))
    {
      can_val_t* can_val = (can_val_t*)buf;
      if(can_val->state.bins_ok && can_val->state.valid)
      {
        rx_time[i] = systime;
        rx_val[i] = can_val->value;
      }
    }
  }

  // roll
  if(((systime-rx_time[CAN_BUFNR_ROLL1]) < ATTITUDE_TIMEOUT) || ((systime-rx_time[CAN_BUFNR_ROLL2]) < ATTITUDE_TIMEOUT))
  {
    gui_state.valid_roll = true;
    gui_state.roll = rx_val[((systime-rx_time[CAN_BUFNR_ROLL1]) < ATTITUDE_TIMEOUT) ? CAN_BUFNR_ROLL1 : CAN_BUFNR_ROLL2]/180.0f*PI;
  }
  else
    gui_state.valid_roll = false;

  // pitch
  if(((systime-rx_time[CAN_BUFNR_PITCH1]) < ATTITUDE_TIMEOUT) || ((systime-rx_time[CAN_BUFNR_PITCH2]) < ATTITUDE_TIMEOUT))
  {
    gui_state.valid_pitch = true;
    gui_state.pitch = rx_val[((systime-rx_time[CAN_BUFNR_PITCH1]) < ATTITUDE_TIMEOUT) ? CAN_BUFNR_PITCH1 : CAN_BUFNR_PITCH2]/180.0f*PI;
  }
  else
    gui_state.valid_pitch = false;

  // heading
  if(((systime-rx_time[CAN_BUFNR_HEADING1]) < ATTITUDE_TIMEOUT) || ((systime-rx_time[CAN_BUFNR_HEADING2]) < ATTITUDE_TIMEOUT))
  {
    gui_state.valid_heading = true;
    gui_state.heading = rx_val[((systime-rx_time[CAN_BUFNR_HEADING1]) < ATTITUDE_TIMEOUT) ? CAN_BUFNR_HEADING1 : CAN_BUFNR_HEADING2]/180.0f*PI;
  }
  else
    gui_state.valid_heading = false;

  // altitude
  if(((systime-rx_time[CAN_BUFNR_PRES1]) < AIR_TIMEOUT) || ((systime-rx_time[CAN_BUFNR_PRES2]) < AIR_TIMEOUT))
  {
    gui_state.valid_altitude = true;
    float pres = rx_val[((systime-rx_time[CAN_BUFNR_PRES1]) < AIR_TIMEOUT) ? CAN_BUFNR_PRES1 : CAN_BUFNR_PRES2];
    float ref = GUI_PRES_QNE;
    if(gui_state.pres_type == GUI_QFE)
      ref = gui_state.pres_qfe;
    else if(gui_state.pres_type == GUI_QNH)
      ref = gui_state.pres_qnh;
    gui_state.altitude = altitude(pres, ref);
  }
  else
    gui_state.valid_altitude = false;

  // airspeed
  if(((systime-rx_time[CAN_BUFNR_AIRSPEED1]) < AIR_TIMEOUT) || ((systime-rx_time[CAN_BUFNR_AIRSPEED2]) < AIR_TIMEOUT))
  {
    gui_state.valid_speed = true;
    gui_state.speed = rx_val[((systime-rx_time[CAN_BUFNR_AIRSPEED1]) < AIR_TIMEOUT) ? CAN_BUFNR_AIRSPEED1 : CAN_BUFNR_AIRSPEED2]*3.6f;
  }
  else
    gui_state.valid_speed = false;

  // vertspeed
  if(((systime-rx_time[CAN_BUFNR_VERTSPEED1]) < AIR_TIMEOUT) || ((systime-rx_time[CAN_BUFNR_VERTSPEED2]) < AIR_TIMEOUT))
  {
    gui_state.valid_ver_speed = true;
    gui_state.vert_speed = rx_val[((systime-rx_time[CAN_BUFNR_VERTSPEED1]) < AIR_TIMEOUT) ? CAN_BUFNR_VERTSPEED1 : CAN_BUFNR_VERTSPEED2];
  }
  else
    gui_state.valid_ver_speed = false;
}
//------------------------------------------------------------------------------

#warning app_on_timer()
 //------------------------------------------------------------------------------
// void app_on_timer()
// {
//   systime = systime + 1;
//
//   if (gui_state.mode == GUI_INIT)
//   {
//     if (systime == 500)
//       gui_state.mode = GUI_TEST;
//   }
//   else if (gui_state.mode == GUI_TEST)
// 	{
// 		static uint32_t test_timeot = 1000;
// 		if(test_timeot == 0)
// 		{
// 			gui_state.mode = GUI_MAIN_MODE;
// 			test_timeot = 1000;
// 		}
// 		else
// 			test_timeot--;
// 	}
//
//     rs422_serve();
// //	lv_tick_inc(HAL_TICK_FREQ_DEFAULT); //ToDo
//     lv_tick_inc(1); // LVGL ожидает миллисекунды.  //ToDo
// //    lv_tick_inc(10); // если таймер 10 ms: //ToDo
// 	encoder.serve_input(input_get_enc(), input_get_btn());
//
// #if 1
//   static uint32_t cntr = 0;
//   if(++cntr == 1000)
//   {
//   	cntr = 0;
//   	static const can_msg_version_t version = {.timestamp=BuildTime, .maj_ver=MajVersion, .min_ver=MinVersion, .build=Build};
//   	can_send_dat(CAN_VERSION_MFI, &version, sizeof(version));
//   }
// #endif
// }
// //------------------------------------------------------------------------------
