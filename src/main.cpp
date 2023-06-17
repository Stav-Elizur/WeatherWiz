#include <Arduino.h>
#include "Communication.h"
#include "IR.h"
#include "Humidity.h"
#include "Config.h"
#include "Logging.h"

void setup()
{
  Serial.begin(9600);
  pinMode(ConfigPins::buildInLedPin, OUTPUT);
  digitalWrite(ConfigPins::buildInLedPin, HIGH); 

  // Blynk
  #ifdef ENABLE_BLYNK
    InitConnection();
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
  {    
    // // PhotoResistor
    // // read the value from the sensor
    // sensorValue = analogRead(photoResistorPin);
    // // print the sensor reading so you know its range
    // Serial.println(sensorValue);
    // // map the sensor reading to a range for the LED
    // analogWrite(9, map(sensorValue, 0, 1023, 0, 255));
    // // Wait for 100 millisecond(s)
    // delay(100);
    
    // // Relay
    // digitalWrite(relayPin, HIGH);
    // // Wait for 1000 millisecond(s)
    // delay(1000);
    // digitalWrite(relayPin, LOW);
    // // Wait for 1000 millisecond(s)
    // delay(1000);
  }
}