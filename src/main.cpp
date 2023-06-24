#include <Arduino.h>
#include "Communication.h"
#include "IR.h"
#include "Humidity.h"
#include "PhotoResistor.h"
#include "Config.h"
#include "Logging.h"

void setup()
{
  Serial.begin(9600);
  pinMode(ConfigPins::buildInLedPin, OUTPUT);
  pinMode(ConfigPins::photoResistorPin, INPUT);
  digitalWrite(ConfigPins::buildInLedPin, HIGH); 

  // Blynk
  #ifdef ENABLE_BLYNK
    InitConnection();
  #else
    digitalWrite(ConfigPins::buildInLedPin, LOW); 
  #endif

  // InfraRed
  InitIR();

  // Humidity
  InitHumidity();
}

void loop()
{
  BlynkProcessing();
  IRProcessing();
  HumidityProcessing();
  PhotoResistorProcessing();

  #ifndef ENABLE_BLYNK
    digitalWrite(ConfigPins::buildInLedPin, !digitalRead(ConfigPins::buildInLedPin));  
  #endif
  
  delay(1000);
  {  
    // // Relay
    // digitalWrite(relayPin, HIGH);
    // // Wait for 1000 millisecond(s)
    // delay(1000);
    // digitalWrite(relayPin, LOW);
    // // Wait for 1000 millisecond(s)
    // delay(1000);
  }
}