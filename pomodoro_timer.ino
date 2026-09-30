#include <Wire.h>
#include "WaveshareLCD.h"

#define PIN_BUTTON  2 // pin 2 for button, can be modified
#define PIN_BUZZER  8 // pin 8 for buzzer, can be modified

enum TimerState {
  START,
  MIN25,
  BUZZ,
  MIN5
};



TimerState currState = START;
unsigned long startTime = 0;
unsigned long duration = 0; // how long to countdown (ms)


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  lcd_init();
  
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currMillis = millis(); //Grab current time at start

  switch (currState){
    case START:
      lcd_set_cursor(0, 0); // Top Line
      lcd_print("Pomodoro Ready !");

      lcd_set_cursor(0, 1); // Bottom Line
      lcd_print("Press Button    ");

      if (digitalRead(PIN_BUTTON) == LOW)
      {
        startTime = millis(); // store exact starting time
        lcd_send_cmd(0x01); // clear screen (the 0x01 clears any leftover text)
        delay(200);
        currState = MIN25; // if button was pressed, go to the 25-minute countdown
      }

      break;
    case MIN25: {
      unsigned long elapsed, remainingSec, mins, secs;
      elapsed = millis() - startTime;
      if (elapsed >= 1500000UL) // 25 minutes are finished
      {
        lcd_send_cmd(0x01);
        delay(200);
        currState = BUZZ;
      }
      else
      {
        // calculation of remaining time
        remainingSec = (1500000UL - elapsed) / 1000; // the 1,500,000 ms is the total duration of 25 minutes
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
    }
    case BUZZ:
      lcd_set_cursor(0, 0);
      lcd_print("Time's Up !");
      lcd_set_cursor(0, 1);
      lcd_print("Break Time !");
      
      tone(PIN_BUZZER, 1000); // 1000Hz beep
      delay(1000);            // Beep for 1 second
      noTone(PIN_BUZZER);     // Stop beep

      startTime = millis();
      currState = MIN5;
      break;
    case MIN5: {
      unsigned long elapsed, remainingSec, mins, secs;
      elapsed = millis() - startTime;
      if (elapsed >= 300000UL)
      {
        lcd_send_cmd(0x01);
        delay(200);
        currState = START;
      }
      else
      {
        remainingSec = (300000UL - elapsed) / 1000;
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
