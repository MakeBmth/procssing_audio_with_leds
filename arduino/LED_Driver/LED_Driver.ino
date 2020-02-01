#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 256

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

byte r, g, b;
byte l_count = 0;
int p_count = 0;
bool refreshLeds = false;

void setup() {
  Wire.begin(0x44);             // join i2c bus with address 0x44
  Wire.onReceive(receiveEvent); // i2c receiving callback
  strip.begin();
  strip.show();
  strip.setPixelColor(0, 255, 0, 0);
  strip.fill(strip.Color(56,56,56),1);
  strip.show();
}

void loop() {
  if (refreshLeds) {
    refreshLeds = false;
    strip.show();
  }
  delay(1);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    byte rb = Wire.read(); // receive byte as a character
    if (1 == rb) {
      p_count = 0;
      l_count = 0;
    } else if (2 == rb) {
      refreshLeds=true;
    } else {
      switch (l_count) {
        case 0: r = rb;
          l_count++;
          break;
        case 1: g = rb;
          l_count++;
          break;
        case 2: b = rb;
          strip.setPixelColor(p_count, r, g, b);
          p_count++;
          if (p_count >= LED_COUNT) { p_count = LED_COUNT-1;}
          l_count = 0;
          break;
      }
    }
  }
}
