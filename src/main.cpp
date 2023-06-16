// C++ code
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define ENABLE_BLYNK

// #define DEBUG
#ifdef DEBUG
#define PrintDebug(...) Serial.print(__VA_ARGS__)
#define PrintDebugLn(...) Serial.println(__VA_ARGS__)
#else
#define PrintDebug(...)
#define PrintDebugLn(...)
#endif

#define INFO
#ifdef INFO
#define PrintInfo(...) Serial.print(__VA_ARGS__)
#define PrintInfoLn(...) Serial.println(__VA_ARGS__)
#else
#define PrintInfo(...)
#define PrintInfoLn(...)
#endif

const int buildInLedPin = D9;
const int infraRedSensorInputPin=D8;
const int infraRedSensorOutputPin=D7;
const int humidityInputPin=D6;

// Blynk
const char* ssid = "Elizur"; //replace this with your WiFi network name
const char* password = "0547684611"; //replace this with your WiFi network password
const char* blynkToken =  "oht3MaP6XwE4ToLGNU1Wte1_BkM3JRa7";
BlynkTimer timer;

//IR Receiver stuff
IRrecv irrecv(infraRedSensorInputPin);

//IR Transmitter stuff
const uint16_t frequency = 38000;
IRsend irsend(infraRedSensorOutputPin);

// RGB - Test
const uint8_t numOfBlueColorBits = 32;
const uint64_t blueCode = 0xF7609F;
const decode_type_t blueCodeType = NEC;

const uint8_t numOfGreenColorBits = 32;
const uint64_t greenCode= 0xF7A05F;
const decode_type_t greenCodeType = NEC;

const uint8_t numOfRedColorBits = 32;
const uint64_t redCode= 0xF720DF;
const decode_type_t redCodeType = NEC;

// Humidity stuff
DHTesp dht;

// Functions
// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void EnableBlynk()
{
  Blynk.begin(blynkToken, ssid, password);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void BlynkProcessing()
{
  Blynk.run();
  timer.run();
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  digitalWrite(buildInLedPin, !digitalRead(buildInLedPin)); 
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// Testing  
void TransmitIRSignal(decode_type_t type, void* data, uint32_t numOfData)
{
  switch (type) 
  {
    case NEC:
    {
      PrintInfoLn("NEC remote control");
      uint64_t code = *((uint64_t*)data);
      irsend.sendNEC(code, numOfData);
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
  switch(value){
  case 0xFFA25D:  
    PrintInfoLn("CH-            "); 
    break;
  case 0xFF629D:  
    PrintInfoLn("CH             "); 
    TransmitIRSignal(redCodeType, (void*)&redCode, numOfRedColorBits);
    break;
  case 0xFFE21D:  
    PrintInfoLn("CH+            "); 
    break;
  case 0xFF22DD:  
    PrintInfoLn("|<<          "); 
    break;
  case 0xFF02FD:  
    PrintInfoLn(">>|        "); 
    TransmitIRSignal(greenCodeType, (void*)&greenCode, numOfGreenColorBits);
    break;
  case 0xFFC23D:  
    PrintInfoLn("PLAY/PAUSE     "); 
    break;
  case 0xFFE01F:  
    PrintInfoLn("VOL-           "); 
    break;
  case 0xFFA857:  
    PrintInfoLn("VOL+           "); 
    TransmitIRSignal(blueCodeType, (void*)&blueCode, numOfBlueColorBits);
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

void HumidityProcessing()
{
  delay(dht.getMinimumSamplingPeriod());

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

void setup()
{
  Serial.begin(9600);
  pinMode(buildInLedPin, OUTPUT);
  digitalWrite(buildInLedPin, HIGH); 

  // Blynk
  #ifdef ENABLE_BLYNK
    EnableBlynk();
  #endif

  // InfraRed
  PrintInfoLn(F("Enabling IRin..."));
  irrecv.enableIRIn(); // Start the receiver
  irsend.begin(); // Start the transmitter
  PrintInfoLn("Enabled IRin");

  // Humidity
  pinMode(humidityInputPin, INPUT);
  dht.setup(humidityInputPin, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
}

void loop()
{  
  BlynkProcessing();
  IRProcessing();
  HumidityProcessing();
  {    
    // // PhotoResistor
    // // read the value from the sensor
    // sensorValue = analogRead(photoResistorPin);
    // // print the sensor reading so you know its range
    // Serial.println(sensorValue);
    // // map the sensor reading to a range for the LED
    // analogWrite(9, map(sensorValue, 0, 1023, 0, 255));
    // // Wait for 100 millisecond(s)
    // delay(100);
    
    // // Relay
    // digitalWrite(relayPin, HIGH);
    // // Wait for 1000 millisecond(s)
    // delay(1000);
    // digitalWrite(relayPin, LOW);
    // // Wait for 1000 millisecond(s)
    // delay(1000);
  }
}