#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <BlynkSimpleEsp8266.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include "Config.h"

// #define ENABLE_BLYNK

// Blynk global variables
BlynkTimer timer;

// Firebase global variables
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData fbdo;


// Functions
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
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

void BlynkProcessing()
{
  Blynk.run();
  timer.run();
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  digitalWrite(ConfigPins::buildInLedPin, !digitalRead(ConfigPins::buildInLedPin)); 
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);

  if (Firebase.ready())
  {
    Firebase.setString(fbdo, F("/Led State"), digitalRead(ConfigPins::buildInLedPin) ? "OFF" : "ON");
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
    Firebase.setString(fbdo, F("/Led State"), "OFF");
}

#endif