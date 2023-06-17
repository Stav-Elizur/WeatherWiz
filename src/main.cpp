// C++ code
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include "Config.h"

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
BlynkTimer timer;

// Firebase
FirebaseData fbdo;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

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
  Blynk.begin(Config::BLYNK_TOKEN, Config::WIFI_SSID, Config::WIFI_PASSWORD);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void ConfigFirebase()
{
  FirebaseAuth auth;
  FirebaseConfig config;

  config.api_key = Config::FIREBASE_TOKEN;
  auth.user.email = Config::USER_EMAIL;
  auth.user.password = Config::USER_PASSWORD;
  config.database_url = Config::FIREBASE_URL;

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
    ConfigFirebase();
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
  
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    Serial.printf("Set bool... %s\n", Firebase.setBool(fbdo, F("/test/bool"), count % 2 == 0) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get bool... %s\n", Firebase.getBool(fbdo, FPSTR("/test/bool")) ? fbdo.to<bool>() ? "true" : "false" : fbdo.errorReason().c_str());

    bool bVal;
    Serial.printf("Get bool ref... %s\n", Firebase.getBool(fbdo, F("/test/bool"), &bVal) ? bVal ? "true" : "false" : fbdo.errorReason().c_str());

    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, F("/test/int"), count) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, F("/test/int")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());

    int iVal = 0;
    Serial.printf("Get int ref... %s\n", Firebase.getInt(fbdo, F("/test/int"), &iVal) ? String(iVal).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set float... %s\n", Firebase.setFloat(fbdo, F("/test/float"), count + 10.2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get float... %s\n", Firebase.getFloat(fbdo, F("/test/float")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set double... %s\n", Firebase.setDouble(fbdo, F("/test/double"), count + 35.517549723765) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get double... %s\n", Firebase.getDouble(fbdo, F("/test/double")) ? String(fbdo.to<double>()).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/test/string"), "Hello World!") ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get string... %s\n", Firebase.getString(fbdo, F("/test/string")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

    // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Parse_Edit.ino
    FirebaseJson json;

    if (count == 0)
    {
      json.set("value/round/" + String(count), F("cool!"));
      json.set(F("vaue/ts/.sv"), F("timestamp"));
      Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/test/json"), json) ? "ok" : fbdo.errorReason().c_str());
    }
    else
    {
      json.add(String(count), "smart!");
      Serial.printf("Update node... %s\n", Firebase.updateNode(fbdo, F("/test/json/value/round"), json) ? "ok" : fbdo.errorReason().c_str());
    }

    Serial.println();

    // For generic set/get functions.

    // For generic set, use Firebase.set(fbdo, <path>, <any variable or value>)

    // For generic get, use Firebase.get(fbdo, <path>).
    // And check its type with fbdo.dataType() or fbdo.dataTypeEnum() and
    // cast the value from it e.g. fbdo.to<int>(), fbdo.to<std::string>().

    // The function, fbdo.dataType() returns types String e.g. string, boolean,
    // int, float, double, json, array, blob, file and null.

    // The function, fbdo.dataTypeEnum() returns type enum (number) e.g. fb_esp_rtdb_data_type_null (1),
    // fb_esp_rtdb_data_type_integer, fb_esp_rtdb_data_type_float, fb_esp_rtdb_data_type_double,
    // fb_esp_rtdb_data_type_boolean, fb_esp_rtdb_data_type_string, fb_esp_rtdb_data_type_json,
    // fb_esp_rtdb_data_type_array, fb_esp_rtdb_data_type_blob, and fb_esp_rtdb_data_type_file (10)

    count++;
  }

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