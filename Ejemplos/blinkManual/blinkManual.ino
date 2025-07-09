#define ledBlink 13
#define pulsador 3

bool flancoAscendente;
bool estadoLedBlink;
long tiempoUltimaLectura = 0;


void setup(){
  pinMode(ledBlink, OUTPUT);
  pinMode(pulsador, INPUT);
}

void loop(){
  if(millis() - tiempoUltimaLectura > 120){
    if( digitalRead(pulsador) == 0 && flancoAscendente == false ){
      estadoLedBlink = !digitalRead(ledBlink);
      digitalWrite(ledBlink, estadoLedBlink);
      flancoAscendente = true;
    }

    if(digitalRead(pulsador) == 1 && flancoAscendente == true){
      flancoAscendente = false;
      tiempoUltimaLectura = millis();
    }
  }

}