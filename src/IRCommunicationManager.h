#ifndef IR_COMMUNICATION_MANAGER_H
#define IR_COMMUNICATION_MANAGER_H

#include "Rf.h"
#include "Communication.h"
#include "Logging.h"
#include "stdint.h"
#include "Config.h"

void SendRFPergolaCode(String side, String pergolaNewState)
{
  if (Firebase.ready())
  {
    Firebase.getInt(Communication::fbdo, "/" + side + "/" + DataBaseArgs::Remote);
    uint32_t remote = Communication::fbdo.to<uint32_t>();
    Firebase.getInt(Communication::fbdo, "/" + side + "/" + DataBaseArgs::RollingCode);
    uint32_t rollingCode = Communication::fbdo.to<uint32_t>();
    PrintInfo("Remote: ");
    PrintInfoLn(remote);
    PrintInfo("RollingCode: ");
    PrintInfoLn(rollingCode);

    uint8_t frame[7];

    if (pergolaNewState == DataBaseArgs::PergolaOpen)
    {
      BuildFrame(frame, RFParameters::OPEN, remote, &rollingCode);
    }
    else if (pergolaNewState == DataBaseArgs::PergolaClose)
    {
      BuildFrame(frame, RFParameters::CLOSE, remote, &rollingCode);
    }
    else
    {
      BuildFrame(frame, RFParameters::PROG, remote, &rollingCode);
    }

    SendCommand(frame, 2);

    for(int i = 0; i<2; i++) 
    {
        SendCommand(frame, 7);
    }

    UpdateRollingCodeInFirebase(side, rollingCode);
  }
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  String pergolaNewState = param.asInt() == 1 ? DataBaseArgs::PergolaOpen : DataBaseArgs::PergolaClose;
  PrintInfoLn(pergolaNewState);

  SendRFPergolaCode(DataBaseArgs::LeftPergola, pergolaNewState);
  UpdateBlynkApp(DataBaseArgs::LeftPergola, pergolaNewState);
  UpdatePergolaStateInFirebase(DataBaseArgs::LeftPergola, pergolaNewState);
}

// This function is called every time the Virtual Pin 3 state changes
BLYNK_WRITE(V3)
{
  String pergolaNewState = param.asInt() == 1 ? DataBaseArgs::PergolaOpen : DataBaseArgs::PergolaClose;
  PrintInfoLn(pergolaNewState);

  SendRFPergolaCode(DataBaseArgs::RightPergola, pergolaNewState);
  UpdateBlynkApp(DataBaseArgs::RightPergola, pergolaNewState);
  UpdatePergolaStateInFirebase(DataBaseArgs::RightPergola, pergolaNewState);
}

// For program
BLYNK_WRITE(V5)
{
  uint32_t program = param.asInt();;

  if (program)
  {
    SendRFPergolaCode(DataBaseArgs::RightPergola, "Program");
  }
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
  SendRFPergolaCode(DataBaseArgs::LeftPergola, pergolaState);
  UpdateBlynkApp(DataBaseArgs::LeftPergola, pergolaState);

  pergolaState = GetPegorlaState(DataBaseArgs::RightPergola);
  SendRFPergolaCode(DataBaseArgs::RightPergola, pergolaState);
  UpdateBlynkApp(DataBaseArgs::RightPergola, pergolaState);
}

#endif