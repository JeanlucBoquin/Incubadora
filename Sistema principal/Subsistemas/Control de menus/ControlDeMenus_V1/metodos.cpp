#include "include_define_obj.h"
#include "configuracion_fsm.h"

long tiempoUltimaLectura = 0;

void on_state(){
  lcd.clear();
};

void a_in_state(){
  static float h;
  static float t;
  
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
  // lcd.write(byte (0));
  lcd.print(" ");
  lcd.write(byte (2));
  check_button_next();
  check_button_previous();
}

void b_in_state(){
  static bool bandera = true;
  static String fecha;
  static String hora;

  DateTime now = rtc.now();
  fecha = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  if(now.second() == 0 && bandera == true){
    lcd.setCursor(10, 1);
    lcd.print("      ");
    tiempoUltimaLectura = millis();
    bandera = false;
  }
  if(millis() - tiempoUltimaLectura > 1000){
    bandera = true;
  }

  lcd.setCursor(0, 0);
  lcd.print("Fecha: ");
  lcd.print(fecha);
  lcd.setCursor(0, 1);
  lcd.print("Hora.: ");
  lcd.print(hora);
  check_button_next();
  check_button_previous();
}

void c_in_state(){
  lcd.setCursor(0, 0);
  lcd.print("Temp exter.: ");
  lcd.print(" ");
  lcd.write(byte (2));
  lcd.setCursor(0, 1);
  lcd.print("Temp inter.: ");
  lcd.print(" ");
  lcd.write(byte (2));
  check_button_next();
  check_button_previous();
}

void check_button_next(){
  static bool flancoAscendente = false;
  
  if(millis() - tiempoUltimaLectura > 120){
    if( digitalRead(BTN_NEXT) == 0 && flancoAscendente == false ){
      if(flancoAscendente == false){
        Serial.println("button_pressed");
        fsm.trigger(BUTTON_EVENT_NEXT);
      }
      flancoAscendente = true;
    }

    if(digitalRead(BTN_NEXT) == 1 && flancoAscendente == true){
      flancoAscendente = false;
      tiempoUltimaLectura = millis();
    }
  }
}

void check_button_previous(){
    static bool flancoAscendente = false;
  
  if(millis() - tiempoUltimaLectura > 120){
    if( digitalRead(BTN_PREVIOUS) == 0 && flancoAscendente == false ){
      if(flancoAscendente == false){
        Serial.println("button_pressed");
        fsm.trigger(BUTTON_EVENT_PREVIOUS);
      }
      flancoAscendente = true;
    }

    if(digitalRead(BTN_PREVIOUS) == 1 && flancoAscendente == true){
      flancoAscendente = false;
      tiempoUltimaLectura = millis();
    }
  }
}