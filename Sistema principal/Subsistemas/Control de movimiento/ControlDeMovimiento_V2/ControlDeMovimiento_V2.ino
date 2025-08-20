#include <Fsm.h>

#define inputSignalOptoAcoplador 2
#define giroDerecha 3
#define giroIzquierda 4

int contDeGiros = 0;
long tiempooSiguienteRotacion = 0;
long tiempooLectura = 0;
bool cambioEstado = false;
bool enPosicion = false;

void on_state_aM();
void on_state_bM();
void a_in_stateM();
void b_in_stateM();

enum Triggers {
  TIEMPO_SIG_ROTACION,    // equivalente #define BUTTON_EVENT_NEXT 1
  ROTACIONES_FINALIZADAS // equivalente #define BUTTON_EVENT_PREVIOUS 2
};


State state_aM(&on_state_aM, &a_in_stateM, nullptr);
State state_bM(&on_state_bM, &b_in_stateM, nullptr);

Fsm fsmM(&state_aM);

void setup(){
  Serial.begin(9600);
  pinMode(inputSignalOptoAcoplador, INPUT);
  pinMode(giroDerecha, OUTPUT);
  pinMode(giroIzquierda, OUTPUT);

  // tiempo = -5000;

  // fsm.add_transition(&state_to_transition_from, &state_to_transition_to, &trigger, &func_on_transition)
  fsm.add_transition(&state_a, &state_b, TIEMPO_SIG_ROTACION, nullptr);
  fsm.add_transition(&state_b, &state_a, ROTACIONES_FINALIZADAS, nullptr);
}

void loop(){
  fsm.run_machine();
}


void on_state_aM(){
  tiempooSiguienteRotacion = millis();
  enPosicion = false;
}

void a_in_stateM(){
  if(digitalRead(inputSignalOptoAcoplador) == 0){
    digitalWrite(giroDerecha, true);
    enPosicion = false;
  }else{
    digitalWrite(giroDerecha, false);
    enPosicion = true;
  }

  if(millis() - tiempooSiguienteRotacion > 5000 && enPosicion == true){
    fsm.trigger(TIEMPO_SIG_ROTACION);
  }
}

void on_state_bM(){
  contDeGiros = 0;
  cambioEstado = false;
  digitalWrite(giroDerecha, true);

  tiempooLectura = millis();
}

void b_in_stateM(){
  bool inputOpto = digitalRead(inputSignalOptoAcoplador);

  if( inputOpto == false){
    cambioEstado = true;
    tiempooLectura = millis();
  }
  
  // 
  if(inputOpto == true && cambioEstado == true && (millis()-tiempooLectura > 1)){
    cambioEstado = false;
    contDeGiros += 1;
  }

  if(contDeGiros >= 3){
    digitalWrite(giroDerecha, false);

    // tiempooLectura = millis();
    // while(millis()-tiempooLectura < 50){
    //   digitalWrite(giroIzquierda, true);
    // }
    // digitalWrite(giroIzquierda, false);
    
    fsm.trigger(ROTACIONES_FINALIZADAS);
  }

}
