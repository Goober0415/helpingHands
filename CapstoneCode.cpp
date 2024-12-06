/*
 * Project helpingHands
 * Author: Jamie Gavina
 * Date: 12/02/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */
// Include Particle Device OS APIs
#include "Particle.h"
#include <SPI.h>
#include <Adafruit_GFX_RK.h>
#include <Adafruit_ILI9341.h>
#include <neopixel.h>
#include <Colors.h>
#include <IoTTimer.h>

#define TFT_DC D5
#define TFT_CS D4
#define STMPE_CS D3
#define SD_CS D2

// Global variables
char buffer[32];
int i;
static unsigned long currentTime = millis();
static unsigned long lastUpdateTime = 0;

// constants
const int PIXELCOUNT = 7;
const int DELAY = 5000;

// objects
Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

// functions
unsigned long testText();
unsigned long testFillScreen();
unsigned long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long testRects(uint16_t color);
unsigned long testFastLines(uint16_t color1, uint16_t color2);
unsigned long testLines(uint16_t color);
unsigned long testRoundRects();
unsigned long testFilledTriangles();
unsigned long testTriangles();
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testFilledRoundRects();
void updateTimeAndDate();
void updateDisplay(const char *text);
void printRemindToPee();
void printRemindFeetUp();

// Timers
// Timer pottyTimer(3600000, printRemindToPee); //per hour
Timer pottyTimer(60000, printRemindToPee); // per minute
// Timer feetUpTimer(2700000, printRemindFeetUp); //every 45 minutes
Timer feetUpTimer(45000, printRemindFeetUp); // per 45 seconds

SYSTEM_MODE(AUTOMATIC);

void setup()
{
  Serial.begin(9600);
  delay(5000);
  Serial.println("FeatherWing TFT Test!");

  tft.begin();
  WiFi.on();
  waitFor(WiFi.ready, 10000);
  Particle.syncTime();
  Time.zone(-7);
  pixel.begin();
  pixel.clear();
  pixel.setBrightness(15);
  pixel.setPixelColor(0, violet);
  pixel.setPixelColor(1, indigo);
  pixel.setPixelColor(2, blue);
  pixel.setPixelColor(3, green);
  pixel.setPixelColor(4, yellow);
  pixel.setPixelColor(5, orange);
  pixel.setPixelColor(6, red);

  pixel.show();
  // Serial.println(F("Done!"));

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x");

  Serial.print(F("Lines                    "));
  Serial.println(testLines(ILI9341_CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(ILI9341_RED, ILI9341_BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(ILI9341_GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, ILI9341_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, ILI9341_WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);

  tft.setRotation(1); // Adjust rotation as needed
  pixel.clear();
  pottyTimer.start();
  feetUpTimer.start();
}

void loop()
{
  pixel.clear();
  pixel.setBrightness(15);
  pixel.setPixelColor(0, blue);
  pixel.setPixelColor(1, violet);
  pixel.setPixelColor(2, blue);
  pixel.setPixelColor(3, violet);
  pixel.setPixelColor(4, blue);
  pixel.setPixelColor(5, violet);
  pixel.setPixelColor(6, blue);

  pixel.show();

  if (millis() - lastUpdateTime >= 1000)
  { // Update every second
    lastUpdateTime = millis();
    updateTimeAndDate();
    updateDisplay(buffer);
  }

  if (millis() - lastUpdateTime >= 1200000)
  {
    printRemindToPee();
    pottyTimer.reset();
  }
  if (millis() - lastUpdateTime >= 45000)
  {
    printRemindFeetUp();
    feetUpTimer.reset();
  }
}

void updateTimeAndDate()
{
  bool synced = Particle.syncTime();
  // Ensure Particle is synchronized with cloud time
  if (!synced)
  {
    Serial.println("Not synced with Particle Cloud. Attempting sync...");
    Particle.syncTime();
    delay(5000); // Wait for sync to complete
  }

  // Get current time
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d\n", Time.hour(), Time.minute(), Time.second());

  // Get current date
  snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), " %02d-%02d-%04d", Time.month(), Time.day(), Time.year());

  Serial.print("Current Time: ");
  Serial.println(buffer);
}

void updateDisplay(const char *text)
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(3);
  tft.println(text);
}

unsigned long testFillScreen()
{
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.fillScreen(ILI9341_RED);
  yield();
  tft.fillScreen(ILI9341_GREEN);
  yield();
  tft.fillScreen(ILI9341_BLUE);
  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText()
{
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color)
{
  unsigned long start, t;
  int x1, y1, x2, y2,
      w = tft.width(),
      h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1 = y1 = 0;
  y2 = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  t = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  t += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1 = 0;
  y1 = h - 1;
  y2 = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  t += micros() - start;

  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);
  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
    tft.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2)
{
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for (y = 0; y < h; y += 5)
    tft.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5)
    tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color)
{
  unsigned long start;
  int n, i, i2,
      cx = tft.width() / 2,
      cy = tft.height() / 2;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  start = micros();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2)
{
  unsigned long start, t = 0;
  int n, i, i2,
      cx = tft.width() / 2 - 1,
      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;
    start = micros();
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
    t += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color)
{
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color)
{
  unsigned long start;
  int x, y, r2 = radius * 2,
            w = tft.width() + radius,
            h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for (x = 0; x < w; x += r2)
  {
    for (y = 0; y < h; y += r2)
    {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles()
{
  unsigned long start;
  int n, i, cx = tft.width() / 2 - 1,
            cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(cx, cy);
  start = micros();
  for (i = 0; i < n; i += 5)
  {
    tft.drawTriangle(
        cx, cy - i,     // peak
        cx - i, cy + i, // bottom left
        cx + i, cy + i, // bottom right
        tft.color565(i, i, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles()
{
  unsigned long start, t = 0;
  int i, cx = tft.width() / 2 - 1,
         cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5)
  {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(0, i * 10, i * 10));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(i * 10, i * 10, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects()
{
  unsigned long start;
  int w, i, i2,
      cx = tft.width() / 2 - 1,
      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  w = min(tft.width(), tft.height());
  start = micros();
  for (i = 0; i < w; i += 6)
  {
    i2 = i / 2;
    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects()
{
  unsigned long start;
  int i, i2,
      cx = tft.width() / 2 - 1,
      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
  {
    i2 = i / 2;
    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}

void printRemindToPee()
{
  static unsigned long lastBlinkTime = 0;
  static const unsigned long BLINK_DELAY = 60000; // 1 minute between blinks

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(45, 20);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(3);
  tft.println("Time to try to use the restroom!");

  pixel.clear();
  pixel.setBrightness(15);
  pixel.setPixelColor(0, yellow);
  pixel.setPixelColor(1, yellow);
  pixel.setPixelColor(2, yellow);
  pixel.setPixelColor(3, yellow);
  pixel.setPixelColor(4, yellow);
  pixel.setPixelColor(5, yellow);
  pixel.setPixelColor(6, yellow);

  pixel.show();

  if (currentTime - lastBlinkTime >= BLINK_DELAY)
  {
    lastBlinkTime = currentTime;

    // Toggle the state of the pixels
    for (int i = 0; i < 7; i++)
    {
      pixel.setPixelColor(i, pixel.getPixelColor(i) == yellow ? LOW : yellow);
    }
  }
}

void printRemindFeetUp()
{
  static unsigned long lastBlinkTime = 0;
  static const unsigned long BLINK_DELAY = 4500; // 2 MINUTES between blinks

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(24, 20);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.println("Are your feet up?!");

  pixel.clear();
  pixel.setBrightness(15);
  pixel.setPixelColor(0, orange);
  pixel.setPixelColor(1, orange);
  pixel.setPixelColor(2, orange);
  pixel.setPixelColor(3, orange);
  pixel.setPixelColor(4, orange);
  pixel.setPixelColor(5, orange);
  pixel.setPixelColor(6, orange);

  pixel.show();

  if (currentTime - lastBlinkTime >= BLINK_DELAY)
  {
    lastBlinkTime = currentTime;

    // Toggle the state of the pixels
    for (int i = 0; i < 7; i++)
    {
      pixel.setPixelColor(i, pixel.getPixelColor(i) == yellow ? LOW : yellow);
    }
  }
}