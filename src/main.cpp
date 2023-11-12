#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#define NUM_LEDS 81

CRGB leds[NUM_LEDS];

/*
Segments placement and directions:

  +--3-->+
  ^      |
  |      4
  2      |
  |      V
  +--8-->+
  ^      |
  |      5
  1      |
  |      V
  +<--7--+
  ^      |
  |      6
  0      |
  |      |
*/

// Directions of segments - so you can always draw them from left to right and top to bottom
// 1 means reversed, 0 means normal
int segment_dirs[9] = {
    1, // 0
    1, // 1
    1, // 2
    0, // 3
    0, // 4
    0, // 5
    1, // 6
    0, // 7
    1  // 8
};

int digit_segments[][10] = {
    {1, 2, 3, 4, 5, 7, -1},    // 0
    {4, 5, -1},                // 1
    {3, 4, 8, 1, 7, -1},       // 2
    {3, 4, 8, 5, 7, -1},       // 3
    {2, 4, 8, 5, -1},          // 4
    {3, 2, 8, 5, 7, -1},       // 5
    {3, 2, 1, 8, 5, 7, -1},    // 6
    {3, 4, 5, -1},             // 7
    {1, 2, 3, 4, 5, 7, 8, -1}, // 8
    {2, 3, 4, 5, 7, 8, -1}     // 9
};

void set_segment(int segment, CRGB col) {
  int start = segment * 9;
  int end = start + 9;
  for (int i = start; i < end; i++) {
    leds[i] = col;
  }
}

void digit(int d, CRGB col) {
  FastLED.clear();
  for (int i = 0; i < 10; i++) {
    int segment = digit_segments[d][i];
    if (segment == -1) {
      break;
    }
    set_segment(segment, col);
  }
}

int counter = 9;

void setup()
{
  // Debug console
  Serial.begin(115200);
  Serial.println("Starting");

  FastLED.addLeds<WS2811, D1, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Serial.println("Start done");
}

CRGB cols[3] = {CRGB::Red, CRGB::Green, CRGB::Blue};

void loop()
{
  u8 sec = counter % 10;
  CRGB col = sec < 3 ? CRGB::Red : (sec < 6 ? CRGB::Yellow : CRGB::Green);
  Serial.println("sec=" + String(sec) + ", col=" + String(col.r) + "," + String(col.g) + "," + String(col.b));
  digit(sec, col);
  FastLED.show();

  if (counter == 0) {
    counter = 9;
  }

  //Serial.println("Colors sent");
  //Serial.println("counter=" + String(counter));
  delay(1000);
  counter--;
}
