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
        digitalWrite(ConfigPins::RelayOutputPin, HIGH);  // Relay
        PrintInfoLn("LDR is DARK, LED is ON");
    }
    else 
    {
        digitalWrite(ConfigPins::RelayOutputPin, LOW); // Relay
        PrintInfoLn("LDR is LIGHT, LED is OFF");
    }
}

#endif