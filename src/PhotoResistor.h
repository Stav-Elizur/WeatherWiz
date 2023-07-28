#ifndef PHOTO_RESISTOR_H
#define PHOTO_RESISTOR_H

#include <Arduino.h>
#include "Config.h"
#include "Logging.h"

void PhotoResistorProcessing()
{
    int ldrStatus = analogRead(ConfigPins::photoResistorPin);   //read the status of the LDR value

    //check if the LDR status is <= 300
    if (ldrStatus <=300)
    {
        pinMode(ConfigPins::RelayOutputPin, OUTPUT); // Relay
        digitalWrite(ConfigPins::buildInLed, HIGH);  
        PrintInfoLn("LDR is DARK, LED is ON");
    }
    else 
    {
        pinMode(ConfigPins::RelayOutputPin, INPUT); // Relay
        digitalWrite(ConfigPins::buildInLed, LOW); 
        PrintInfoLn("LDR is LIGHT, LED is OFF");
    }
}

#endif