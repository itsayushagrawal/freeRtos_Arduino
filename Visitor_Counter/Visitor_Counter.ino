#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void updateDisplay();

void setup() 
{
  Serial.begin(9600);
  pinMode(A0,INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IN: 0   OUT: 0");
  lcd.setCursor(0, 1);
  lcd.print("Current: 0");

  updateDisplay();
}

const uint8_t in_pin = 3;
const uint8_t out_pin = 2;
const uint8_t timeout = 500;

uint8_t in_count= 0;
uint8_t out_count = 0;
int8_t current_count = 0;

void check_in()
{ 
  unsigned long startTimeIN= millis();
  while(millis()-startTimeIN < timeout)
  {
    if(digitalRead(out_pin)==1)
    {
      in_count++;
      updateDisplay();
      break;
    }
  }
}

void baldev()
{
  lcd.setCursor(0, 0);
  lcd.print("Baldev          ");
  lcd.setCursor(0, 1);
  lcd.print("The Destroyer   ");
}

void updateDisplay()
{
  lcd.setCursor(4, 0);
  lcd.print("   ");
  lcd.setCursor(4, 0);
  lcd.print(in_count);
  lcd.setCursor(13, 0);
  lcd.print("   ");
  lcd.setCursor(13, 0);
  lcd.print(out_count);
  current_count = in_count - out_count;
  if(current_count < 0)
    baldev();
  else
  {
    lcd.setCursor(9, 1);
    lcd.print("   ");
    lcd.setCursor(9, 1);
    lcd.print(current_count);
  }
}

void check_out()
{
  unsigned long startTimeOUT= millis();
  while(millis()-startTimeOUT < timeout)
  {
    if(digitalRead(in_pin)==1)
    {
      out_count++;
      updateDisplay();
      break;
    }
  }
}

void loop() 
{
  if(digitalRead(in_pin)==1)
  {
    check_in();
  }
  if(digitalRead(out_pin)==1)
  {
    check_out();
  }
}
