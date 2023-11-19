// #define FASTLED_ESP8266_RAW_PIN_ORDER
// #include <FastLED.h>
// #define NUM_LEDS 81

#include <WS2812FX.h>

WS2812FX leds = WS2812FX(81, D1, NEO_BRG + NEO_KHZ400);

#define BLYNK_PRINT Serial
#include "secrets.hpp"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

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

// Directions of segments - so you can always draw them from left to right
// and top to bottom: 1 means reversed, 0 means normal
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

void set_segment(int segment, uint32_t col) {
  int start = segment * 9;
//  leds.setSegment(segment, start, start+8, FX_MODE_STATIC, col, 0, false);
}

void digit(int d, uint32_t col) {
//  leds.setColor(0);
  for (int i = 0; i < 10; i++) {
    int segment = digit_segments[d][i];
    if (segment == -1) {
      break;
    }
    set_segment(segment, col);
  }
}

BlynkTimer uptime_timer;

void blynk_send_uptime()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

BLYNK_CONNECTED() {
  Serial.println("Blynk connected");
  Blynk.syncAll();
}

bool leds_on = true;
int current_digit = 0;
uint32_t digit_color = 0x00FFFFFF;

BLYNK_WRITE(V0) {
  current_digit = param.asInt();
  Serial.println("V0=" + String(current_digit));
}

BLYNK_WRITE(V1) {
  leds_on = (bool)param.asInt();
  Serial.println("V1=" + String(leds_on));
}

BLYNK_WRITE(V3) {
  int red = param[0].asInt();
  int green = param[1].asInt();
  int blue = param[2].asInt();
  Serial.println("red=" + String(red) + ", " +
                 "green=" + String(green) + ", " + 
                 "blue=" + String(blue));
  digit_color = (uint32_t)red << 16 | (uint32_t)green << 8 | (uint32_t)blue;
}

BLYNK_WRITE(V5) {
  int mode = param.asInt();
  Serial.println("V5=" + String(mode));
  leds.setMode(mode);
}

BLYNK_WRITE(V6) {
  int speed = param.asInt();
  Serial.println("V5=" + String(speed));
  leds.setSpeed(speed);
}

int counter = 9;

void setup()
{
  // Debug console
  Serial.begin(115200);
  Serial.println("Starting");

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  uptime_timer.setInterval(1000L, blynk_send_uptime);

  // FastLED.addLeds<WS2811, D2, BRG>(leds, NUM_LEDS)
  //   .setCorrection(TypicalLEDStrip);

  leds.init();
  leds.setBrightness(200);
  leds.setMode(FX_MODE_STROBE_RAINBOW);
  leds.setSpeed(10);
  leds.start();

  Serial.println("Start done");
}

void loop()
{
  Blynk.run();
  uptime_timer.run();

  leds.service();
  // if (leds_on) {
  //   digit(current_digit, digit_color);
  // }
  // else {
  //   FastLED.clear();
  // }
  // FastLED.show();
}