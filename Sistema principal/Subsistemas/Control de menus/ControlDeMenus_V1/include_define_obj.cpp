#include "include_define_obj.h"

// ############################ Instancia de objetos ##########################
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS3231 rtc;