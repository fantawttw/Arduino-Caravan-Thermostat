/*

  Caravan Thermostat RX

  Copyright 2016 Jon Burrows

*/

// Setup the includes
#include <SPI.h>
#include <SoftwareSerial.h>

// Define the pins.
// BLE

#define BLERXPIN 2
#define BLETXPIN 3

// Create instances
SoftwareSerial BTserial(BLERXPIN, BLETXPIN); // RX, TX

// Define Global Variables

void setup() {
  Serial.begin(9600);
Serial.println("Enter AT commands:");
  BTserial.begin(9600);
  at("AT+NAMECaravanRX");
  //at("AT+CON0015830084F7"); //Connect to Master Arduino
  at("AT+ROLE0");
  at("AT+RESET");
  
  // Master Address 00:15:83:00:84:F7
  // Slave Address 00:15:83:00:83:EE
  delay(1000); // wait a bit, NECESSARY!!
}

void loop() {
 // Keep reading from HC-06 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        Serial.write(BTserial.read());
    }
 
    // Keep reading from Arduino Serial Monitor and send to HC-06
    if (Serial.available())
    {
        BTserial.write(Serial.read());
    }
}

void at(char* cmd) {
  BTserial.write(cmd);
  BTserial.write("\r\n");
  Serial.print(cmd);
  while(!BTserial.find("OK")) Serial.print(".");
  
  Serial.println(" .. OK");
}
