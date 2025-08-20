#include "include_define_obj.h"

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BME280 bme;
LiquidCrystal_I2C lcd(0x27,20,4);
// LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS3231 rtc;

long tiempoUltimaLectura = 0;
int contDeGiros = 0;
long tiempooSiguienteRotacion = 0;
long tiempooLectura = 0;
bool cambioEstado = false;
bool enPosicion = false;
float tBME = 0;
float hBME = 0;
float tDHT = 0;
float hDHT = 0;

int rotacionesRealizadas = 0;

State state_a_subsis_menus(&on_state_subsis_menus, &in_a_state_subsis_menus, nullptr);
State state_b_subsis_menus(&on_state_subsis_menus, &in_b_state_subsis_menus, nullptr);
State state_c_subsis_menus(&on_state_subsis_menus, &in_c_state_subsis_menus, nullptr);
Fsm fsm_subsis_menus(&state_a_subsis_menus);

State state_a_subsis_mov(&on_a_state_subsis_mov, &a_in_state_subsis_mov, nullptr);
State state_b_subsis_mov(&on_b_state_subsis_mov, &b_in_state_subsis_mov, nullptr);
Fsm fsm_subsis_mov(&state_a_subsis_mov);

State state_a_subsis_temp(&on_a_state_subsis_temp, &in_a_state_subsis_temp, nullptr);
State state_b_subsis_temp(&on_b_state_subsis_temp, &in_b_state_subsis_temp, nullptr);
State state_c_subsis_temp(&on_c_state_subsis_temp, &in_c_state_subsis_temp, nullptr);
Fsm fsm_subsis_temp(&state_a_subsis_temp);