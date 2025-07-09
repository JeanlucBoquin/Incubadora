#include "DHT.h"
#include "Fsm.h"
#include "RTClib.h"
#include "LiquidCrystal_I2C.h"

#define DHTPIN 2      //Pin digital #2 (datos)
#define DHTTYPE DHT22 //Sensor DHT22
#define pulsador 3

// ############################ Definicion de variables #######################
enum Trigger{
  BUTTON_EVENT // Equivalente a #define BUTTON_EVENT 1
};

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS3231 rtc;

long tiempoUltimaLectura = 0;
byte degree_symbol[8] =
{
0b00111,  //   ***    
0b00101,  //   * *
0b00111,  //   ***
0b00000,  //   
0b00000,  //   
0b00000,  //   
0b00000,  //   
0b00000   //
};
byte gota[8] =
{
0b00100,  //   *  
0b00100,  //   *  
0b01110,  //  *** 
0b01110,  //  *** 
0b11111,  // *****
0b11111,  // *****
0b11111,  // ***** 
0b01110   //  ***
};
byte termometro[8] =
{
0b00100,  //   *        
0b01010,  //  * *   
0b01010,  //  * *   
0b01010,  //  * *   
0b01110,  //  ***   
0b11111,  // *****  
0b11111,  // *****   
0b01110   //  ***  
};

// ############################ Prototipos de metodos #########################
void on_state();
void a_in_state();
void b_in_state();
void c_in_state();
void check_button();

// ############################ Configuracion de FSM ##########################
// State state(&func_on_enter_state, &func_in_state, &func_on_exit_state)
State state_a(&on_state, &a_in_state, nullptr);
State state_b(&on_state, &b_in_state, nullptr);
State state_c(&on_state, &c_in_state, nullptr);
Fsm fsm(&state_a);

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando...");
  dht.begin();
  rtc.begin();

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, degree_symbol);
  lcd.createChar(1, gota);
  lcd.createChar(2, termometro);

  // fsm.add_transition(&state_to_transition_from, &state_to_transition_to, &trigger, &func_on_transition)
  fsm.add_transition(&state_a, &state_b, BUTTON_EVENT, nullptr);
  fsm.add_transition(&state_b, &state_c, BUTTON_EVENT, nullptr);
  fsm.add_transition(&state_c, &state_a, BUTTON_EVENT, nullptr);
}

void loop() {
  fsm.run_machine();
}

void on_state(){
  lcd.clear();
};

void a_in_state(){
  float h = dht.readHumidity();         //Leemos la Humedad
  float t = dht.readTemperature();      //Leemos la temperatura en grados Celsius
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(h);
  lcd.print(" ");
  lcd.write(byte (1));

  lcd.setCursor(0, 1);
  lcd.print("Tempert: ");
  lcd.print(t);
  lcd.print(" ");
  lcd.write(byte (2));
  check_button();
}

void b_in_state(){
  static bool bandera = true;
  static String fecha;
  static String hora;

  DateTime now = rtc.now();
  fecha = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  // (now.second() == 59 ) ? lcd.clear() : (void)0 ;
  
  if(now.second() == 0 && bandera == true){
    lcd.setCursor(10, 1);
    lcd.print("      ");
    tiempoUltimaLectura = millis();
    bandera = false;
  }
  if(millis() - tiempoUltimaLectura > 1000){
    bandera = true;
  }

  Serial.println(hora);
  lcd.setCursor(0, 0);
  lcd.print("Fecha: ");
  lcd.print(fecha);
  lcd.setCursor(0, 1);
  lcd.print("Hora.: ");
  lcd.print(hora);
  check_button();
}

void c_in_state(){
  lcd.print("Hola mundo");
  lcd.setCursor(0, 0);
  check_button();
}

void check_button(){
  static bool flancoAscendente = false;

  if(millis() - tiempoUltimaLectura > 120){
    if( digitalRead(pulsador) == 0 && flancoAscendente == false ){
      if(flancoAscendente == false){
        Serial.println("button_pressed");
        fsm.trigger(BUTTON_EVENT);
      }
      flancoAscendente = true;
    }

    if(digitalRead(pulsador) == 1 && flancoAscendente == true){
      flancoAscendente = false;
      tiempoUltimaLectura = millis();
    }
  }
}