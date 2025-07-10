#ifndef INCLUDE_DEFINE_OBJ_H
#define INCLUDE_DEFINE_OBJ_H

  #include <Arduino.h>
  #include <DHT.h>
  #include <RTClib.h>
  #include <LiquidCrystal_I2C.h>

  #define DHTPIN 2      //Pin digital #2 (datos)
  #define DHTTYPE DHT22 //Sensor DHT22
  #define BTN_NEXT 3
  #define BTN_PREVIOUS 4

  enum Triggers {
    BUTTON_EVENT_NEXT,    // equivalente #define BUTTON_EVENT_NEXT 1
    BUTTON_EVENT_PREVIOUS // equivalente #define BUTTON_EVENT_PREVIOUS 2
  };

  extern DHT dht;
  extern LiquidCrystal_I2C lcd;
  extern RTC_DS3231 rtc;

#endif