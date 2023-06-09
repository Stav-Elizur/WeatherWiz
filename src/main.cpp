// C++ code

// #define receiveMode
#define RAW_DATA_LEN 67

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>

const int buildInLedPin = D9;
const int infraRedSensorPin=D7;

//IR Library stuff
#ifdef receiveMode
  IRrecv irrecv(infraRedSensorPin);
#else
  IRsend irsend(infraRedSensorPin);
  uint16_t rawData[RAW_DATA_LEN]={
  8680, 4210, 590, 1558, 622, 458, 622, 1532, 
  622, 458, 624, 458, 622, 1530, 620, 458, 
  624, 1530, 620, 458, 626, 1528, 622, 456, 
  624, 1532, 594, 1556, 622, 458, 622, 1532, 
  622, 456, 624, 458, 626, 456, 624, 1532, 
  620, 1530, 596, 1556, 596, 482, 598, 484, 
  622, 458, 598, 1556, 622, 1528, 622, 458, 
  626, 454, 626, 456, 624, 1532, 622, 1528, 
  624, 1528, 622};
  const uint16_t kFrequency = 38000;
#endif
decode_results results;
void translateIR();

void setup()
{
  Serial.begin(9600);
  pinMode(buildInLedPin, OUTPUT);

  // InfraRed
  Serial.println(F("Enabling IRin..."));

  #ifdef receiveMode
    irrecv.enableIRIn(); // Start the receiver
  #else
    irsend.begin(); // Start the transmitter
  #endif

  Serial.println("Enabled IRin");
}

void loop()
{  
  // InfraRed
  #ifdef receiveMode
    if (irrecv.decode(&results)) {
      Serial.println("\n\n-------------------------");
      Serial.println(results.value, HEX);
      Serial.println(results.bits, DEC);
      Serial.println(results.decode_type, DEC);
      // Convert the results into an array suitable for sendRaw().
      // resultToRawArray() allocates the memory we need for the array.
      uint16_t *raw_array = resultToRawArray(&results);

      // Find out how many elements are in the array.
      uint16_t length = getCorrectedRawLength(&results);

      Serial.println("Received IR signal:");
      Serial.print(F("\n#define RAW_DATA_LEN "));
      Serial.println(length, DEC);

      Serial.print(F("uint16_t rawData[RAW_DATA_LEN]={\n"));
      for (uint32_t i = 0; i < length; i++) {
        Serial.print(raw_array[i], DEC);
        Serial.print(F(", "));
        if (((i+1) % 8) == 0) {
          Serial.print(F("\n"));
        }
      }
      Serial.println(F("};"));
      translateIR();
      Serial.println("-------------------------");
      irrecv.resume();
    }
    delay(100); 
  #else
    irsend.sendRC6(0xA55A38C7L, 32, 0);
    irsend.sendRaw(rawData, RAW_DATA_LEN, 38);
    delay(5000);
    digitalWrite(buildInLedPin, !digitalRead(buildInLedPin)); 
  #endif

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

void translateIR() // takes action based on IR code received describing Car MP3 IR codes 
{
  switch(results.value){
  case 0xFFA25D:  
    Serial.println(" CH-            "); 
    break;
  case 0xFF629D:  
    Serial.println(" CH             "); 
    break;
  case 0xFFE21D:  
    Serial.println(" CH+            "); 
    break;
  case 0xFF22DD:  
    Serial.println(" blue LED off          "); 
    digitalWrite(13,LOW);
    break;
  case 0xFF02FD:  
    Serial.println(" blue LED on        "); 
    digitalWrite(13, HIGH);
    break;
  case 0xFFC23D:  
    Serial.println(" PLAY/PAUSE     "); 
    break;
  case 0xFFE01F:  
    Serial.println(" VOL-           "); 
    break;
  case 0xFFA857:  
    Serial.println(" VOL+           "); 
    break;
  case 0xFF906F:  
    Serial.println(" EQ             "); 
    break;
  case 0xFF6897:  
    Serial.println(" 0              "); 
    break;
  case 0xFF9867:  
    Serial.println(" 100+           "); 
    break;
  case 0xFFB04F:  
    Serial.println(" 200+           "); 
    break;
  case 0xFF30CF:  
    Serial.println(" 1              "); 
    break;

  case 0xFF18E7:  
    Serial.println(" 2              "); 
    break;

  case 0xFF7A85:  
    Serial.println(" 3              "); 
    break;

  case 0xFF10EF:  
    Serial.println(" 4              "); 
    break;

  case 0xFF38C7:  
    Serial.println(" 5              "); 
    break;

  case 0xFF5AA5:  
    Serial.println(" 6              "); 
    break;

  case 0xFF42BD:  
    Serial.println(" 7              "); 
    break;

  case 0xFF4AB5:  
    Serial.println(" 8              "); 
    break;

  case 0xFF52AD:  
    Serial.println(" 9              "); 
    break;

  default: 
    Serial.print(" unknown button   ");
    Serial.println(results.value, HEX);

  }

  delay(500);
} 