#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <BlynkSimpleEsp8266.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include "Config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Logging.h"

namespace Communication
{
  // Blynk global variables
  BlynkTimer timer;

  // Firebase global variables
  FirebaseAuth auth;
  FirebaseConfig config;
  FirebaseData fbdo;

  // NTP global variables
  const long utcOffsetInSeconds = 3 * 3600;
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
}

// Functions
void myTimerEvent()
{
  Communication::timeClient.update();
  Blynk.virtualWrite(V2, Communication::timeClient.getFormattedTime());
}

void EnableBlynk()
{
  Blynk.begin(ConfigWirelessConnection::BLYNK_TOKEN, ConfigWirelessConnection::WIFI_SSID, ConfigWirelessConnection::WIFI_PASSWORD);
  Communication::timer.setInterval(1000L, myTimerEvent);
}

void ConfigFirebase()
{
  Communication::config.api_key = ConfigWirelessConnection::FIREBASE_TOKEN;
  Communication::auth.user.email = ConfigWirelessConnection::USER_EMAIL;
  Communication::auth.user.password = ConfigWirelessConnection::USER_PASSWORD;
  Communication::config.database_url = ConfigWirelessConnection::FIREBASE_URL;

  Communication::config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&Communication::config, &Communication::auth);
}

void ConfigNTP()
{
  Communication::timeClient.begin();
}

void BlynkProcessing()
{
  Blynk.run();
  Communication::timer.run();
}

void UpdateBlynkApp(String side, String pergolaState)
{
  if (Firebase.ready())
  {
    if (side == DataBaseArgs::LeftPergola)
    {
        Blynk.virtualWrite(V0, pergolaState == DataBaseArgs::PergolaOpen ? 1 : 0);
        Blynk.virtualWrite(V1, pergolaState);
    }
    else
    {
        Blynk.virtualWrite(V3, pergolaState == DataBaseArgs::PergolaOpen ? 1 : 0);
        Blynk.virtualWrite(V4, pergolaState);
    }
  }
}

void UpdatePergolaStateInFirebase(String side, String pergolaNewState)
{
  if (Firebase.ready())
  {
    Firebase.setString(Communication::fbdo,  "/" + side + "/" + DataBaseArgs::PergolaState, pergolaNewState);
  }
}

void UpdateRollingCodeInFirebase(String side, uint32_t rollingCode)
{
  if (Firebase.ready())
  {
    Firebase.setInt(Communication::fbdo,  "/" + side + "/" + DataBaseArgs::RollingCode, rollingCode);
  }
}

String GetPegorlaState(String side)
{
    String pergolaState = "";

    if (Firebase.ready())
    {
        Firebase.getString(Communication::fbdo, "/" + side + "/" + DataBaseArgs::PergolaState, &pergolaState);
    }

    return pergolaState;
}

#endif