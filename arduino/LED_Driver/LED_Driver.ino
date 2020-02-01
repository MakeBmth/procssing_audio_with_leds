#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 16

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

byte r, g, b;
byte l_count = 0;
int p_count = 0;
bool refreshLeds = false;
unsigned long t1, t2, t3, t4;

void setup() {
  Wire.begin(0x44);                // join i2c bus with address
  Wire.onReceive(receiveEvent); // register event
  strip.begin();
  strip.show();
  Serial.begin(115200);           // start serial for output
  Serial.println("I2C receiving");
  strip.setPixelColor(0, 255, 0, 0);
  strip.fill(strip.Color(56,56,56),1);
  strip.show();
}

void loop() {
  if (refreshLeds) {
    unsigned long tme = t2 -t1;
    Serial.print("Receiving microseconds = ");
    Serial.println(tme);
    t3 = micros();
    strip.show();
    t4 = micros();
    tme = t4-t3;
    refreshLeds = false;
    Serial.print("show LEDs microseconds = ");
    Serial.println(tme);
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
      t1 = micros();
    } else if (2 == rb) {
  //    Serial.println(" : showing");
      t2 = micros();
      refreshLeds=true;
    } else {
      switch (l_count) {
        case 0: r = rb;
  //        Serial.println();
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
 //   Serial.print(rb);         // print the character
 //   Serial.print(' ');
  }
}
