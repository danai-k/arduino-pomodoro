#include <Wire.h>
#include "WaveshareLCD.h"

#define BUTTON  2 // pin 2 for button, can be modified
#define BUZZER  8 // pin 8 for buzzer, can be modified

enum TimerState {
  START,
  FOCUSTIME, // 25 min
  BUZZ,
  BREAKTIME // 5 min
};

TimerState currState = START;
unsigned long startTime = 0;
unsigned long duration = 0; // how long to countdown
const unsigned long focusDuration = 1500000UL; // can be changed
const unsigned long breakDuration = 300000UL; // can be changed

void setup() {
  Wire.begin();
  lcd_init();

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

}

void loop() {
  switch (currState){
    case START:
      lcd_set_cursor(0, 0); // top line
      lcd_print("Pomodoro Ready !");

      lcd_set_cursor(0, 1); // bottom line
      lcd_print("Press Button    ");

      if (digitalRead(BUTTON) == LOW) // button is pressed
      {
        startTime = millis(); // exact starting time
        lcd_send_cmd(0x01); // clear screen (0x01 clears any leftover text)
        currState = FOCUSTIME; // goto x-minute countdown for focus
      }
      break;

    case FOCUSTIME: 
      unsigned long elapsed, remainingSec, mins, secs;
      elapsed = millis() - startTime;
      if (elapsed >= focusDuration) //  minutes are finished 
      {
        lcd_send_cmd(0x01);
        delay(200);
        currState = BUZZ;
      }
      else
      {
        // calculation of remaining time
        remainingSec = (focusDuration - elapsed) / 1000;
        mins = remainingSec / 60;
        secs = remainingSec % 60;

        lcd_set_cursor(0, 0);
        lcd_print("Focus Time !   ");

        lcd_set_cursor(0, 1);
        lcd_print(mins);
        lcd_print("m ");
        lcd_print(secs);
        lcd_print("s      ");
      }
      break;

    case BUZZ:
      lcd_set_cursor(0, 0);
      lcd_print("Time's Up !");
      lcd_set_cursor(0, 1);
      lcd_print("Break Time !");
      
      tone(BUZZER, 1000);
      delay(1000);
      noTone(BUZZER);

      startTime = millis();
      currState = BREAKTIME;
      break;

    case BREAKTIME: {
      unsigned long elapsed, remainingSec, mins, secs;
      elapsed = millis() - startTime;
      if (elapsed >= breakDuration)
      {
        lcd_send_cmd(0x01);
        delay(200);
        currState = START;
      }
      else
      {
        remainingSec = (breakDuration - elapsed) / 1000;
        mins = remainingSec / 60;
        secs = remainingSec % 60;

        lcd_set_cursor(0, 0);
        lcd_print("Break Time !");

        lcd_set_cursor(0, 1);
        lcd_print(mins);
        lcd_print("m ");
        lcd_print(secs);
        lcd_print("s         ");
      }
      break;
    }
  }
}
