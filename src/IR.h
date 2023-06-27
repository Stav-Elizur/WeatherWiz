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
        PrintInfoLn("Sony remote control");
        // Add your Sony remote control handling code here
        break;
    case RC5:
        PrintInfoLn("RC5 remote control");
        // Add your RC5 remote control handling code here
        break;
    case RC6:
        PrintInfoLn("RC6 remote control");
        // Add your RC6 remote control handling code here
        break;
    case DISH:
        PrintInfoLn("DISH remote control");
        // Add your DISH remote control handling code here
        break;
    case SHARP:
        PrintInfoLn("Sharp remote control");
        // Add your Sharp remote control handling code here
        break;
    // Add more cases for other IR remote control types as needed
    default:
        PrintInfoLn("Unknown remote control type");
        // Add your default handling code here
        break;
    }
}

void translateIR(uint64_t value) // takes action based on IR code received describing Car MP3 IR codes 
{
    std::string colorTransmittingIR = "";
    switch(value){
    case 0xFFA25D:  
        PrintInfoLn("CH-            "); 
        break;
    case 0xFF629D:  
        PrintInfoLn("CH             "); 
        colorTransmittingIR = "Red";
        break;
    case 0xFFE21D:  
        PrintInfoLn("CH+            "); 
        break;
    case 0xFF22DD:  
        PrintInfoLn("|<<          "); 
        break;
    case 0xFF02FD:  
        PrintInfoLn(">>|        "); 
        colorTransmittingIR = "Green";
        break;
    case 0xFFC23D:  
        PrintInfoLn("PLAY/PAUSE     "); 
        break;
    case 0xFFE01F:  
        PrintInfoLn("VOL-           "); 
        break;
    case 0xFFA857:  
        PrintInfoLn("VOL+           "); 
        colorTransmittingIR = "Blue";
        break;
    case 0xFF906F:  
        PrintInfoLn("EQ             "); 
        break;
    case 0xFF6897:  
        PrintInfoLn("0              "); 
        break;
    case 0xFF9867:  
        PrintInfoLn("100+           "); 
        break;
    case 0xFFB04F:  
        PrintInfoLn("200+           "); 
        break;
    case 0xFF30CF:  
        PrintInfoLn("1              "); 
        break;
    case 0xFF18E7:  
        PrintInfoLn("2              "); 
        break;
    case 0xFF7A85:  
        PrintInfoLn("3              "); 
        break;
    case 0xFF10EF:  
        PrintInfoLn("4              "); 
        break;
    case 0xFF38C7:  
        PrintInfoLn("5              "); 
        break;
    case 0xFF5AA5:  
        PrintInfoLn("6              "); 
        break;
    case 0xFF42BD:  
        PrintInfoLn("7              "); 
        break;
    case 0xFF4AB5:  
        PrintInfoLn("8              "); 
        break;
    case 0xFF52AD:  
        PrintInfoLn("9              "); 
        break;
    default: 
        PrintInfo("unknown button: ");
        PrintInfoLn(value, HEX);
    }

    delay(500);

    if (Firebase.ready() && colorTransmittingIR != "")
    {
        Firebase.getInt(fbdo, "/" + colorTransmittingIR + "/CodeType");
        decode_type_t CodeType = (decode_type_t)(fbdo.to<uint32_t>());
        Firebase.getInt(fbdo, "/" + colorTransmittingIR + "/Code");
        uint32_t Code = fbdo.to<uint32_t>();
        Firebase.getInt(fbdo, "/" + colorTransmittingIR + "/BitNumbers");
        uint32_t numOfColorBits = fbdo.to<uint32_t>();
        PrintInfo("Code type: ");
        PrintInfoLn(CodeType);
        PrintInfo("Code: ");
        PrintInfoLn(Code);
        PrintInfo("numOfColorBits: ");
        PrintInfoLn(numOfColorBits);
        TransmitIRSignal(CodeType, (void*)&Code, numOfColorBits);
    }
}

void IRProcessing()
{
    decode_results results;
    // InfraRed
    if (irrecv.decode(&results)) {
    PrintDebugLn("-------------------------");
    PrintDebugLn(results.value, HEX);
    PrintDebugLn(results.bits, DEC);
    PrintDebugLn(results.decode_type, DEC);
    // Convert the results into an array suitable for sendRaw().
    // resultToRawArray() allocates the memory we need for the array.
    uint16_t *raw_array = resultToRawArray(&results);

    // Find out how many elements are in the array.
    uint16_t length = getCorrectedRawLength(&results);

    PrintDebugLn("Received IR signal:");
    PrintDebug(F("#define RAW_DATA_LEN "));
    PrintDebugLn(length, DEC);

    PrintDebugLn(F("uint16_t rawData[RAW_DATA_LEN]={"));
    for (uint32_t i = 0; i < length; i++) 
    {
        PrintDebug(raw_array[i], DEC);
        PrintDebug(F(", "));
        if (((i+1) % 8) == 0) 
        {
        PrintDebug(F("\n"));
        }
    }
        PrintDebugLn(F("};"));
        translateIR(results.value);
        PrintDebugLn("-------------------------");
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