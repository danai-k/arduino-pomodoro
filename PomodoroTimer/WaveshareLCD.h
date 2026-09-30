#ifndef WAVESHARE_LCD_H
#define WAVESHARE_LCD_H

#include <Arduino.h>
#include <Wire.h>

#define LCD_ADDR 0x3E  // Change to 0x27 if your screen uses 0x27

void lcd_send_cmd(byte cmd) {
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0x80);
  Wire.write(cmd);
  Wire.endTransmission();
}

void lcd_send_data(byte data) {
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0x40);
  Wire.write(data);
  Wire.endTransmission();
}

void lcd_init() {
  delay(50);
  lcd_send_cmd(0x38);
  delayMicroseconds(50);
  lcd_send_cmd(0x0C);
  delayMicroseconds(50);
  lcd_send_cmd(0x01);
  delay(2);
  lcd_send_cmd(0x06);
}

void lcd_set_cursor(byte col, byte row) {
  byte addr = (row == 0) ? col : (0x40 + col);
  lcd_send_cmd(0x80 | addr);
}

void lcd_print(const char* str) {
  while (*str) {
    lcd_send_data(*str++);
  }
}

void lcd_print(unsigned int number) {
  char buf[10];
  itoa(number, buf, 10);
  lcd_print(buf);
}

#endif
