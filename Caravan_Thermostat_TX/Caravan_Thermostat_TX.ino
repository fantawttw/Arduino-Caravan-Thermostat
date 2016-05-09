/*

  Caravan Thermostat TX

  Copyright 2016 Jon Burrows

*/

// Setup the includes
#include <SPI.h>
#include <TFT.h>  // Arduino LCD library
#include <dht.h> // DHT11 Library
#include <TimeLib.h>
#include <DS1307RTC.h> // RTC Library
#include <IRremote.h> // IR Library
#include <RF24.h>

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
IRrecv irrecv(IRPIN);
decode_results results;
//RF24 radio(7,8);

// Define Global Variables
bool radioNumber = 0; //Master is 0, Slave is 1

char PreviousTemperature[4];
char PreviousHumidity[4];
char CurrentTemperature[4];
char CurrentHumidity[4];
bool HeatingOn = false;
int ReqTemp = 17; // Failsafe incase of powerloss.
int TempStartPointX = 5; //44 Center
int TempStartPointY = 35;
int TimeStartPointX = 5;
int TimeStartPointY = TFTscreen.height() - 20;
int ReqTempStartPointX = TFTscreen.width() - 75;
int ReqTempStartPointY = TFTscreen.height() - 32;
int PreviousHour = 0;
int PreviousMinute = 0;
String PreviousDate = "";
uint32_t IRCheckTime = 0;

char *MessageOfTheDay[] =
{
  "Love you Baby Boo x x x",

};
char *FamilyName[] =
{
  "Jon",
  "Lisa",
  "Zak",
  "Max",
  "Ty",
};
char *SeasonGreetings[] =
{
  "Merry Christmas",
  "Happy New Year",
  "Happy Birthday",
};


// Define custom include files.
#include "DefineColors.h"
#include "DisplayDigits.h"
#include "CampFireGFX.h"

void setup() {
  Serial.begin(115200);
  setSyncProvider(RTC.get);
  SetupDisplayLayout();
  irrecv.enableIRIn();
}

void loop() {
  // Read the Temperature
  ReadTemperature();
  ReadTime();
  CheckIR(); // This has the delay of 2000ms
}

void CheckIR()
{
  IRCheckTime = millis() + 2000;
  while (IRCheckTime > millis())
  {
    if (irrecv.decode(&results)) {
      TFTscreen.setTextSize(1);
      TFTscreen.stroke(Black);
      String TempRequested = "";
      TempRequested += ReqTemp;
      TFTscreen.text(TempRequested.c_str(), ReqTempStartPointX, ReqTempStartPointY);
      Serial.println(results.value, HEX);
      Serial.println(results.value);
      if (results.value == 2579) //Down
      {
        ReqTemp--;
      }
      if (results.value == 530) //Up
      {
        ReqTemp++;
      }
      irrecv.resume(); // Receive the next value
      memset(PreviousTemperature, 0, sizeof(PreviousTemperature));
      memset(PreviousHumidity, 0, sizeof(PreviousHumidity));
      TempRequested = "";
      TempRequested += ReqTemp;
      TFTscreen.stroke(White);
      TFTscreen.text(TempRequested.c_str(), ReqTempStartPointX, ReqTempStartPointY);
    }
  }
}

// Define Functions.
void SetupDisplayLayout() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(White);
  TFTscreen.setTextSize(1);
  TFTscreen.text("o", TempStartPointX + 76, TempStartPointY - 8);
  String TempRequested = "";
  TempRequested += ReqTemp;
  TFTscreen.text(TempRequested.c_str(), ReqTempStartPointX, ReqTempStartPointY);

  TFTscreen.setTextSize(2);
  TFTscreen.text("c", TempStartPointX + 80, TempStartPointY - 4);
  TFTscreen.text(":", TimeStartPointX + 23, TimeStartPointY);

  TFTscreen.drawLine(0, TFTscreen.height() - 24, TFTscreen.width(), TFTscreen.height() - 24, OffBlack);
  TFTscreen.drawLine(0, 24, TFTscreen.width(), 24, OffBlack);
  TFTscreen.drawLine(TFTscreen.width() - 60, 24, TFTscreen.width() - 60, TFTscreen.height() - 24, OffBlack);
  DrawFire(HeatingOn);
}

void ReadTemperature()
{
  int chk = DHT.read11(DHT11PIN);
  switch (chk)
  {
    case DHTLIB_OK:
      double sensorVal = double(DHT.temperature);
      dtostrf(sensorVal, 2, 0, CurrentTemperature);
      if (memcmp(CurrentTemperature, PreviousTemperature, 4) != 0)
      {
        UpdateTemperature();
        // Do we need to turn the heating on?
        if (sensorVal < ReqTemp - 1)  {
          HeatingOn = true;
        }
        if (sensorVal >= ReqTemp) {
          HeatingOn = false;
        }
        DrawFire(HeatingOn);
      }
      sensorVal = double(DHT.humidity);
      dtostrf(sensorVal, 2, 0, CurrentHumidity);
  }
}

void ReadTime()
{
  String TimeToDisplay = "";
  if (PreviousHour != hour())
  {
    TimeToDisplay = "";
    TFTscreen.setTextSize(2);
    TFTscreen.stroke(Black);
    TimeToDisplay += Time2Digit(PreviousHour);
    TFTscreen.text(TimeToDisplay.c_str(), TimeStartPointX, TimeStartPointY);

    TimeToDisplay = "";
    TimeToDisplay += Time2Digit(hour());
    TFTscreen.stroke(White);
    TFTscreen.text(TimeToDisplay.c_str(), TimeStartPointX, TimeStartPointY);

    PreviousHour = hour();
  }
  if (PreviousMinute != minute())
  {
    TimeToDisplay = "";
    TFTscreen.setTextSize(2);
    TFTscreen.stroke(Black);
    TimeToDisplay += Time2Digit(PreviousMinute);
    TFTscreen.text(TimeToDisplay.c_str(), TimeStartPointX + 35, TimeStartPointY);

    TimeToDisplay = "";
    TimeToDisplay += Time2Digit(minute());
    TFTscreen.stroke(White);
    TFTscreen.text(TimeToDisplay.c_str(), TimeStartPointX + 35, TimeStartPointY);

    PreviousMinute = minute();
  }
  if (PreviousDate != CurrentDate())
  {
    TFTscreen.setTextSize(1);
    TFTscreen.text(CurrentDate().c_str(), TimeStartPointX + 75, TimeStartPointY + 5);
    PreviousDate = CurrentDate();
    // Update Message Of The Day
    UpdateMOTD();
  }
}

void UpdateMOTD()
{
  // We could check for birthdays in here:)
  TFTscreen.setTextSize(1);
  // Need to clear the box out first.
  TFTscreen.fillRect(0, 0, TFTscreen.width(), 23, Black);
  if (day() == 25 && month() == 12)
  {
    TFTscreen.text(SeasonGreetings[0], 0, 5);
  }
  else if (day() == 1 && month() == 1)
  {
    TFTscreen.text(SeasonGreetings[1], 0, 5);
  }
  else if (day() == 17 && month() == 10)
  {
    TFTscreen.text(SeasonGreetings[2], 0, 5);
    TFTscreen.text(FamilyName[0], 90, 5);
  }
  else
  {
    TFTscreen.text(MessageOfTheDay[0], 0, 5);
  }
}
String CurrentDate()
{
  String DateToDisplay = "";
  DateToDisplay += dayShortStr(weekday());
  DateToDisplay += " ";
  DateToDisplay += Time2Digit(day());
  DateToDisplay += "/";
  DateToDisplay += Time2Digit(month());
  DateToDisplay += "/";
  DateToDisplay += Time2Digit(year() - 2000);
  return DateToDisplay;
}
void DrawFire(bool Flame)
{
  //Do we need flame?
  if (Flame)
  {
    TFTscreen.drawBitmap(TFTscreen.width() - 45, 36, CampFireFlames, 40, 44, Red);
  }
  else
  {
    TFTscreen.drawBitmap(TFTscreen.width() - 45, 36, CampFireFlames, 40, 44, Black);
  }

  TFTscreen.drawBitmap(TFTscreen.width() - 45, 80, CampFireLogs, 40, 12, Brown);
}

String Time2Digit(int Number)
{
  String StringToReturn = "";
  if (Number >= 0 && Number < 10) {
    StringToReturn += "0";
  }
  StringToReturn += Number;
  return StringToReturn;
}

void UpdateTemperature()
{
  DisplayDigit(CurrentTemperature[0], 0);
  DisplayDigit(CurrentTemperature[1], 40);
  memcpy(PreviousTemperature, CurrentTemperature, 2);
}



