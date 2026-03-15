#include "app.h"
#include "app_cpp.h"

#include <array>
#include <cstdio>
#include <math.h>
#include <stdint.h>

#include "RotaryEncoder.h"
#include "air_signals.h"
#include "build_time.h"
#include "can_messages.h"
#include "disp_bright.h"
#include "gui.h"
#include "input_hal.h"
#include "rs422_port.h"

static uint32_t test_timeout = 0;

void app_on_timer() {
  systime = systime + 1;
  printf("mode %d timeout %d\n", gui_state.mode, test_timeout);

  switch (gui_state.mode) {
  case GUI_INIT:
    if (systime >= 500) {
      gui_state.mode = GUI_TEST;
      test_timeout = 1000;
    }
    break;
  case GUI_TEST:
    if (test_timeout <= 0) {
      gui_state.mode = GUI_MAIN_MODE;
    } else {
      test_timeout--;
    }
    break;
  default:
    break;
  }

  input_poll();

  rs422_serve();
  //	lv_tick_inc(HAL_TICK_FREQ_DEFAULT); //ToDo
  lv_tick_inc(1); // LVGL ожидает миллисекунды.  //ToDo
                  //    lv_tick_inc(10); // если таймер 10 ms: //ToDo
  encoder.serve_input(input_get_enc(), input_get_btn());

  static uint32_t cntr = 0;
  if (++cntr == 1000) {
    cntr = 0;

    // Версия для CAN
    static const can_msg_version_t version = {
        .timestamp = BuildTime, .maj_ver = MajVersion, .min_ver = MinVersion, .build = Build};

    can_send_dat(CAN_VERSION_MFI, &version, sizeof(version));
  }
}

#if APP_DEBUG
#define debug_print rs422_printf
#else
#define debug_print(...)
#endif

using namespace std;

void dummy_hold() { debug_print("dummy hold\r"); }
void dummy_timeout() { debug_print("dummy timeout\r"); }

#if USE_MTI_ICC
void icc_select(uint8_t item);
void bad_icc_select(uint8_t item);
void good_icc_select(uint8_t item);
#endif

RotaryEncoder::listener_t enc_brg = {brg_rotate, brg_click, brg_hold, dummy_timeout};
RotaryEncoder::listener_t enc_ref = {ref_rotate, ref_click, dummy_hold, dummy_timeout};
RotaryEncoder::listener_t enc_pres = {pres_rotate, pres_click, dummy_hold, dummy_timeout};
RotaryEncoder::listener_t enc_menu = {menu_rotate, menu_click, menu_hold, menu_timeout};
RotaryEncoder::listener_t enc_menu_timeless = {menu_rotate, menu_click, menu_hold, dummy_timeout};

#if USE_MTI_ICC
const char *main_menu_items[] = {"ЕД. ИЗМ. ДАВЛЕНИЯ", "ЕД. ИЗМ. ВЫСОТЫ", "КАЛИБРОВКА КОМПАСА",
                                 "ВЕРСИЯ"};
gui_menu_t menu_main = {nullptr, main_menu_items, 4, 0, menu_select};
#else
const char *main_menu_items[] = {"ЕД. ИЗМ. ДАВЛЕНИЯ", "ЕД. ИЗМ. ВЫСОТЫ", "ВЕРСИЯ"};
gui_menu_t menu_main = {nullptr, main_menu_items, 3, 0, menu_select};
#endif

const char *pres_items[] = {"мм. рт. ст", "гПа"};
gui_menu_t menu_pres = {"ЕДИНИЦЫ ДАВЛЕНИЯ", pres_items, 2, 0, pres_select};

const char *alt_items[] = {"метры", "футы"};
gui_menu_t menu_alt = {"ЕДИНИЦЫ ВЫСОТЫ", alt_items, 2, 0, alt_select};

const char *ver_items[] = {"ПО БИНС: -", "ПО МФИ: 0.5", "ЗАГРУЗЧИК БИНС: -", "ЗАГРУЗЧИК МФИ: -"};
gui_menu_t menu_ver = {"ВЕРСИЯ", ver_items, 4, 0, ver_select};

#if USE_MTI_ICC
const char *icc_items[] = {"вкл", "выкл"};
gui_menu_t menu_icc = {"КАЛИБРОВКА КОМПАСА", icc_items, 2, 0, icc_select};

const char *bad_icc_items[] = {"ОК"};
gui_menu_t menu_bad_icc = {"НЕДОСТАТОЧНО ДАННЫХ\rДЛЯ КАЛИБРОВКИ", bad_icc_items, 1, 0,
                           bad_icc_select};

const char *good_icc_items[] = {"СОХРАНИТЬ", "ОТМЕНА"};
gui_menu_t menu_good_icc = {nullptr, good_icc_items, 2, 0, good_icc_select};
#endif

gui_state_t gui_state = {
    // 0.0f,
    0,
};
RotaryEncoder encoder(enc_brg);
volatile uint32_t systime = 0;

//------------------------------------------------------------------------------
void brg_rotate(int8_t delta) {
  int brg = disp_bright_get();
  brg += (1 + brg / 10) * delta;
  if (brg < 0)
    brg = 0;
  else if (brg > 255)
    brg = 255;
  disp_bright_set((uint8_t)brg);
  debug_print("brightness %d\r", brg);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void brg_click() {
  gui_state.mode = GUI_SET_PRES_REF;
  encoder.change_listener(enc_ref);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void brg_hold() {
  gui_state.menu = &menu_main;
  gui_state.mode = GUI_MENU;
  encoder.change_listener(enc_menu);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ref_rotate(int8_t delta) {
  int type_nr = (int)gui_state.pres_type;
  if (delta > 0)
    gui_state.pres_type = (gui_type_pres_t)((type_nr < 2) ? ++type_nr : 0);
  else if (delta < 0)
    gui_state.pres_type = (gui_type_pres_t)((type_nr > 0) ? --type_nr : 2);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ref_click() {
  if (gui_state.pres_type == GUI_QNE) {
    gui_state.mode = GUI_MAIN_MODE;
    encoder.change_listener(enc_brg);
  } else {
    gui_state.mode = GUI_SET_PRES_VAL;
    encoder.change_listener(enc_pres);
  }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void pres_rotate(int8_t delta) {
  if (gui_state.pres_type == GUI_QFE)
    gui_state.pres_qfe += delta * 10;
  else if (gui_state.pres_type == GUI_QNH)
    gui_state.pres_qnh += delta * 10;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void pres_click() {
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void menu_rotate(int8_t delta) {
  int item_nr = (int)gui_state.menu->curr_item;
  if (delta > 0)
    gui_state.menu->curr_item = ((item_nr < (gui_state.menu->items_num - 1)) ? ++item_nr : 0);
  else if (delta < 0)
    gui_state.menu->curr_item = ((item_nr > 0) ? --item_nr : (gui_state.menu->items_num - 1));
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void menu_click() { gui_state.menu->on_select(gui_state.menu->curr_item); }
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void menu_hold() {
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void menu_timeout() {
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void menu_select(uint8_t item) {
  switch (item) {
  case 0:
    gui_state.menu = &menu_pres;
    break;
  case 1:
    gui_state.menu = &menu_alt;
    break;
#if USE_MTI_ICC
  case 2:
    gui_state.menu = &menu_icc;
    break;
  case 3:
    gui_state.menu = &menu_ver;
    break;
#else
  case 2:
    gui_state.menu = &menu_ver;
    break;
#endif
  }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void pres_select(uint8_t item) {
  switch (item) {
  case 0:
    gui_state.pres_unit = GUI_MERC;
    break;
  case 1:
    gui_state.pres_unit = GUI_PASC;
    break;
  }
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void alt_select(uint8_t item) {
  switch (item) {
  case 0:
    gui_state.alt_unit = GUI_METER;
    break;
  case 1:
    gui_state.alt_unit = GUI_FOOT;
    break;
  }
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ver_select(uint8_t item) {
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if USE_MTI_ICC
void icc_select(uint8_t item) {
  can_icc_cmd_t cmd;
  switch (item) {
  case 0:
    cmd = 0x0;
    break;
  case 1:
    cmd = 0x1;
    break;
  }
  can_send_dat(CAN_CMD_ICC, &cmd, sizeof(cmd));
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if USE_MTI_ICC
void bad_icc_select(uint8_t item) {
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if USE_MTI_ICC
void good_icc_select(uint8_t item) {
  if (item == 0) {
    can_icc_cmd_t cmd = CAN_CMD_ICC_SRORE;
    can_send_dat(CAN_CMD_ICC, &cmd, sizeof(cmd));
  }
  gui_state.mode = GUI_MAIN_MODE;
  encoder.change_listener(enc_brg);
}
#endif
//------------------------------------------------------------------------------
