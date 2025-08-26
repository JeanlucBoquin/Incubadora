#include "Arduino.h"
#include "include_define_obj.h"

#define temperaturaMin 37.50
#define temperaturaMax 37.79
#define temperaturaAler 38.10

// ================================== SUBSISTEMA DE CONTROL DE PANTALLAS ========================================
void on_state_subsis_menus(){
  lcd.clear();
};

void in_a_state_subsis_menus(){
  static long tiempoLecturaBME = millis();
  if(millis() - tiempoLecturaBME > 250){
    lcd.setCursor(0, 0 );
    lcd.print("Humedada...: ");
    lcd.print(hBME);
    lcd.print(" ");
    lcd.write(byte (1));

    lcd.setCursor(0, 1);
    lcd.print("Temperatura: ");
    lcd.print(tBME);
    lcd.print(" ");
    lcd.write(byte (2));

    lcd.setCursor(0, 2);
    lcd.print("Rotaciones.: ");
    lcd.print(rotacionesRealizadas);
  

    lcd.setCursor(0, 3);
    lcd.print("Tempera DHT: ");
    lcd.print(tDHT);
    lcd.print(" ");
    lcd.write(byte (2));
    
    tiempoLecturaBME = millis();
  }
  check_button_next();
  check_button_previous();
}

void in_b_state_subsis_menus(){
  static bool bandera = true;
  static String fecha;
  static String hora;

  DateTime now = rtc.now();
  fecha = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  // // (now.second() == 59 ) ? lcd.clear() : (void)0 ;

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

void in_c_state_subsis_menus(){
  // Rotaciones
  // SiguienteRotacion
  // Rango de horas de rotacion
  lcd.setCursor(0, 0);
  lcd.print("Hola mundo");
  check_button_next();
  check_button_previous();
}

void check_button_next(){
  static bool flancoAscendente = false;

  if(millis() - tiempoUltimaLectura > 120){
    if(digitalRead(BTN_NEXT) == 0 && flancoAscendente == false ){
      if(flancoAscendente == false){
        Serial.println("button_pressed");
        fsm_subsis_menus.trigger(BUTTON_EVENT_NEXT);
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
        fsm_subsis_menus.trigger(BUTTON_EVENT_PREVIOUS);
      }
      flancoAscendente = true;
    }

    if(digitalRead(BTN_PREVIOUS) == 1 && flancoAscendente == true){
      flancoAscendente = false;
      tiempoUltimaLectura = millis();
    }
  }
}
// ===============================================================================================================


// ================================== SUBSISTEMA DE CONTROL DE MOVIMIENTO ========================================

void on_a_state_subsis_mov(){
  tiempooSiguienteRotacion = millis();
  enPosicion = false;
}

void a_in_state_subsis_mov(){
  //Para realizar los giros 7am a 4pm
  DateTime now = rtc.now();

  if(digitalRead(inputSignalOptoAcoplador) == 0){
    digitalWrite(giroDerecha, true);
    enPosicion = false;
  }else{
    digitalWrite(giroDerecha, false);
    enPosicion = true;
  }

  // if(millis() - tiempooSiguienteRotacion > 14400000 && enPosicion == true){ // 4 horas
  // if(millis() - tiempooSiguienteRotacion > 14400000 && enPosicion == true && (now.hour() >= 7 && now.hour() <= 16)){ // 4 horas
  if(millis() - tiempooSiguienteRotacion > 13630000 && enPosicion == true && (now.hour() >= 7 && now.hour() <= 16)){ // 41.5 horas
  // if(millis() - tiempooSiguienteRotacion > 1800000 && enPosicion == true){ // 30 min
    fsm_subsis_mov.trigger(TIEMPO_SIG_ROTACION);
  }
}

void on_b_state_subsis_mov(){
  contDeGiros = 0;
  cambioEstado = false;
  digitalWrite(giroDerecha, true);

  tiempooLectura = millis();
}

void b_in_state_subsis_mov(){
  bool inputOpto = digitalRead(inputSignalOptoAcoplador);
  if(contDeGiros < 3){
    if( inputOpto == false){
      cambioEstado = true;
      tiempooLectura = millis();
    }

    if(inputOpto == true && cambioEstado == true && (millis()-tiempooLectura > 100)){
      cambioEstado = false;
      contDeGiros += 1;
    }
    // Por que cuando comento el Serial.println, ya no funciona el contador de 3 vueltas?
    Serial.println(contDeGiros);
  }

  if(contDeGiros >= 3){
    digitalWrite(giroDerecha, false);
    digitalWrite(giroIzquierda, true);
    
    //Este espera de 100ms se realiza con el objetivo de que el motor sea mas preciso en su punto de parada
    // con el uso del contra giro gracias al puente H con transitores.
    if(millis()-tiempooLectura > 100){
      rotacionesRealizadas += 1;
      digitalWrite(giroIzquierda, false);
      fsm_subsis_mov.trigger(ROTACIONES_FINALIZADAS);
    }
  }
}
// ===============================================================================================================


// ================================== SUBSISTEMA DE CONTROL DE TEMPERATURA ========================================

void on_a_state_subsis_temp(){
  digitalWrite(outputRelay, HIGH);
}

void in_a_state_subsis_temp(){
  // Serial.println("A");
  obtenerDatosTemperaturaHumedad();
  if(tBME >= temperaturaAler or tDHT >= temperaturaAler){
    fsm_subsis_temp.trigger(TEMP_HIGH_HIGH1);
  }
  if(tBME <= temperaturaMin){
    digitalWrite(outputRelay, LOW);
    fsm_subsis_temp.trigger(TEMP_LOW);
  }
  tiempoSubsistemaControlTemperatura = millis();
}

void on_b_state_subsis_temp(){
}

void in_b_state_subsis_temp(){
  // Serial.println("B");
  // tBME = bme.readTemperature(); Esto estaba muy mal en el programa, se mama la comunicacion I2C por que eran muchas lecturas?
  //Seguro de lo anterior????
  obtenerDatosTemperaturaHumedad();
  if(tBME >= temperaturaAler or tDHT >= temperaturaAler){
    fsm_subsis_temp.trigger(TEMP_HIGH_HIGH2);
  }
  if(tBME >= temperaturaMax){
    digitalWrite(outputRelay, HIGH);
    fsm_subsis_temp.trigger(TEMP_HIGH);
  }
  tiempoSubsistemaControlTemperatura = millis();
}

void on_c_state_subsis_temp(){
  lcd.clear();
  digitalWrite(outputRelay, HIGH);
}

void in_c_state_subsis_temp(){
  // Serial.println("C");
  obtenerDatosTemperaturaHumedad();
  if(tBME <= 37.5 or tDHT <= 37.5){
    fsm_subsis_temp.trigger(REST_TEMP);
  }
  tiempoSubsistemaControlTemperatura = millis();
}
// ===============================================================================================================


// ======================================= FUNCIONES DE CONTROL MANUAL ============================================
void button_manual_focos(){
  static bool flancoAscendente = false;

  if(millis() - tiempoUltimaLectura > 150){
    if(digitalRead(BTN_NEXT) == 0 && flancoAscendente == false ){
      digitalWrite(outputRelay, !digitalRead(outputRelay));
      flancoAscendente = true;
    }

    if(digitalRead(BTN_NEXT) == 1 && flancoAscendente == true){
      flancoAscendente = false;
      tiempoUltimaLectura = millis();
    }
  }
}

void button_manual_motor(){
  static bool flancoAscendente = false;

  if(millis() - tiempoUltimaLectura > 150){
    if( digitalRead(BTN_PREVIOUS) == 0 && flancoAscendente == false ){

      digitalWrite(giroDerecha, !digitalRead(giroDerecha));
      flancoAscendente = true;
    }

    if(digitalRead(BTN_PREVIOUS) == 1 && flancoAscendente == true){
      flancoAscendente = false;
      tiempoUltimaLectura = millis();
    }
  }
}
// ===============================================================================================================


// ========================================== FUNCIONES ADICIONALES ==============================================
void obtenerDatosTemperaturaHumedad(){
  static long tiempoLecturaTempHumBME = millis();
  static long tiempoLecturaTempHumDHT = millis();

  if(millis() - tiempoLecturaTempHumBME > 500){
    tBME = bme.readTemperature();  //Leemos la temperatura en grados Celsius BME
    hBME = bme.readHumidity();     //Leemos la Humedad BME
    tiempoLecturaTempHumBME  = millis();
  }

  if(millis() - tiempoLecturaTempHumDHT > 2000){
    tDHT = dht.readTemperature();  //Leemos la temperatura en grados Celsius DHT22
    hDHT = dht.readHumidity();     //Leemos la Humedad DHT22
    tiempoLecturaTempHumDHT  = millis();
  }
}
// ===============================================================================================================