#include <Arduino.h>
#include "Config.h"

void PhotoResistorProcessing()
{
    int ldrStatus = analogRead(ConfigPins::photoResistorPin);   //read the status of the LDR value

    //check if the LDR status is <= 300
    if (ldrStatus <=300)
    {
        digitalWrite(ConfigPins::buildInLedPin, LOW); 
        Serial.println("LDR is DARK, LED is ON");
    }
    else 
    {
        digitalWrite(ConfigPins::buildInLedPin, HIGH); 
        Serial.println("LDR is LIGHT, LED is OFF");
    }
}