#ifndef IR_COMMUNICATION_MANAGER_H
#define IR_COMMUNICATION_MANAGER_H

#include "IR.h"
#include "Communication.h"
#include "Logging.h"
#include "stdint.h"
#include "Config.h"

void SendIRPergolaCode(String side, String pergolaNewState)
{
  if (Firebase.ready())
  {
    Firebase.getInt(Communication::fbdo, "/" + side + "/" + pergolaNewState + "/CodeType");
    decode_type_t CodeType = (decode_type_t)(Communication::fbdo.to<uint32_t>());
    Firebase.getInt(Communication::fbdo, "/" + side + "/" + pergolaNewState + "/Code");
    uint32_t Code = Communication::fbdo.to<uint32_t>();
    Firebase.getInt(Communication::fbdo, "/" + side + "/" + pergolaNewState + "/BitNumbers");
    uint32_t numOfColorBits = Communication::fbdo.to<uint32_t>();
    PrintInfo("Code type: ");
    PrintInfoLn(CodeType);
    PrintInfo("Code: ");
    PrintInfoLn(Code);
    PrintInfo("numOfColorBits: ");
    PrintInfoLn(numOfColorBits);

    TransmitIRSignal(CodeType, (void*)&Code, numOfColorBits);
  }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  String pergolaNewState = param.asString();
  PrintInfoLn(pergolaNewState);

  SendIRPergolaCode(DataBaseArgs::LeftPergola, pergolaNewState);
  UpdateBlynkApp(DataBaseArgs::LeftPergola, pergolaNewState);
  UpdatePergolaStateInFirebase(DataBaseArgs::LeftPergola, pergolaNewState);
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V3)
{
  String pergolaNewState = param.asString();
  PrintInfoLn(pergolaNewState);

  SendIRPergolaCode(DataBaseArgs::RightPergola, pergolaNewState);
  UpdateBlynkApp(DataBaseArgs::RightPergola, pergolaNewState);
  UpdatePergolaStateInFirebase(DataBaseArgs::RightPergola, pergolaNewState);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  PrintInfoLn("Blynk connected");
}

void InitConnection()
{
  EnableBlynk();
  ConfigFirebase();

  String pergolaState = GetPegorlaState(DataBaseArgs::LeftPergola);
  SendIRPergolaCode(DataBaseArgs::LeftPergola, pergolaState);
  UpdateBlynkApp(DataBaseArgs::LeftPergola, pergolaState);

  pergolaState = GetPegorlaState(DataBaseArgs::RightPergola);
  SendIRPergolaCode(DataBaseArgs::RightPergola, pergolaState);
  UpdateBlynkApp(DataBaseArgs::RightPergola, pergolaState);
}

#endif