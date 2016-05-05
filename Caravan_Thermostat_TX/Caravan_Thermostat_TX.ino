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
dht DHT;

// Define Global Variables
char PreviousTemperature[4];
char PreviousHumidity[4];
char CurrentTemperature[4];
char CurrentHumidity[4];
bool HeatingOn = false;
double ReqTemp = 0; // Failsafe to 0 incase of powerloss. This may well be changed.
int TempStartPointX = 44;
int TempStartPointY = 40;

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

void setup() {
  Serial.begin(115200);
  SetupDisplayLayout();
}

void loop() {
  // Read the Temperature
  ReadTemperature();

  // wait for a bit.
  delay(2000);
}

// Define Functions.
void SetupDisplayLayout() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);

  // Draw the relevant static content.
  //oC
  TFTscreen.stroke(WHITE);
  TFTscreen.setTextSize(1);
  TFTscreen.text("o", 120, 25);
  TFTscreen.setTextSize(2);
  TFTscreen.text("c", 124, 29);
}

void ReadTemperature()
{
  int chk = DHT.read11(DHT11PIN);
  //Serial.println("Read Temp");
  switch (chk)
  {
    case DHTLIB_OK:
      //Serial.println("Temp OK");
      double sensorVal = double(DHT.temperature);
      dtostrf(sensorVal, 2, 0, CurrentTemperature);
      //Serial.println(PreviousTemperature);
      //Serial.println(CurrentTemperature);
      sensorVal = double(DHT.humidity);
      dtostrf(sensorVal, 2, 0, CurrentHumidity);
      if (memcmp(CurrentTemperature, PreviousTemperature, 4) != 0)
      {
        UpdateTemperature();
      }
  }
}
void UpdateTemperature()
{
  Serial.println("Temp Diff");
  DisplayDigit(CurrentTemperature[0], 0);
  DisplayDigit(CurrentTemperature[1], 40);
  memcpy(PreviousTemperature, CurrentTemperature, 2);
}

void DisplayDigit(int DigitToDisplay, int Pos)
{
  Serial.println(DigitToDisplay);
  switch (DigitToDisplay)
  {
    case 48 : Display0(Pos); break;
    case 49 : Display1(Pos); break;
    case 50 : Display2(Pos); break;
    case 51 : Display3(Pos); break;
    case 52 : Display4(Pos); break;
    case 53 : Display5(Pos); break;
    case 54 : Display6(Pos); break;
    case 55 : Display7(Pos); break;
    case 56 : Display8(Pos); break;
    case 57 : Display9(Pos); break;
  }
}

void Display1(int Pos)
{
  Display8(Pos);
  DisplayTop(Pos, 0x0000);
  DisplayTopLeft(Pos, 0x0000);
  DisplayCenter(Pos, 0x0000);
  DisplayBottom(Pos, 0x0000);
  DisplayBottomLeft(Pos, 0x0000);
}
void Display2(int Pos)
{
  Display8(Pos);
  DisplayTopLeft(Pos, 0x0000);
  DisplayBottomRight(Pos, 0x0000);
}

void Display3(int Pos)
{
  Display8(Pos);
  DisplayTopLeft(Pos, 0x0000);
  DisplayBottomLeft(Pos, 0x0000);
}
void Display4(int Pos)
{
  Display8(Pos);
  DisplayTop(Pos, 0x0000);
  DisplayBottomLeft(Pos, 0x0000);
  DisplayBottom(Pos, 0x0000);
}
void Display5(int Pos)
{
  Display8(Pos);
  DisplayTopRight(Pos, 0x0000);
  DisplayBottomLeft(Pos, 0x0000);
}
void Display6(int Pos)
{
  Display8(Pos);
  DisplayTopRight(Pos, 0x0000);
}
void Display7(int Pos)
{
  Display8(Pos);
  DisplayTopLeft(Pos, 0x0000);
  DisplayCenter(Pos, 0x0000);
  DisplayBottomLeft(Pos, 0x0000);
  DisplayBottom(Pos, 0x0000);
}

void Display8(int Pos)
{
  DisplayTop(Pos, 0xFFFF);
  DisplayTopLeft(Pos, 0xFFFF);
  DisplayTopRight(Pos, 0xFFFF);
  DisplayCenter(Pos, 0xFFFF);
  DisplayBottom(Pos, 0xFFFF);
  DisplayBottomLeft(Pos, 0xFFFF);
  DisplayBottomRight(Pos, 0xFFFF);
}
void Display9(int Pos)
{
  Display8(Pos);
  DisplayBottomLeft(Pos, 0x0000);
}
void Display0(int Pos)
{
  Display8(Pos);
  DisplayCenter(Pos, 0x0000);
}
void DisplayTop(int Pos, uint16_t Color)
{
  for (int i = 0; i <= 6; i++) {
    TFTscreen.drawLine(TempStartPointX + Pos + i, TempStartPointY + i, TempStartPointX + Pos + 33 - i, TempStartPointY + i, Color);
  }
}

void DisplayTopLeft(int Pos, uint16_t Color)
{
  for (int i = 0; i <= 6; i++) {
    TFTscreen.drawLine(TempStartPointX + Pos + i, TempStartPointY + 4 + i, TempStartPointX + Pos + i, TempStartPointY + 28 - i, Color);
  }
}

void DisplayTopRight(int Pos, uint16_t Color)
{
  for (int i = 6; i >= 0; i--) {
    TFTscreen.drawLine(TempStartPointX + Pos + 34 - i, TempStartPointY + 4 + i, TempStartPointX + Pos + 34 - i, TempStartPointY + 28 - i, Color);
  }
}
void DisplayCenter(int Pos, uint16_t Color)
{
  for (int i = 0; i <= 3; i++) {
    TFTscreen.drawLine(TempStartPointX + Pos + 3 + i, TempStartPointY + 29 + i, TempStartPointX + Pos + 30 - i, TempStartPointY + 29 + i, Color);
  }
  for (int i = 3; i >= 0; i--) {
    TFTscreen.drawLine(TempStartPointX + Pos + 3 + i, TempStartPointY + 28 - i, TempStartPointX + Pos + 30 - i, TempStartPointY + 28 - i, Color);
  }

}
void DisplayBottom(int Pos, uint16_t Color)
{
  for (int i = 6; i >= 0; i--) {
    TFTscreen.drawLine(TempStartPointX + Pos + i, TempStartPointY + 56 - i, TempStartPointX + Pos + 33 - i, TempStartPointY + 56 - i, Color);
  }
}
void DisplayBottomLeft(int Pos, uint16_t Color)
{
  for (int i = 0; i <= 6; i++) {
    TFTscreen.drawLine(TempStartPointX + Pos + i, TempStartPointY + 29 + i, TempStartPointX + Pos + i, TempStartPointY + 53 - i, Color);
  }
}

void DisplayBottomRight(int Pos, uint16_t Color)
{
  for (int i = 6; i >= 0; i--) {
    TFTscreen.drawLine(TempStartPointX + Pos + 34 - i, TempStartPointY + 29 + i, TempStartPointX + Pos + 34 - i, TempStartPointY + 53 - i, Color);
  }
}

