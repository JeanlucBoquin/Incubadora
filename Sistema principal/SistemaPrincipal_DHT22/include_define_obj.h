#ifndef INCLUDE_DEFINE_OBJ_H
#define INCLUDE_DEFINE_OBJ_H

  #include <Wire.h>
  #include <Fsm.h>
  #include <RTClib.h>
  #include <LiquidCrystal_I2C.h>
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>
  #include "DHT.h"

  #define DHTPIN 2      //Pin digital #2 (datos)
  #define DHTTYPE DHT22 //Sensor DHT22

  #define BTN_NEXT 12
  #define BTN_PREVIOUS 11
  #define inputSignalOptoAcoplador 8
  #define giroDerecha 9
  #define giroIzquierda 10
  #define SEALEVELPRESSURE_HPA (1013.25)
  #define outputRelay 7
  #define modoAutomatico 6

  // ############################ Definicion de variables #######################
  enum Trigger{
      BUTTON_EVENT_NEXT,      // equivalente #define BUTTON_EVENT_NEXT 12
      BUTTON_EVENT_PREVIOUS,  // equivalente #define BUTTON_EVENT_PREVIOUS 11
      TIEMPO_SIG_ROTACION,    // Tres rotaciones cada 4 horas
      ROTACIONES_FINALIZADAS,  // Flinaliza el ciclo de tres rotaciones cada 4 horas
      TEMP_LOW,      // equivalente #define BUTTON_EVENT_NEXT 12
      TEMP_HIGH
  };

  // extern Adafruit_BME280 bme;
  extern LiquidCrystal_I2C lcd;
  extern RTC_DS3231 rtc;
  extern DHT dht;

  extern long tiempoUltimaLectura;
  extern int contDeGiros;
  extern long tiempooSiguienteRotacion;
  extern long tiempooLectura;
  extern bool cambioEstado;
  extern bool enPosicion;
  extern float t;
  extern float h;

  extern int rotacionesRealizadas;


  // ############################ Prototipos de metodos #########################
  void on_state_subsis_menus();
  void in_a_state_subsis_menus();
  void in_b_state_subsis_menus();
  void in_c_state_subsis_menus();
  void check_button_next();
  void check_button_previous();

  void on_a_state_subsis_mov();
  void on_b_state_subsis_mov();
  void a_in_state_subsis_mov();
  void b_in_state_subsis_mov();

  void on_a_state_subsis_temp();
  void on_b_state_subsis_temp();
  void in_a_state_subsis_temp();
  void in_b_state_subsis_temp();

  void button_manual_focos();
  void button_manual_motor();



// ############################ Configuracion de FSM ##########################
// State state(&func_on_enter_state, &func_in_state, &func_on_exit_state)
  extern State state_a_subsis_menus;
  extern State state_b_subsis_menus;
  extern State state_c_subsis_menus;
  extern Fsm fsm_subsis_menus;

  extern State state_a_subsis_mov;
  extern State state_b_subsis_mov;
  extern Fsm fsm_subsis_mov;

  extern State state_a_subsis_temp;
  extern State state_b_subsis_temp;
  extern Fsm fsm_subsis_temp;

#endif