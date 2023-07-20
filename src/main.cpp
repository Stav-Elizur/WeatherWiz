#include <Arduino.h>
#include "Communication.h"
#include "IRCommunicationManager.h"
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
  pinMode(ConfigPins::RelayOutputPin, OUTPUT);
  pinMode(ConfigPins::RelayOutputPin, HIGH);
  digitalWrite(ConfigPins::buildInLedPin, HIGH); 

  // Blynk
  InitConnection();

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
}