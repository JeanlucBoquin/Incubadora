#ifndef DEFINICIONES_OBJETOS_H
#define DEFINICIONES_OBJETOS_H

  #include <Arduino.h>
  #include <DHT.h>
  #include <RTClib.h>
  #include <LiquidCrystal_I2C.h>

  #define DHTPIN 2      //Pin digital #2 (datos)
  #define DHTTYPE DHT22 //Sensor DHT22
  #define pulsador 3
  #define BUTTON_EVENT 1

  extern DHT dht;
  extern LiquidCrystal_I2C lcd;
  extern RTC_DS3231 rtc;

#endif