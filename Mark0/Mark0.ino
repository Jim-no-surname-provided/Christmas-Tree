/// @file    Mark0.ino
/// @brief   Demonstrates how to use @ref ColorPalettes
/// @example Mark0.ino

#include <FastLED.h>

#define LED_PIN 5
#define NUM_LEDS 100
#define ROWS 4
const int COLS = NUM_LEDS / ROWS;
#define BRIGHTNESS 100
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
CRGB ledsMatrix[ROWS][COLS];

#define UPDATES_PER_SECOND 100

extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  delay(3000);  // power-up safety delay
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}


void loop() {
  static int timeIndex = 0;
  // allPink();
  // austrianFlag();
  // mexicanFlag();

  movingLine(timeIndex);

  ledsToCurve();

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  ++timeIndex;
}

void allPink() {
  for (int row = 0; row < ROWS; ++row) {
    for (int col = 0; col < COLS; ++col) {
      ledsMatrix[row][col] = CRGB(CHSV(HUE_PINK, 255, 255));
    }
  }
}

void austrianFlag() {
  for (int col = 0; col < COLS; ++col) {
    ledsMatrix[0][col] = CRGB::Red;
  }
  for (int col = 0; col < COLS; ++col) {
    ledsMatrix[1][col] = CRGB::White;
  }
  for (int col = 0; col < COLS; ++col) {
    ledsMatrix[2][col] = CRGB::Red;
  }
}
void mexicanFlag() {
  for (int row = 0; row < ROWS; ++row) {
    for (int col = 0; col < COLS; ++col) {
      if (col < COLS / 3) {
        set(row, col, CRGB::Green);
      } else if (col < 2 * COLS / 3) {
        set(row, col, CRGB::White);
      } else {
        set(row, col, CRGB::Red);
      }
    }
  }
}

void ledsToCurve() {
  int index = NUM_LEDS - 1;
  for (int row = 0; row < ROWS; ++row) {
    for (int col = 0; col < COLS; ++col, --index) {
      if (row % 2 == 0) {
        leds[index] = ledsMatrix[row][col];
      } else {
        leds[index] = ledsMatrix[row][COLS - col - 1];
      }
    }
  }

  for (; index >= 0; --index) {
    leds[index] = ledsMatrix[ROWS - 1][COLS - 1];
  }
}


void set(int x, int y, CRGB color) {
  ledsMatrix[ROWS - y - 1][x] = color;
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

int const velocityLine = 10;  // leds per second
int const minXLine = -COLS / 2;
int const maxXLine = 3 * COLS / 2;
void movingLine(int timeIndex) {
  float time = ((float)timeIndex) / UPDATES_PER_SECOND;
  float L = (maxXLine - minXLine) / velocityLine;
  bool left2Right = true;
  float anim_x = ((float)velocityLine) * floatModulo(time, L) + minXLine;

  int angle = 1;
  int m = tan(angle);
  int d = 2;

  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      if (isPartOfLine(anim_x, x, y, m, d)) {
        set(x, y, CRGB::White);
      } else {
        set(x, y, CRGB::Green);
      }
    }
  }
}

float floatModulo(float a, float b) {
  while (a > b) {
    a -= b;
  }
  return a;
}

bool isPartOfLine(float anim_x, int x, int y, int m, int d) {
  float n = (y - m * x + m * anim_x);
  return d * d >= n * n / (m * m + 1);
}