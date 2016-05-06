/*

  Caravan Thermostat TX

  Copyright 2016 Jon Burrows

*/

// Setup the includes
#include <Wire.h>
#include <SPI.h>
#include <TFT.h>  // Arduino LCD library
#include <dht.h> // DHT11 Library
#include <TimeLib.h>
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
int ReqTemp = 28; // Failsafe to 0 incase of powerloss. This may well be changed.
int TempStartPointX = 5; //44 Center
int TempStartPointY = 35;
int TimeStartPointX = 5;
int TimeStartPointY = TFTscreen.height() - 20;
int ReqTempStartPointX = TFTscreen.width() - 75;
int ReqTempStartPointY = TFTscreen.height() - 32;
int PreviousHour = 0;
int PreviousMinute = 0;
String PreviousDate = "";

// Color definitions
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */
#define Pink            0xF81F
#define Brown           0x8A22
#define OffBlack        0x3186

const unsigned char PROGMEM CampFireFlames[] =
{
  B00000000, B00000010, B00000000, B00000000, B00000000, //               #
  B00000000, B00000011, B11100000, B00000000, B00000000, //               #####
  B00000000, B00000011, B00011000, B00000000, B00000000, //               ##   ##
  B00000000, B00000001, B00000100, B00000000, B00000000, //                #     #
  B00000000, B00000001, B10000010, B00000000, B00000000, //                ##     #
  B00000000, B00000000, B10000001, B00000000, B00000000, //                 #      #
  B00000000, B00000000, B10000001, B00000000, B00000000, //                 #      #
  B00000000, B00000000, B10000000, B10000000, B00000000, //                 #       #
  B00000000, B00000000, B10000000, B10000000, B00000000, //                 #       #
  B00000000, B00000000, B10000000, B10000000, B00000000, //                 #       #
  B00000000, B00000001, B10000000, B10011000, B00000000, //                ##       #  ##
  B00000000, B00000001, B00000001, B00011100, B00000000, //                #       #   ###
  B00000000, B00000001, B00000001, B00001011, B00000000, //                #       #    # ##
  B00000000, B00000010, B00000001, B00011001, B00000000, //               #        #   ##  #
  B00000011, B10000110, B00000001, B00010000, B10000000, //       ###    ##        #   #    #
  B00000001, B11001100, B00000001, B11110000, B10000000, //        ###  ##         #####    #
  B00000001, B01111000, B00000000, B01000000, B01000000, //        # ####            #       #
  B00000001, B00110000, B00000000, B00000000, B01000000, //        #  ##                     #
  B00000001, B00000000, B00000000, B00000000, B10000000, //        #                        #
  B00000001, B00000000, B00000000, B00000000, B10000000, //        #                        #
  B00000011, B00000000, B00110000, B00000001, B00000000, //       ##          ##           #
  B00000010, B00000000, B00110000, B00000001, B00000000, //       #           ##           #
  B00000100, B00000000, B00111000, B00000001, B00000000, //      #            ###          #
  B00001000, B00000000, B00111000, B00000001, B00000000, //     #             ###          #
  B00011000, B00000000, B00101100, B00000000, B10000000, //    ##             # ##          #
  B00010000, B00000000, B01001100, B00000000, B11000000, //    #             #  ##          ##
  B00010000, B00000000, B10001000, B00000000, B01100000, //    #            #   #            ##
  B00110000, B00000001, B10001000, B00000000, B00100000, //   ##           ##   #             #
  B00010000, B00000011, B00011000, B10000000, B00010000, //    #          ##   ##   #          #
  B00110000, B00000110, B00010000, B11000000, B00010000, //   ##         ##    #    ##         #
  B00010000, B00000100, B00110000, B11000000, B00001000, //    #         #    ##    ##          #
  B00010000, B00001100, B00110000, B11100000, B00001000, //    #        ##    ##    ###         #
  B00010000, B00001000, B00010001, B11100000, B00001000, //    #        #      #   ####         #
  B00001000, B00001000, B00001111, B11110000, B00001000, //     #       #       ########        #
  B00001100, B00001000, B00000000, B01110000, B00001000, //     ##      #            ###        #
  B00000100, B00001000, B00000000, B01110000, B00001000, //      #      #            ###        #
  B00000010, B00001000, B00000000, B00010000, B00010000, //       #     #              #       #
  B00000011, B00001100, B00000000, B00010000, B00010000, //       ##    ##             #       #
  B00000001, B00000100, B00000000, B00000000, B00100000, //        #     #                    #
  B00000000, B01000110, B00000000, B00110000, B01000000, //          #   ##           ##     #
  B00000000, B00110011, B00000000, B00100001, B10000000, //           ##  ##          #    ##
  B00000000, B00011111, B10000000, B01000111, B00000000, //            ######        #   ###
  B00000000, B00000111, B11111111, B11110100, B00000000, //              ############### #
  B00000000, B00000000, B11111111, B11100000, B00000000, //                 ###########
};
const unsigned char PROGMEM CampFireLogs[] =
{
  B00001100, B00000000, B00000000, B00000000, B00011100, //     ##                             ###
  B00011001, B11110000, B00000000, B00000111, B11110110, //    ##  #####                 ####### ##
  B00010000, B00011111, B10000000, B11111100, B00000011, //    #       ######       ######        ##
  B00010000, B00000011, B11111101, B10000000, B00000010, //    #          ######## ##             #
  B00011110, B00001111, B11110000, B00000000, B00011110, //    ####     ########               ####
  B00001111, B11111100, B00000000, B00000011, B11011000, //     ##########                #### ##
  B00111111, B00000000, B00000000, B11111111, B11110000, //   ######                ############
  B01000000, B00000000, B00011111, B01000000, B00011000, //  #                 ##### #         ##
  B11000000, B00000101, B11110110, B11000000, B00001000, // ##           # ##### ## ##          #
  B11000001, B11111000, B00000000, B01111110, B00001000, // ##     ######            ######     #
  B01111111, B10000000, B00000000, B00000011, B01111000, //  ########                     ## ####
  B00000000, B00000000, B00000000, B00000000, B00100000, //                                   #
};

void setup() {
  Serial.begin(115200);
  setSyncProvider(RTC.get);
  SetupDisplayLayout();

}

void loop() {
  // Read the Temperature
  ReadTemperature();
  delay(2000);
  ReadTime();
}

// Define Functions.
void SetupDisplayLayout() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);

  // Draw the relevant static content.
  //oC
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
  //Serial.println("Read Temp");
  switch (chk)
  {
    case DHTLIB_OK:
      //Serial.println("Temp OK");
      double sensorVal = double(DHT.temperature);
      dtostrf(sensorVal, 2, 0, CurrentTemperature);
      if (memcmp(CurrentTemperature, PreviousTemperature, 4) != 0)
      {
        UpdateTemperature();
        // Do we need to turn the heating on?
        if (sensorVal < ReqTemp - 1)  {
          HeatingOn = true;
        }
        if (sensorVal > ReqTemp + 1) {
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
    TFTscreen.text(CurrentDate().c_str(), TimeStartPointX + 75, TimeStartPointY+5);
    PreviousDate = CurrentDate();
  }
  TimeToDisplay += Time2Digit(hour());
  TimeToDisplay += ":";
  TimeToDisplay += Time2Digit(minute());
  TimeToDisplay += ":";
  TimeToDisplay += Time2Digit(second());
  Serial.println("Time is " + TimeToDisplay);

  // Display the time on the screen.

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
  DateToDisplay += Time2Digit(year()-2000);
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
  DisplayTop(Pos, Black);
  DisplayTopLeft(Pos, Black);
  DisplayCenter(Pos, Black);
  DisplayBottom(Pos, Black);
  DisplayBottomLeft(Pos, Black);
}
void Display2(int Pos)
{
  Display8(Pos);
  DisplayTopLeft(Pos, Black);
  DisplayBottomRight(Pos, Black);
}

void Display3(int Pos)
{
  Display8(Pos);
  DisplayTopLeft(Pos, Black);
  DisplayBottomLeft(Pos, Black);
}
void Display4(int Pos)
{
  Display8(Pos);
  DisplayTop(Pos, Black);
  DisplayBottomLeft(Pos, Black);
  DisplayBottom(Pos, Black);
}
void Display5(int Pos)
{
  Display8(Pos);
  DisplayTopRight(Pos, Black);
  DisplayBottomLeft(Pos, Black);
}
void Display6(int Pos)
{
  Display8(Pos);
  DisplayTopRight(Pos, Black);
}
void Display7(int Pos)
{
  Display8(Pos);
  DisplayTopLeft(Pos, Black);
  DisplayCenter(Pos, Black);
  DisplayBottomLeft(Pos, Black);
  DisplayBottom(Pos, Black);
}

void Display8(int Pos)
{
  DisplayTop(Pos, White);
  DisplayTopLeft(Pos, White);
  DisplayTopRight(Pos, White);
  DisplayCenter(Pos, White);
  DisplayBottom(Pos, White);
  DisplayBottomLeft(Pos, White);
  DisplayBottomRight(Pos, White);
}
void Display9(int Pos)
{
  Display8(Pos);
  DisplayBottomLeft(Pos, Black);
}
void Display0(int Pos)
{
  Display8(Pos);
  DisplayCenter(Pos, Black);
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

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}


