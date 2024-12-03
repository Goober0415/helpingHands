/*
 * Project helpingHands
 * Author: Jamie Gavina
 * Date: 12/02/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include <SPI.h>
#include <Arduino.h>
#include <Colors.h>
#include <string.h>
#include <time.h>

SYSTEM_MODE(SEMI_AUTOMATIC);

// Define LCD dimensions
#define LCD_WIDTH 320
#define LCD_HEIGHT 240

// SPI pin definitions (adjust according to your hardware)
#define SCK_PIN D4
#define MOSI_PIN D15
#define CS_PIN A2
#define DC_PIN D5

// Declare and implement functions
void lcdInit();
void drawTestGraphics();
void fillScreen(uint16_t color);
void lcdUpdate();
void transfer(uint16_t data);
void drawCircle(int x, int y, int radius);
void drawRect(int x, int y, int w, int h);

void loop()
{
  static unsigned int currentTime = millis();
  static unsigned long lastUpdate = 0;
  unsigned char currentHour;
  unsigned char currentMinute;
  unsigned char currentSecond;
  int hour;
  int minute;
  int second;
  const char *COLON = ":";

  // Set initial time
  currentHour = ;
  currentMinute = ;
  currentSecond = ;

  // Update every 500ms
  if (millis() - lastUpdate >= 500)
  {
    lastUpdate = millis();

    // Clear the screen
    fillScreen(black);

    // Draw a rotating circle
    static int angle = 0;
    angle++;
    drawCircle(160, 120, 90);

    // Add some text
    printf("Hello, SPI LCD!");

    // Draw a simple graphic
    drawRect(10, 10, 300, 220);

    // Add dynamic text
    String currentTime = String(currentHour + COLON + currentMinute + COLON + currentSecond);
    printf(currentTime);
  }
}

// Use transfer16() instead of SPI.transfer16() in your drawing functions.
void lcdInit()
{
  // Initialize SPI
  SPI.begin();

  // Set up GPIO pins
  pinMode(CS_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);

  // Initialize LCD commands
  uint8_t init_commands[] = {
      0x11,       // Sleep out
      0xD5, 0xF1, // Display inversion
      0x15, 0x3F, // Frame rate control
      0x71,       // Column address set
      0x6C, 0x00, // Memory access control
      0x76,       // Pixel format set
      0x11,       // Sleep out
      0x2A, 0x01, // Partial mode on
      0x2B, 0x00, // Page mode off
      0x29,       // Display on
      0x24, 0x00, // Display start line set
      0x26, 0x80, // Memory data interface setting
      0x2E, 0x00, // Multi-tone display control
      0x2F,       // Inverse display on
      0x36, 0x80, // Memory data setting
      0x3A, 0x55, // Column address set
      0x3B, 0x10, // Page address set
      0x33,       // Display function control
      0x32,       // VOP control
      0x35, 0x3B, // Gray scale select
      0x34, 0x07, // FRM rate control
      0x9D,       // Power control 1
      0x95, 0x14, // Power control 2
      0x8D, 0x14, // VCOM control 1
      0x85, 0x00, // VCOM control 2
      0x78,       // Entry mode set
      0x7C,       // Set display offset
      0x54, 0x03, // Tearing effect control
      0x5C, 0x03, // Deactivate scroll
      0x5E,       // Master clearing
      0x61, 0x00, // Deactivate oscillator enable
      0x3C,       // Display on
      0x6B, 0x00, // Partial mode off
      0x6C, 0x01, // Memory access control
      0x64, 0x03, // Column address set
      0x65, 0x80, // Page address set
      0x6D,       // Display on
      0x78,       // Entry mode set
      0x7C,       // Set display offset
      0x54, 0x03, // Tearing effect control
      0x5C, 0x03, // Deactivate scroll
      0x5E,       // Master clearing
      0x61, 0x01, // Activate oscillator enable
      0x34, 0x07, // FRM rate control
      0x35, 0x3B, // Gray scale select
      0x36, 0x80, // Memory data setting
      0x3A, 0x55, // Column address set
      0x3B, 0x10, // Page address set
      0x2E, 0x00, // Multi-tone display control
      0x2F,       // Inverse display on
      0x26, 0x80, // Memory data interface setting
      0x2A, 0x01, // Partial mode on
      0x2B, 0x00, // Page mode off
      0x29,       // Display on
      0x7C,       // Set display offset
      0x54, 0x03, // Tearing effect control
      0x5C, 0x03, // Deactivate scroll
      0x5E,       // Master clearing
      0x61, 0x01, // Activate oscillator enable
      0x34, 0x07, // FRM rate control
      0x35, 0x3B, // Gray scale select
      0x36, 0x80, // Memory data setting
      0x3A, 0x55, // Column address set
      0x3B, 0x10, // Page address set
      0x2E, 0x00, // Multi-tone display control
      0x2F,       // Inverse display on
      0x26, 0x80, // Memory data interface setting
      0x2A, 0x01, // Partial mode on
      0x2B, 0x00, // Page mode off
      0x29        // Display on
  };

  // Send initialization commands
  for (int i = 0; i < sizeof(init_commands); i++)
  {
    SPI.transfer(0x1000 | init_commands[i]);
  }

  // Set DC pin high for data/command mode
  digitalWrite(DC_PIN, HIGH);
}

void drawTestGraphics()
{
  lcdUpdate();

  fillScreen(black);

  drawRect(10, 10, 300, 220);
  drawCircle(160, 120, 90);
}

void fillScreen(uint16_t color)
{
  for (int y = 0; y < LCD_HEIGHT; y++)
  {
    for (int x = 0; x < LCD_WIDTH; x++)
    {
      uint16_t pixelColor = color;
      SPI.transfer(0x2000 | (pixelColor << 8) | pixelColor);
    }
  }
}

void drawRect(int x, int y, int w, int h)
{
  for (int i = 0; i < w; i++)
  {
    SPI.transfer(0x2000 | ((y + h - 1) << 8) | (x + i));
    SPI.transfer(0x2000 | ((y + h - 1) << 8) | (x + i));
  }
  for (int i = 0; i < h; i++)
  {
    SPI.transfer(0x2000 | ((y + i) << 8) | (x + w - 1));
    SPI.transfer(0x2000 | ((y + i) << 8) | (x + w - 1));
  }
}

void drawCircle(int x, int y, int radius)
{
  for (int dy = -radius; dy <= radius; dy++)
  {
    for (int dx = -radius; dx <= radius; dx++)
    {
      if (dx * dx + dy * dy <= radius * radius)
      {
        SPI.transfer(0x2000 | ((y + dy) << 8) | (x + dx));
        SPI.transfer(0x2000 | ((y + dy) << 8) | (x + dx));
      }
    }
  }
}

void drawLine(int x1, int y1, int x2, int y2)
{
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = (dx > dy) ? dx : -dy;

  while (true)
  {
    SPI.transfer(0x2000 | ((y1 << 8) | x1));
    if (x1 == x2 && y1 == y2)
      break;
    int e2 = 2 * err;
    if (e2 > -dy)
    {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx)
    {
      err += dx;
      y1 += sy;
    }
  }
}

void transfer(uint16_t data)
{
  SPI.transfer(data >> 8);
  SPI.transfer(data & 0xFF);
}

void lcdUpdate()
{
  digitalWrite(CS_PIN, LOW);
  digitalWrite(DC_PIN, HIGH); // Set DC high for data/command mode
  delay(10);                  // Small delay to ensure stability

  // Send command to set page address
  SPI.transfer(0x2B00);

  // Send column address
  SPI.transfer(0x0000);

  // Send data
  for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT / 8; i++)
  {
    uint16_t pixelColor = 0;
    // Fill pixelColor with appropriate color value
    SPI.transfer(0x2000 | (pixelColor << 8) | pixelColor);
  }

  digitalWrite(CS_PIN, HIGH);
}