#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "characterLCD.h"

#define BTN_ON_OFF  4
#define FOCO_ON_OFF 5
#define DHTPIN 8      //Pin digital #2 (datos)
// #define DHTPIN2 2      //Pin digital #2 (datos)
#define DHTTYPE DHT22 //Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);
// DHT dht2(DHTPIN2, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

bool flancoAscendente = false;
bool estadoFoco;
long tiempoUltimaLectura = 0;

float h;
float t;
float t2;

void setup(){
  Serial.begin(9600);
  pinMode(BTN_ON_OFF, INPUT);
  pinMode(FOCO_ON_OFF, OUTPUT);
  digitalWrite(FOCO_ON_OFF, true);

  dht.begin();
  // dht2.begin();
  
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, simbolo_grados);
  lcd.createChar(1, gota);
  lcd.createChar(2, termometro);
  tiempoUltimaLectura = millis();
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);

}

void loop(){
  h = dht.readHumidity();         //Leemos la Humedad
  t = dht.readTemperature();      //Leemos la temperatura en grados Celsius
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(h);
  lcd.print(" ");
  lcd.write(byte (1));
  
  lcd.setCursor(0, 1);
  lcd.print("Tempert: ");
  lcd.print(t);
  lcd.write(byte (2));

  if(millis()-tiempoUltimaLectura > 500){
    // t = dht.readHumidity();      //Leemos la temperatura en grados Celsius
    Serial.print("readHumidity: ");
    Serial.println(h);
    // t = dht.readTemperature();      //Leemos la temperatura en grados Celsius
    Serial.print("Tempert: ");
    Serial.println(t);
    tiempoUltimaLectura = millis();
  }

  if(t < 30){
    digitalWrite(FOCO_ON_OFF, false);
  }
  if(t > 38){
    digitalWrite(FOCO_ON_OFF, true);
  }
}

