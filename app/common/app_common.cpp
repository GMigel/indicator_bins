#include "app.h"
#include "app_cpp.h"

#include <math.h>
#include <cstdio>
#include <stdint.h>
#include <array>

#include "air_signals.h"
#include "build_time.h"
#include "RotaryEncoder.h"
#include "gui.h"
#include "can_port.h"
#include "disp_bright_hal.h"
#include "input_hal.h"
#include "rs422_port.h"

#if APP_DEBUG
	#define debug_print rs422_printf
#else
  #define debug_print(...)
#endif

using namespace std;

void dummy_hold() {debug_print("dummy hold\r");};
void dummy_timeout() {debug_print("dummy timeout\r");};

void brg_rotate(int8_t delta);
void brg_click();
void brg_hold();

void ref_rotate(int8_t delta);
void ref_click();

void pres_rotate(int8_t delta);
void pres_click();

void menu_rotate(int8_t delta);
void menu_click();
void menu_hold();
void menu_timeout();
void menu_select(uint8_t item);
void pres_select(uint8_t item);
void alt_select(uint8_t item);
void ver_select(uint8_t item);
#if USE_MTI_ICC
void	icc_select(uint8_t item);
void	bad_icc_select(uint8_t item);
void	good_icc_select(uint8_t item);
#endif
// void can_serve();

RotaryEncoder::listener_t enc_brg = {brg_rotate, brg_click, brg_hold, dummy_timeout};
RotaryEncoder::listener_t enc_ref = {ref_rotate, ref_click, dummy_hold, dummy_timeout};
RotaryEncoder::listener_t enc_pres = {pres_rotate, pres_click, dummy_hold, dummy_timeout};
RotaryEncoder::listener_t enc_menu = {menu_rotate, menu_click, menu_hold, menu_timeout};
RotaryEncoder::listener_t enc_menu_timeless = {menu_rotate, menu_click, menu_hold, dummy_timeout};

#if USE_MTI_ICC
const char* main_menu_items[] = {"ЕД. ИЗМ. ДАВЛЕНИЯ","ЕД. ИЗМ. ВЫСОТЫ", "КАЛИБРОВКА КОМПАСА", "ВЕРСИЯ"};
gui_menu_t menu_main = {NULL, main_menu_items, 4, 0, menu_select};
#else
const char* main_menu_items[] = {"ЕД. ИЗМ. ДАВЛЕНИЯ","ЕД. ИЗМ. ВЫСОТЫ", "ВЕРСИЯ"};
gui_menu_t menu_main = {NULL, main_menu_items, 3, 0, menu_select};
#endif

const char* pres_items[] = {"мм. рт. ст","гПа"};
gui_menu_t menu_pres = {"ЕДИНИЦЫ ДАВЛЕНИЯ", pres_items, 2, 0, pres_select};

const char* alt_items[] = {"метры","футы"};
gui_menu_t menu_alt = {"ЕДИНИЦЫ ВЫСОТЫ", alt_items, 2, 0, alt_select};

const char* ver_items[] = {"ПО БИНС: -","ПО МФИ: 0.5", "ЗАГРУЗЧИК БИНС: -", "ЗАГРУЗЧИК МФИ: -"};
gui_menu_t menu_ver = {"ВЕРСИЯ", ver_items, 4, 0, ver_select};

#if USE_MTI_ICC
const char* icc_items[] = {"вкл","выкл"};
gui_menu_t menu_icc = {"КАЛИБРОВКА КОМПАСА", icc_items, 2, 0, icc_select};

const char* bad_icc_items[] = {"ОК"};
gui_menu_t menu_bad_icc = {"НЕДОСТАТОЧНО ДАННЫХ\rДЛЯ КАЛИБРОВКИ", bad_icc_items, 1, 0, bad_icc_select};

const char* good_icc_items[] = {"СОХРАНИТЬ","ОТМЕНА"};
gui_menu_t menu_good_icc = {nullptr, good_icc_items, 2, 0, good_icc_select};
#endif

gui_state_t gui_state = {0.0f, };
RotaryEncoder encoder(enc_brg);
volatile uint32_t systime = 0;
