//Alarm Clock Code
/*
  Parts:
  LCD Dsiplay
  Real Time Clock
  Passive Buzzer
*/

//Libraries
#include <LiquidCrystal.h>
#include "pitches.h"
#include <Wire.h>
#include <DS3231.h>
#include "LedControl.h"



//Constant initialization
const int BUZZER = 3;

/*LED Matrix*/
const int DIN_PIN = 4, CS_PIN = 5, CLK_PIN = 6, LED_DISPLAY = 1, INTENSITY = 0;

/*LCD*/
const int RS_PIN = 7, E_PIN = 8, D4_PIN = 9, D5_PIN = 10, D6_PIN = 11, D7_PIN = 12, LCD_COL = 16, LCD_ROW = 2;

/*DS1307*/
const int SDA_PIN = 20, SCL_PIN = 21;

/*Buttons*/
const int setButton = 22, changeButton = 24;



//Object initialization
LedControl matrix = LedControl(DIN_PIN, CLK_PIN, CS_PIN, LED_DISPLAY);
DS3231 clock;
RTCDateTime dt;
LiquidCrystal lcd(RS_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);



//Timing of notes for alarm
const int eighth = 500;
const int sixteenth = 250;
const int between = 150;



//Images
const byte wakeup[][8] = {
  {
    B11000011,
    B11000011,
    B11011011,
    B11011011,
    B11011011,
    B01011011,
    B00111100,
    B00111100
  }, {
    B00011000,
    B00100100,
    B01100110,
    B01100110,
    B01111110,
    B01100110,
    B01100110,
    B01100110
  }, {
    B01100011,
    B01100110,
    B01101100,
    B01111000,
    B01111000,
    B01101100,
    B01100110,
    B01100011
  }, {
    B01111110,
    B01111110,
    B01000000,
    B01111110,
    B01111110,
    B01000000,
    B01111110,
    B01111110
  }, {
    B11000011,
    B11000011,
    B11000011,
    B11000011,
    B11000011,
    B11000011,
    B01100110,
    B00111100
  }, {
    B01111100,
    B01100010,
    B01100010,
    B01100010,
    B01111100,
    B01100000,
    B01100000,
    B01100000
  }
};
const int wakeup_len = sizeof(wakeup) / 8;

const byte skull[][8] = {
  {
    B00000000,
    B00111110,
    B01111111,
    B01110010,
    B01110010,
    B00111101,
    B00011111,
    B00001010
  }
  , {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
  }
};
const int skull_len = sizeof(skull) / 8;



//Global Variables
int setting = 0;
int hourAlarm = 0;
int minuteAlarm = 0;



//Setup for Main Code and other methods
void setup() {

  lcd.begin(LCD_COL, LCD_ROW);
  //Sets the number of columns and rows in the LCD

  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);
  //Initializes the clock

  matrix.shutdown(0, false);
  matrix.setIntensity(0, INTENSITY);
  //Settings for the LED matrix

  pinMode(setButton, INPUT);
  pinMode(changeButton, INPUT);
  //Buttons for input
  
}



//Shows "Wake Up" on the LED Matrix
void wakeUp() {
  for (int images = 0; images < wakeup_len; images++) {
    for (int col = 7, bits = 0; col >= 0 && bits < 8; col--, bits++) {
      matrix.setColumn(0, col, wakeup[images][bits]);
    }
    delay(500);
  }
}



//Flashes a skull on the LED Matrix
void showSans(){
  for (int i = 0; i < 3; i++){
    for (int images = 0; images < skull_len; images++) {
      for (int col = 7, bits = 0; col >= 0 && bits < 8; col--, bits++) {
      matrix.setColumn(0, col, skull[images][bits]);
    }
    delay(50);
    }
  }
}



//Displays Date on LCD Screen
void displayDateOnLCD() {
  lcd.setCursor(0, 0);
  lcd.print(dt.month);  lcd.print("-");
  lcd.print(dt.day);    lcd.print("-");
  lcd.print(dt.year);
}


//Displays Time on LCD Screen
void displayTimeOnLCD() {
  lcd.setCursor(0, 1);
  if (dt.hour > 12) {
    lcd.print(dt.hour - 12);  lcd.print(":");
    if (dt.minute < 10){
      lcd.print("0");
      lcd.print(dt.minute);
    } else {
      lcd.print(dt.minute);
    }
    lcd.print(" p.m.");
  } else {
    lcd.print(dt.hour);  lcd.print(":");
    if (dt.minute < 10){
      lcd.print("0");
      lcd.print(dt.minute);
    } else {
      lcd.print(dt.minute);
    }
    lcd.print(" a.m.");
  }
}



//Methods
void alarm() {

  int bassNotes[] = {NOTE_D4, NOTE_C4, NOTE_B3, NOTE_AS3};

  for (int measure = 0; measure < 4; measure++) {
    for (int i = 0; i < 2; i++) {
      tone(BUZZER, bassNotes[measure], sixteenth);
      delay(between);
    }
    tone(BUZZER, NOTE_D5, eighth + sixteenth);
    delay(between);
    tone(BUZZER, NOTE_A4, eighth + sixteenth);
    delay(between * 2);
    tone(BUZZER, NOTE_GS4, sixteenth);
    delay(between * 2);
    tone(BUZZER, NOTE_G4, sixteenth);
    delay(between * 2);
    tone(BUZZER, NOTE_F4, sixteenth);
    delay(between);
    tone(BUZZER, NOTE_F4, sixteenth);
    delay(between);
    tone(BUZZER, NOTE_D4, sixteenth);
    delay(between);
    tone(BUZZER, NOTE_F4, sixteenth);
    delay(between);
    tone(BUZZER, NOTE_G4, sixteenth);
    delay(between);
  }
}



//Main Code
void loop() {

  //Gets the date and time data
  dt = clock.getDateTime();

  //Displays the date and time on LCD Screen
  displayDateOnLCD();
  displayTimeOnLCD();

  //Checks if the time and alarm is the same and sets off the alarm
  if (dt.hour == hourAlarm && dt.minute == minuteAlarm) {
    wakeUp();
    alarm();
    showSans();
  }

  //When setButton is pressed, goes into setting mode
  if (digitalRead(setButton) == HIGH) {
    setting++;
    delay(500);

    //Clears LCD and sets up hour setting mode
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Hour Alarm:");

    //Uses a while loop to stay on hour setting mode
    while (setting == 1) {
      lcd.setCursor(0, 1);
      lcd.print("Hour = ");

      //When changeButton is pressed, increments hourAlarm until setButton is pressed again
      if (digitalRead(changeButton) == HIGH) {
        if (hourAlarm == 23) {
          hourAlarm = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Set Hour Alarm:");
          lcd.setCursor(0, 1);
          lcd.print("Hour = ");
        } else {
          hourAlarm++;
          delay(100);
        }
        delay(150);
      }
      lcd.print(hourAlarm);
      if (digitalRead(setButton) == HIGH) {
        setting++;
        delay(500);
      }
    }

    //Uses a while loop to stay on minute setting mode
    while (setting == 2) {

      //Clears LCD and sets up hour setting mode
      lcd.setCursor(0, 0);
      lcd.print("Set Minute Alarm");
      lcd.setCursor(0, 1);
      lcd.print("Minute = ");

      //When changeButton is pressed, increments hourAlarm until setButton is pressed again
      if (digitalRead(changeButton) == HIGH) {
        if (minuteAlarm == 59) {
          minuteAlarm = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Set Minute Alarm");
          lcd.setCursor(0, 1);
          lcd.print("Minute = ");
        } else {
          minuteAlarm++;
          delay(20);
        }
        delay(150);
      }
      lcd.print(minuteAlarm);

      //Breaks out of setting mode when setButton is pressed
      if (digitalRead(setButton) == HIGH) {
        setting = 0;
        delay(500);
        lcd.clear();
      }
    }
  }
}
