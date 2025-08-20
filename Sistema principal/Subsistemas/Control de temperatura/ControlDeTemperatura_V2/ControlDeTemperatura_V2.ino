#include "Fsm.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define outputRelay 7

enum Trigger{
    TEMP_LOW,      // equivalente #define BUTTON_EVENT_NEXT 12
    TEMP_HIGH
};

Adafruit_BME280 bme;

float t = 0;
float h = 0;

void on_a_state_subsis_temp();
void on_b_state_subsis_temp();
void in_a_state_subsis_temp();
void in_b_state_subsis_temp();

State state_a_subsis_temp(&on_a_state_subsis_temp, &in_a_state_subsis_temp, nullptr);
State state_b_subsis_temp(&on_b_state_subsis_temp, &in_b_state_subsis_temp, nullptr);
Fsm fsm_subsis_temp(&state_a_subsis_temp);

void setup(){
  pinMode(outputRelay, OUTPUT);
  digitalWrite(outputRelay, HIGH);

  Serial.begin(9600);
	if (!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}

  fsm_subsis_temp.add_transition(&state_a_subsis_temp, &state_b_subsis_temp, TEMP_LOW, nullptr);
  fsm_subsis_temp.add_transition(&state_b_subsis_temp, &state_a_subsis_temp, TEMP_HIGH, nullptr);
}

void loop(){
  fsm_subsis_temp.run_machine();
}

void on_a_state_subsis_temp(){
  digitalWrite(outputRelay, HIGH);
}

void in_a_state_subsis_temp(){
  t = bme.readTemperature();
  if(t <= 36.5){
    digitalWrite(outputRelay, LOW);
    fsm_subsis_temp.trigger(TEMP_LOW);
  }
  Serial.print("Temperature = ");
	Serial.print(bme.readTemperature());
	Serial.println("*C");
  Serial.print("Humidity = ");
	Serial.print(bme.readHumidity());
	Serial.println("%");
	Serial.println("ESTADO A");
	Serial.println();
}

void on_b_state_subsis_temp(){
}

void in_b_state_subsis_temp(){
  t = bme.readTemperature();
  if(t >= 37.5){
    digitalWrite(outputRelay, HIGH);
    fsm_subsis_temp.trigger(TEMP_HIGH);
  }

  Serial.print("Temperature = ");
	Serial.print(bme.readTemperature());
	Serial.println("*C");
  Serial.print("Humidity = ");
	Serial.print(bme.readHumidity());
	Serial.println("%");
	Serial.println("ESTADO B");
	Serial.println();
}