#include "include_define_obj.h"

void on_state_subsis_menus(){
  lcd.clear();
};

void in_a_state_subsis_menus(){
  static long tiempoLecturaBME = millis();
  if(millis() - tiempoLecturaBME > 150){

    // h = bme.readHumidity();    //Leemos la Humedad
    // t = bme.readTemperature();      //Leemos la temperatura en grados Celsius
    h = dht.readHumidity();         //Leemos la Humedad
    t = dht.readTemperature(); 
    
    lcd.setCursor(0, 0 );
    // lcd.print("Hume: ");
    // Serial.println(t);
    // // Serial.println(h);
    lcd.print("Humedada...: ");
    lcd.print(h);
    lcd.print(" ");
    lcd.write(byte (1));

    lcd.setCursor(0, 1);
    lcd.print("Temperatura: ");
    // lcd.print("Temp: ");
    lcd.print(t);
    lcd.print(" ");
    lcd.write(byte (2));

    lcd.setCursor(0, 2);
    lcd.print("Rotaciones.: ");
    lcd.print(rotacionesRealizadas);
    tiempoLecturaBME = millis();
  }
  check_button_next();
  check_button_previous();
}

void in_b_state_subsis_menus(){
  static bool bandera = true;
  static String fecha;
  static String hora;

  // DateTime now = rtc.now();
  // fecha = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  // hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  // // (now.second() == 59 ) ? lcd.clear() : (void)0 ;
  
  // if(now.second() == 0 && bandera == true){
  //   lcd.setCursor(10, 1);
  //   lcd.print("      ");
  //   tiempoUltimaLectura = millis();
  //   bandera = false;
  // }
  // if(millis() - tiempoUltimaLectura > 1000){
  //   bandera = true;
  // }

  // Serial.println(hora);
  lcd.setCursor(0, 0);
  lcd.print("Fecha: ");
  // lcd.print(fecha);
  // lcd.setCursor(0, 1);
  // lcd.print("Hora.: ");
  // lcd.print(hora);
  check_button_next();
  check_button_previous();
}

void in_c_state_subsis_menus(){
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

void on_a_state_subsis_mov(){
  tiempooSiguienteRotacion = millis();
  enPosicion = false;
}

void a_in_state_subsis_mov(){
  if(digitalRead(inputSignalOptoAcoplador) == 0){
    digitalWrite(giroDerecha, true);
    enPosicion = false;
  }else{
    digitalWrite(giroDerecha, false);
    enPosicion = true;
  }

  // if(millis() - tiempooSiguienteRotacion > 14400000 && enPosicion == true){ // 4 horas
  if(millis() - tiempooSiguienteRotacion > 1800000 && enPosicion == true){ // 30min
  // if(millis() - tiempooSiguienteRotacion > 10000 && enPosicion == true){ // 30min
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
    
    if(inputOpto == true && cambioEstado == true && (millis()-tiempooLectura > 1)){
      cambioEstado = false;
      contDeGiros += 1;
    }
    // Serial.print("contDeGiros: ");
    // Serial.print("Optoacoplador: ");
    // Serial.println(inputOpto);
    // Serial.print("cambio de estado: ");
    // Serial.println(cambioEstado);
    // Serial.print("Tiempo: ");
    // Serial.println(tiempooLectura);
      // Por que cuando comento el Serial.println, ya no funciona el contador de 3 vueltas?
    Serial.println(contDeGiros);
  }

  if(contDeGiros >= 3){
    digitalWrite(giroDerecha, false);
    digitalWrite(giroIzquierda, true);

    if(millis()-tiempooLectura > 100){
      rotacionesRealizadas += 1; 
      digitalWrite(giroIzquierda, false);
      fsm_subsis_mov.trigger(ROTACIONES_FINALIZADAS);
    }

  }
}

void on_a_state_subsis_temp(){
  digitalWrite(outputRelay, HIGH);
}

void in_a_state_subsis_temp(){
  // t = bme.readTemperature();
  if(t <= 37.5){
    digitalWrite(outputRelay, LOW);
    fsm_subsis_temp.trigger(TEMP_LOW);
  }
}

void on_b_state_subsis_temp(){
}

void in_b_state_subsis_temp(){
  // t = bme.readTemperature(); Esto estaba muy miy mal en el programa, se mamaba la comunicacion I2C por que eran muchas lecturas
  // Serial.print("SubTem:");
  // Serial.println(t);
  if(t >= 37.8){
    digitalWrite(outputRelay, HIGH);
    fsm_subsis_temp.trigger(TEMP_HIGH);
  }
}

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