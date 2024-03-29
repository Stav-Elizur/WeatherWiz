#include <Arduino.h>
#include "Communication.h"
#include "ControllerManager.h"
#include "Humidity.h"
#include "PhotoResistor.h"
#include "Config.h"
#include "Logging.h"

void setup()
{
  Serial.begin(9600);
  pinMode(ConfigPins::photoResistorPin, INPUT);
  pinMode(ConfigPins::buildInLed, OUTPUT);
  pinMode(ConfigPins::rfOutputPin, OUTPUT);
  digitalWrite(ConfigPins::rfOutputPin, LOW);

  // Blynk
  InitConnection();

  // Humidity
  InitHumidity();
}

void loop()
{
  BlynkProcessing();
  HumidityProcessing();
  PhotoResistorProcessing();
}