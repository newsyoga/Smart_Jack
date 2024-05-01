#include "Arduino.h"
#include "pulse-sensor-arduino.h"

#define HEARTPULSE_PIN_SIG	A0

PulseSensor heartpulse;
int Delay_in = 1000;
int s20 = 1;
bool in20 = 0;
int count_hb = 0;

void setup() 
{
    Serial.begin(9600);
    while (!Serial) ; 
    Serial.println("start");
    
    heartpulse.begin(HEARTPULSE_PIN_SIG);
    
}

void loop() 
{
  if(count_hb > (Delay_in / s20)){
    int heartpulseBPM = heartpulse.BPM;
    Serial.println(heartpulseBPM);

    if (heartpulse.QS == true) {
    Serial.println("PULSE");
    heartpulse.QS = false;
    }

    count_hb = 0;
  } 

  heartpulse.every20s();
  count_hb ++;
  delay(s20);
  }