#ifndef IR_H
#define IR_H

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include "Config.h"
#include "Logging.h"
#include "Communication.h"

//IR Receiver global variables
IRrecv irrecv(ConfigPins::infraRedSensorInputPin);

//IR Transmitter global variables
const uint16_t frequency = 38000;
IRsend irsend(ConfigPins::infraRedSensorOutputPin);

// Functions
void TransmitIRSignal(decode_type_t type, void* data, uint32_t numOfData)
{
    switch (type) 
    {
    case NEC:
    {
        PrintInfoLn("NEC remote control");
        uint64_t code = *((uint64_t*)data);
        irsend.sendNEC(code, numOfData, 3);
        break;
    }
    case SONY:
        PrintInfoLn("Sony remote control - NOT SUPPORTED");
        // Add your Sony remote control handling code here
        break;
    case RC5:
        PrintInfoLn("RC5 remote control - NOT SUPPORTED");
        // Add your RC5 remote control handling code here
        break;
    case RC6:
        PrintInfoLn("RC6 remote control - NOT SUPPORTED");
        // Add your RC6 remote control handling code here
        break;
    case DISH:
        PrintInfoLn("DISH remote control - NOT SUPPORTED");
        // Add your DISH remote control handling code here
        break;
    case SHARP:
        PrintInfoLn("Sharp remote control - NOT SUPPORTED");
        // Add your Sharp remote control handling code here
        break;
    // Add more cases for other IR remote control types as needed
    default:
        PrintInfoLn("Unknown remote control type - NOT SUPPORTED");
        // Add your default handling code here
        break;
    }
}

// takes action based on IR code received describing Car MP3 IR codes 
void translateIR(uint64_t value)
{
    const auto pergolaLeftOpenCode = GetPergolaCode(DataBaseArgs::LeftPergola, DataBaseArgs::PergolaOpen);
    const auto pergolaLeftCloseCode = GetPergolaCode(DataBaseArgs::LeftPergola, DataBaseArgs::PergolaClose);
    const auto pergolaRightOpenCode = GetPergolaCode(DataBaseArgs::RightPergola, DataBaseArgs::PergolaOpen);
    const auto pergolaRightCloseCode = GetPergolaCode(DataBaseArgs::RightPergola, DataBaseArgs::PergolaClose);

    if (value == pergolaLeftOpenCode)
    {
        UpdateBlynkApp(DataBaseArgs::LeftPergola, DataBaseArgs::PergolaOpen);
    }
    else if (value == pergolaLeftCloseCode)
    {
        UpdateBlynkApp(DataBaseArgs::LeftPergola, DataBaseArgs::PergolaClose); 
    }
    else if(value == pergolaRightOpenCode)
    {
        UpdateBlynkApp(DataBaseArgs::RightPergola, DataBaseArgs::PergolaOpen);
    }
    else if(value == pergolaRightCloseCode)
    {
        UpdateBlynkApp(DataBaseArgs::RightPergola, DataBaseArgs::PergolaClose);
    }
    else{
        PrintInfo("unknown button: ");
        PrintInfoLn(value, HEX);
    }
}

void IRProcessing()
{
    decode_results results;
    if (irrecv.decode(&results)) {
        translateIR(results.value);
        irrecv.resume();
    }
}

void InitIR()
{
    PrintInfoLn("Enabling IRin...");
    irrecv.enableIRIn(); // Start the receiver
    irsend.begin(); // Start the transmitter
    PrintInfoLn("Enabled IRin");
}

#endif