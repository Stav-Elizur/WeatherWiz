#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <BlynkSimpleEsp8266.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include "Config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define ENABLE_BLYNK

// Blynk global variables
const char* PERGOLA_STATE = " Pergola State";
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


// Functions
void myTimerEvent()
{
  timeClient.update();
  Blynk.virtualWrite(V2, timeClient.getFormattedTime());
}

void EnableBlynk()
{
  Blynk.begin(ConfigWirelessConnection::BLYNK_TOKEN, ConfigWirelessConnection::WIFI_SSID, ConfigWirelessConnection::WIFI_PASSWORD);
  timer.setInterval(1000L, myTimerEvent);
}

void ConfigFirebase()
{
  config.api_key = ConfigWirelessConnection::FIREBASE_TOKEN;
  auth.user.email = ConfigWirelessConnection::USER_EMAIL;
  auth.user.password = ConfigWirelessConnection::USER_PASSWORD;
  config.database_url = ConfigWirelessConnection::FIREBASE_URL;

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
}

void ConfigNTP()
{
  timeClient.begin();
}

void BlynkProcessing()
{
  Blynk.run();
  timer.run();
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  digitalWrite(ConfigPins::buildInLedPin, !digitalRead(ConfigPins::buildInLedPin)); 
  
  String pergolaState = digitalRead(ConfigPins::buildInLedPin) ? "OFF" : "ON";

  // Update state
  Blynk.virtualWrite(V1, pergolaState);

  if (Firebase.ready())
  {
    Firebase.setString(fbdo, F("/PERGOLA_STATE"), pergolaState);
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void InitConnection()
{
    EnableBlynk();
    ConfigFirebase();

    String pergolaState = "";
    Firebase.getString(fbdo, F("/PERGOLA_STATE"), &pergolaState);

    digitalWrite(ConfigPins::buildInLedPin, pergolaState == "ON" ? LOW : HIGH);
    Blynk.virtualWrite(V0, pergolaState == "ON" ? 1 : 0);
    Blynk.virtualWrite(V1, pergolaState);
}

#endif