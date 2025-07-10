#include "characterLCD.h"
#include "include_define_obj.h"
#include "configuracion_fsm.h"

void setup() {
  Serial.begin(9600);
  pinMode(BTN_NEXT, INPUT);
  pinMode(BTN_PREVIOUS, INPUT);

  dht.begin();
  rtc.begin();

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, simbolo_grados);
  lcd.createChar(1, gota);
  lcd.createChar(2, termometro);

  // fsm.add_transition(&state_to_transition_from, &state_to_transition_to, &trigger, &func_on_transition)
  fsm.add_transition(&state_a, &state_b, BUTTON_EVENT_NEXT, nullptr);
  fsm.add_transition(&state_b, &state_c, BUTTON_EVENT_NEXT, nullptr);
  fsm.add_transition(&state_c, &state_a, BUTTON_EVENT_NEXT, nullptr);

  fsm.add_transition(&state_a, &state_c, BUTTON_EVENT_PREVIOUS, nullptr);
  fsm.add_transition(&state_b, &state_a, BUTTON_EVENT_PREVIOUS, nullptr);
  fsm.add_transition(&state_c, &state_b, BUTTON_EVENT_PREVIOUS, nullptr);
}

void loop() {
  fsm.run_machine();
}