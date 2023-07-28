#ifndef HUMIDITY_H
#define HUMIDITY_H

#include "DHTesp.h"
#include "Logging.h"
#include "Config.h"

// Humidity global variables
DHTesp dht;

// Functions
void HumidityProcessing()
{
  delay(2 * dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  PrintInfo(dht.getStatusString());
  PrintInfo("\t");
  PrintInfo(humidity, 1);
  PrintInfo("\t\t");
  PrintInfoLn(temperature, 1);
  PrintDebug("\t\t");
  PrintDebug(dht.toFahrenheit(temperature), 1);
  PrintDebug("\t\t");
  PrintDebug(dht.computeHeatIndex(temperature, humidity, false), 1);
  PrintDebug("\t\t");
  PrintDebugLn(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
}

void InitHumidity()
{
  pinMode(ConfigPins::humidityInputPin, INPUT);
  dht.setup(ConfigPins::humidityInputPin, DHTesp::DHT11);
}

#endif