/*

  Caravan Thermostat TX

  Copyright 2016 Jon Burrows

*/

// Setup the includes
#include <SPI.h>
#include <TFT.h>  // Arduino LCD library
#include <dht.h> // DHT11 Library
#include <DS1307RTC.h> // RTC Library
//#include <IRremote.h> // IR Library

// Define the pins.

// LCD
#define cs   10
#define dc   9
#define rst  8

// DHT11
#define DHT11PIN 2

// IR Receiver
#define IRPIN 5

// Create instances
TFT TFTscreen = TFT(cs, dc, rst);

// Define Global Variables

void setup() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  SetupDisplayLayout();
}

void loop() {
  // put your main code here, to run repeatedly:

}

// Define Functions.
void SetupDisplayLayout() {
}

