void DisplayTop(int Pos, uint16_t Color)
{
  for (uint8_t  i = 0; i <= 6; i++) {
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


void DisplayDigit(int DigitToDisplay, int Pos)
{
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
