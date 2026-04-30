#include "lcd.h"
#include <Arduino.h>

// see pages 24-25 of datasheet
// https://www.alldatasheet.com/datasheet-pdf/view/63673/HITACHI/HD44780.html
enum REG { IR = 0, DR = 1 }; // instruction vs data register

// part of the initialization process
// 8bit mode command is just a nibble (4 bits)
// since only 4 wires are physically connected
const uint8_t CMD_NIBBLE_8BIT_MODE = 0x3;
const uint8_t CMD_NIBBLE_4BIT_MODE = 0x2;

// full byte commands, sent in 2 nibbles
const uint8_t CMD_4BIT_MODE = 0x28;
const uint8_t CMD_CLEAR = 0x01;
const uint8_t CMD_ENTRY_MODE = 0x06;
const uint8_t CMD_DISPLAY_ON = 0x0C;
const uint8_t CMD_DISPLAY_OFF = 0x08;
const uint8_t CMD_SET_CGRAM_ADDR = 1 << 6;
const uint8_t CMD_SET_DDRAM_ADDR = 1 << 7;

static void write_nibble(LCD *lcd, uint8_t n, REG reg);
static void write_command(LCD *lcd, uint8_t n);
static void write_data(LCD *lcd, uint8_t n);

/* High level funcs */
void LCD_init(LCD *lcd) {
  // arduino pin setup
  pinMode(lcd->RS_PIN, OUTPUT);
  pinMode(lcd->EN_PIN, OUTPUT);
  pinMode(lcd->D4_PIN, OUTPUT);
  pinMode(lcd->D5_PIN, OUTPUT);
  pinMode(lcd->D6_PIN, OUTPUT);
  pinMode(lcd->D7_PIN, OUTPUT);

  // see pg 46 of datasheet
  // describing 4-bit mode initialization
  delay(50); // wait 50 ms for VCC to rise >= 2.7V

  // set 8 bit mode
  write_nibble(lcd, CMD_NIBBLE_8BIT_MODE, IR);
  delay(5);

  // set 8 bit mode again
  write_nibble(lcd, CMD_NIBBLE_8BIT_MODE, IR);
  delayMicroseconds(150);

  // set 8 bit mode a 3rd time
  write_nibble(lcd, CMD_NIBBLE_8BIT_MODE, IR);
  delayMicroseconds(150);

  // set 4 bit mode
  write_nibble(lcd, CMD_NIBBLE_4BIT_MODE, IR);
  delay(1);
  write_command(lcd, CMD_4BIT_MODE);

  write_command(lcd, CMD_DISPLAY_OFF);
  LCD_clear(lcd);
  write_command(lcd, CMD_ENTRY_MODE);
  write_command(lcd, CMD_DISPLAY_ON);
}

void LCD_clear(LCD *lcd) {
  write_command(lcd, CMD_CLEAR);
  delay(2); // extra slow instruction, needs more time
}

void LCD_create_char(LCD *lcd, uint8_t char_idx, const Bitmap bm) {
  char_idx =
      char_idx <= 7 ? char_idx : 7; // safety check, only 8 custom char slots
  int addr = char_idx * 8;          // offset for each row in CGRAM
  write_command(lcd, CMD_SET_CGRAM_ADDR | addr);

  for (int i = 0; i < BITMAP_HEIGHT; i++)
    write_data(lcd, bm[i]);
}

void LCD_set_cursor(LCD *lcd, uint8_t row, uint8_t col) {
  // see pg. 11 of datasheet
  row = row == 0 ? 0x00 : 0x40;
  col = col < 15 ? col : 15;
  uint8_t addr = row + col;
  write_command(lcd, CMD_SET_DDRAM_ADDR | addr);
}

void LCD_print(LCD *lcd, const char *c) {
  while (*c) {
    write_data(lcd, *c++);
  }
}

void LCD_print(LCD *lcd, uint8_t n) { write_data(lcd, n); }

/* mid level helpers */
static void write_byte(LCD *lcd, uint8_t n, REG reg);

static void write_command(LCD *lcd, uint8_t n) { write_byte(lcd, n, IR); }
static void write_data(LCD *lcd, uint8_t n) { write_byte(lcd, n, DR); }

/* low level helpers */
static void pulse(LCD *lcd) {
  // see pg 52 of datasheet for timing tolerances
  digitalWrite(lcd->EN_PIN, LOW);
  delayMicroseconds(1);
  digitalWrite(lcd->EN_PIN, HIGH);
  delayMicroseconds(1); // at least 200 ns
  digitalWrite(lcd->EN_PIN, LOW);
  // most commands have max execution time of
  // 37 microseconds
  delayMicroseconds(40);
};

// send 4 lowest bits
static void write_nibble(LCD *lcd, uint8_t n, REG reg) {
  // which register to write to
  digitalWrite(lcd->RS_PIN, reg);

  // data to write
  digitalWrite(lcd->D4_PIN, n & 0x1);
  digitalWrite(lcd->D5_PIN, (n >> 1) & 0x1);
  digitalWrite(lcd->D6_PIN, (n >> 2) & 0x1);
  digitalWrite(lcd->D7_PIN, (n >> 3) & 0x1);

  pulse(lcd);
}

static void write_byte(LCD *lcd, uint8_t n, REG reg) {
  // split 8 bits into 2 nibbles
  write_nibble(lcd, n >> 4, reg);
  write_nibble(lcd, n, reg);
}
