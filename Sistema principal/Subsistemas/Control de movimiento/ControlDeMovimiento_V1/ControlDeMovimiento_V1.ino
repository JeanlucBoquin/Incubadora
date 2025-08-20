#define INPUTSIGNAL 2
#define giroDerecha 3
#define giroIzquierda 4

long tiempo = 0;
long tiempo2 = 0;
bool cambioEstado = false;

void setup(){
  pinMode(INPUTSIGNAL, INPUT);
  pinMode(12, OUTPUT);
  pinMode(giroDerecha, OUTPUT);
  pinMode(giroIzquierda, OUTPUT);
  Serial.begin(9600);

  digitalWrite(giroDerecha, HIGH);
  tiempo = -5000;
}

void loop(){

  if(millis() - tiempo > 7000 && digitalRead(INPUTSIGNAL) == LOW){
    digitalWrite(giroDerecha, HIGH);
  }
  // activa el tiempo
  if(cambioEstado == false && digitalRead(INPUTSIGNAL) == HIGH && (millis() - tiempo > 4000)){
    digitalWrite(giroDerecha, HIGH);
  }
  
  // Cambio de estado
  if(digitalRead(INPUTSIGNAL) == LOW){
    cambioEstado = true;
  }

  // // desactiva el HIGH
  
  if(cambioEstado == true && digitalRead(INPUTSIGNAL) == HIGH){
    cambioEstado = false;
    digitalWrite(giroDerecha, LOW);
    digitalWrite(giroIzquierda, HIGH);
    tiempo = millis();
    tiempo2 = millis();
  }

  if(millis() - tiempo2 > 5){
    digitalWrite(giroIzquierda, LOW);
  }

  Serial.print("cambio de estado: ");
  Serial.println(cambioEstado);
  Serial.print("Entrada: ");
  Serial.println(digitalRead(INPUTSIGNAL));
  // delay(500);
 
}