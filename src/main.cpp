// C++ code
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>

// #define DEBUG
#ifdef DEBUG
#define PrintDebug(...) Serial.println(__VA_ARGS__)
#else
#define PrintDebug(...)
#endif

#define INFO
#ifdef INFO
#define PrintInfo(...) Serial.println(__VA_ARGS__)
#else
#define PrintInfo(...)
#endif

const int buildInLedPin = D9;
const int infraRedSensorInputPin=D8;
const int infraRedSensorOutputPin=D7;

//IR Receiver stuff
IRrecv irrecv(infraRedSensorInputPin);

//IR Transmitter stuff
const uint16_t frequency = 38000;
IRsend irsend(infraRedSensorOutputPin);

// RGB - Test
const uint8_t numOfBlueColorBits = 67;
const uint64_t blueCode = 0xF7609F;
const decode_type_t blueCodeType = NEC;

const uint8_t numOfGreenColorBits = 32;
const uint64_t greenCode= 0xF7A05F;
const decode_type_t greenCodeType = NEC;

const uint8_t numOfRedColorBits = 32;
const uint64_t redCode= 0xF720DF;
const decode_type_t redCodeType = NEC;

// Testing  
void TransmitIRSignal(decode_type_t type, void* data, uint32_t numOfData)
{
  switch (type) 
  {
    case NEC:
    {
      PrintInfo("NEC remote control");
      uint64_t code = *((uint64_t*)data);
      irsend.sendNEC(code, numOfData);
      break;
    }
    case SONY:
      PrintInfo("Sony remote control");
      // Add your Sony remote control handling code here
      break;
    case RC5:
      PrintInfo("RC5 remote control");
      // Add your RC5 remote control handling code here
      break;
    case RC6:
      PrintInfo("RC6 remote control");
      // Add your RC6 remote control handling code here
      break;
    case DISH:
      PrintInfo("DISH remote control");
      // Add your DISH remote control handling code here
      break;
    case SHARP:
      PrintInfo("Sharp remote control");
      // Add your Sharp remote control handling code here
      break;
    // Add more cases for other IR remote control types as needed
    default:
      PrintInfo("Unknown remote control type");
      // Add your default handling code here
      break;
  }
}

void translateIR(uint64_t value) // takes action based on IR code received describing Car MP3 IR codes 
{
  switch(value){
  case 0xFFA25D:  
    PrintInfo(" CH-            "); 
    break;
  case 0xFF629D:  
    PrintInfo(" CH             "); 
    TransmitIRSignal(redCodeType, (void*)&redCode, numOfRedColorBits);
    break;
  case 0xFFE21D:  
    PrintInfo(" CH+            "); 
    break;
  case 0xFF22DD:  
    PrintInfo(" |<<          "); 
    break;
  case 0xFF02FD:  
    PrintInfo(" >>|        "); 
    TransmitIRSignal(greenCodeType, (void*)&greenCode, numOfGreenColorBits);
    break;
  case 0xFFC23D:  
    PrintInfo(" PLAY/PAUSE     "); 
    break;
  case 0xFFE01F:  
    PrintInfo(" VOL-           "); 
    break;
  case 0xFFA857:  
    PrintInfo(" VOL+           "); 
    TransmitIRSignal(blueCodeType, (void*)&blueCode, numOfBlueColorBits);
    break;
  case 0xFF906F:  
    PrintInfo(" EQ             "); 
    break;
  case 0xFF6897:  
    PrintInfo(" 0              "); 
    break;
  case 0xFF9867:  
    PrintInfo(" 100+           "); 
    break;
  case 0xFFB04F:  
    PrintInfo(" 200+           "); 
    break;
  case 0xFF30CF:  
    PrintInfo(" 1              "); 
    break;

  case 0xFF18E7:  
    PrintInfo(" 2              "); 
    break;

  case 0xFF7A85:  
    PrintInfo(" 3              "); 
    break;

  case 0xFF10EF:  
    PrintInfo(" 4              "); 
    break;

  case 0xFF38C7:  
    PrintInfo(" 5              "); 
    break;

  case 0xFF5AA5:  
    PrintInfo(" 6              "); 
    break;

  case 0xFF42BD:  
    PrintInfo(" 7              "); 
    break;

  case 0xFF4AB5:  
    PrintInfo(" 8              "); 
    break;

  case 0xFF52AD:  
    PrintInfo(" 9              "); 
    break;

  default: 
    PrintInfo(" unknown button   ");
    PrintInfo("------------------");
    PrintInfo(value, HEX);
  }

  delay(500);
}

void IRProcessing()
{
  decode_results results;
  // InfraRed
  if (irrecv.decode(&results)) {
    PrintDebug("\n\n-------------------------");
    PrintDebug(results.value, HEX);
    PrintDebug(results.bits, DEC);
    PrintDebug(results.decode_type, DEC);
    // Convert the results into an array suitable for sendRaw().
    // resultToRawArray() allocates the memory we need for the array.
    uint16_t *raw_array = resultToRawArray(&results);

    // Find out how many elements are in the array.
    uint16_t length = getCorrectedRawLength(&results);

    PrintDebug("Received IR signal:");
    PrintDebug(F("\n#define RAW_DATA_LEN "));
    PrintDebug(length, DEC);

    PrintDebug(F("uint16_t rawData[RAW_DATA_LEN]={\n"));
    for (uint32_t i = 0; i < length; i++) 
    {
      PrintDebug(raw_array[i], DEC);
      PrintDebug(F(", "));
      if (((i+1) % 8) == 0) 
      {
        PrintDebug(F("\n"));
      }
    }
      PrintDebug(F("};"));
      translateIR(results.value);
      PrintDebug("-------------------------");
      irrecv.resume();
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(buildInLedPin, OUTPUT);

  // InfraRed
  PrintInfo(F("Enabling IRin..."));
  irrecv.enableIRIn(); // Start the receiver
  irsend.begin(); // Start the transmitter
  PrintInfo("Enabled IRin");
}

void loop()
{  
  IRProcessing();
  

  delay(1000);
  digitalWrite(buildInLedPin, !digitalRead(buildInLedPin)); 
  {
    // // Temperator Sensor
    // RawValue = analogRead(temperaturePin);
    // Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
    // tempC = (Voltage-500) * 0.1; // 500 is the offset
    // tempF = (tempC * 1.8) + 32; // convert to F  
    // Serial.print("Raw Value = " );                  
    // Serial.print(RawValue);      
    // Serial.print("\t milli volts = ");
    // Serial.print(Voltage,0); //
    // Serial.print("\t Temperature in C = ");
    // Serial.print(tempC,1);
    // Serial.print("\t Temperature in F = ");
    // Serial.println(tempF,1);
    // humiditysensorOutput = analogRead(temperaturePin);
    // Serial.print("Humidity: "); // Printing out Humidity Percentage
    // Serial.print(map(humiditysensorOutput, 0, 1023, 10, 70));
    // Serial.println("%");
    // delay(1000);  //iterate every 5 seconds
    
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