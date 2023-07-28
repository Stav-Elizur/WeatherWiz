#include <stdint.h>
#include <Arduino.h>
#include "config.h"


void BuildFrame(uint8_t* frame, 
                uint8_t button, 
                uint32_t remote,
                uint32_t* rollingCode) 
{
  unsigned int code = *rollingCode;
  
  frame[0] = 0xA7; // Encryption key. Doesn't matter much
  frame[1] = button << 4;  // Which button did  you press? The 4 LSB will be the checksum
  frame[2] = code >> 8;    // Rolling code (big endian)
  frame[3] = code;         // Rolling code
  frame[4] = remote >> 16; // Remote address
  frame[5] = remote >>  8; // Remote address
  frame[6] = remote;       // Remote address

  Serial.print("Frame         : ");
  for(byte i = 0; i < 7; i++) {
    if(frame[i] >> 4 == 0) { //  Displays leading zero in case the most significant
      Serial.print("0");     // nibble is a 0.
    }
    Serial.print(frame[i],HEX); Serial.print(" ");
  }
  
// Checksum calculation: a XOR of all the nibbles
  uint8_t checksum = 0;
  for(byte i = 0; i < 7; i++) {
    checksum = checksum ^ frame[i] ^ (frame[i] >> 4);
  }
  checksum &= 0b1111; // We keep the last 4 bits only


//Checksum integration
  frame[1] |= checksum; //  If a XOR of all the nibbles is equal to 0, the blinds will
                        // consider the checksum ok.

  Serial.println(""); Serial.print("With checksum : ");
  for(byte i = 0; i < 7; i++) {
    if(frame[i] >> 4 == 0) {
      Serial.print("0");
    }
    Serial.print(frame[i],HEX); Serial.print(" ");
  }

  
// Obfuscation: a XOR of all the bytes
  for(byte i = 1; i < 7; i++) {
    frame[i] ^= frame[i-1];
  }

  Serial.println(""); Serial.print("Obfuscated    : ");
  for(byte i = 0; i < 7; i++) {
    if(frame[i] >> 4 == 0) {
      Serial.print("0");
    }
    Serial.print(frame[i],HEX); Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Rolling Code  : "); Serial.println(code);

  *rollingCode = code + 1;                                      // Arduino function takes care of it.
}

void SendCommand(uint8_t* frame, uint8_t sync) {
if(sync == 2) { // Only with the first frame.
    //Wake-up pulse & Silence
    digitalWrite(ConfigPins::rfOutputPin, HIGH);
    delayMicroseconds(9415);
    digitalWrite(ConfigPins::rfOutputPin, LOW);
    delayMicroseconds(89565);
  }

  // Hardware sync: two sync for the first frame, seven for the following ones.
  for (int i = 0; i < sync; i++) {
    digitalWrite(ConfigPins::rfOutputPin, HIGH);
    delayMicroseconds(4*RFParameters::SYMBOL);
    digitalWrite(ConfigPins::rfOutputPin, LOW);
    delayMicroseconds(4*RFParameters::SYMBOL);
  }

  // Software sync
  digitalWrite(ConfigPins::rfOutputPin, HIGH);
  delayMicroseconds(4550);
  digitalWrite(ConfigPins::rfOutputPin, LOW);
  delayMicroseconds(RFParameters::SYMBOL);
  
  
  //Data: bits are sent one by one, starting with the MSB.
  for(byte i = 0; i < 56; i++) {
    if(((frame[i/8] >> (7 - (i%8))) & 1) == 1) {
      digitalWrite(ConfigPins::rfOutputPin, LOW);
      delayMicroseconds(RFParameters::SYMBOL);
      digitalWrite(ConfigPins::rfOutputPin, HIGH);
      delayMicroseconds(RFParameters::SYMBOL);
    } else {
      digitalWrite(ConfigPins::rfOutputPin, HIGH);
      delayMicroseconds(RFParameters::SYMBOL);
      digitalWrite(ConfigPins::rfOutputPin, LOW);
      delayMicroseconds(RFParameters::SYMBOL);
    }
  }
  
  digitalWrite(ConfigPins::rfOutputPin, LOW);
  delayMicroseconds(30415); // Inter-frame silence
}