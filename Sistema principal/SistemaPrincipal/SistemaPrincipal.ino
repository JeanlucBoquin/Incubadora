#include "characterLCD.h"
#include "include_define_obj.h"


void setup() {
  delay(1500);
  pinMode(BTN_NEXT, INPUT);
  pinMode(BTN_PREVIOUS, INPUT);
  pinMode(modoAutomatico, INPUT);
  pinMode(inputSignalOptoAcoplador, INPUT);
  pinMode(giroDerecha, OUTPUT);
  pinMode(giroIzquierda, OUTPUT);
  pinMode(outputRelay, OUTPUT);
  digitalWrite(outputRelay, HIGH);

  Serial.begin(9600);
  wdt_enable(WDTO_2S);
  Serial.println("Iniciando...");
  
	if (!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}

  dht.begin();
  rtc.begin();
  // rtc.adjust(DateTime(2025,8,10,15,10,0));

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, simbolo_grados);
  lcd.createChar(1, gota);
  lcd.createChar(2, termometro);

  // fsm_subsis_menus.add_transition(&state_to_transition_from, &state_to_transition_to, &trigger, &func_on_transition)
  fsm_subsis_menus.add_transition(&state_a_subsis_menus, &state_b_subsis_menus, BUTTON_EVENT_NEXT, nullptr);
  fsm_subsis_menus.add_transition(&state_b_subsis_menus, &state_c_subsis_menus, BUTTON_EVENT_NEXT, nullptr);
  fsm_subsis_menus.add_transition(&state_c_subsis_menus, &state_a_subsis_menus, BUTTON_EVENT_NEXT, nullptr);

  fsm_subsis_menus.add_transition(&state_a_subsis_menus, &state_c_subsis_menus, BUTTON_EVENT_PREVIOUS, nullptr);
  fsm_subsis_menus.add_transition(&state_b_subsis_menus, &state_a_subsis_menus, BUTTON_EVENT_PREVIOUS, nullptr);
  fsm_subsis_menus.add_transition(&state_c_subsis_menus, &state_b_subsis_menus, BUTTON_EVENT_PREVIOUS, nullptr);

  fsm_subsis_mov.add_transition(&state_a_subsis_mov, &state_b_subsis_mov, TIEMPO_SIG_ROTACION, nullptr);
  fsm_subsis_mov.add_transition(&state_b_subsis_mov, &state_a_subsis_mov, ROTACIONES_FINALIZADAS, nullptr);

  fsm_subsis_temp.add_transition(&state_a_subsis_temp, &state_b_subsis_temp, TEMP_LOW, nullptr);
  fsm_subsis_temp.add_transition(&state_b_subsis_temp, &state_a_subsis_temp, TEMP_HIGH, nullptr);
  
  fsm_subsis_temp.add_transition(&state_a_subsis_temp, &state_c_subsis_temp, TEMP_HIGH_HIGH1, nullptr);
  fsm_subsis_temp.add_transition(&state_b_subsis_temp, &state_c_subsis_temp, TEMP_HIGH_HIGH2, nullptr);
  fsm_subsis_temp.add_transition(&state_c_subsis_temp, &state_a_subsis_temp, REST_TEMP, nullptr);
}

void loop() {
    // h = dht.readHumidity();
    tDHT = dht.readTemperature();
    // Serial.println(tDHT);
  if(digitalRead(modoAutomatico) == true){
    fsm_subsis_menus.run_machine();
    // fsm_subsis_mov.run_machine();
    fsm_subsis_temp.run_machine();
  }
  else{

    lcd.setCursor(0, 0);
    lcd.print("Focos: ");

    if(digitalRead(outputRelay) == false){
      lcd.print("ON ");
    }else{
      lcd.print("OFF");  
    }

    lcd.setCursor(0, 1);
    lcd.print("Motor: ");
    if(digitalRead(giroDerecha) == true){
      lcd.print("ON ");
    }else{
      lcd.print("OFF");  
    }

    button_manual_focos();
    button_manual_motor();
  }
  wdt_reset();
}

