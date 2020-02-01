import processing.sound.*;
import processing.io.*;

FFT fft;
AudioIn in;
int bands = 256;
int square = ceil(sqrt(bands));
int boost = 15;
int lightMultiplier = 10;
float[] spectrum = new float[bands];
byte[] rgb = new byte[3];
I2C i2c;
int clr;
int amp;

void settings() {
  size(square * 8, square * 8);
}

void setup() {
  background(0);
  stroke(255);
  rectMode(CORNER);
  print(I2C.list()[0]);
  i2c = new I2C(I2C.list()[0]);
    
  // Create an Input stream which is routed into the Amplitude analyzer
  fft = new FFT(this, bands);
  in = new AudioIn(this, 0);
  
  // start the Audio Input
  in.start();
  
  // patch the AudioIn
  fft.input(in);
}      

void draw() { 
  background(0);
  fft.analyze(spectrum);
  try {
    i2c.beginTransmission(0x44);  
    i2c.write(1);
    i2c.endTransmission();
  } catch (Exception e) {
    print (e.toString());
  }
  for(int i = 0; i < bands; i++){
    clr = round(spectrum[i] * 256 % 4);
    amp = round(map(min(255, spectrum[i] * 256 * boost), 0, 255, 1, 40));
    switch (clr) {
      case 0:
        fill(amp*lightMultiplier, 0, 0);
        rgb[0] = (byte)(amp*lightMultiplier);
        rgb[1] = 0;
        rgb[2] = 0;
      break;
      case 1:
        fill(0, amp*lightMultiplier, 0);
        rgb[0] = 0;
        rgb[1] = (byte)(amp*lightMultiplier);
        rgb[2] = 0;
      break;
      case 2:
        fill(0, 0, amp*lightMultiplier);
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = (byte)(amp*lightMultiplier);
      break;
      case 3:
        fill(amp*lightMultiplier, amp*lightMultiplier, amp*lightMultiplier);
        rgb[0] = (byte)(amp*lightMultiplier);
        rgb[1] = (byte)(amp*lightMultiplier);
        rgb[2] = (byte)(amp*lightMultiplier);
      break;
      default :
    }
    float x = i % square;
    float y = abs(i / square);
    rect(x*8, y*8, 8, 8);
    try {
      i2c.beginTransmission(0x44);  
      i2c.write(rgb);
      i2c.endTransmission();
    } catch (Exception e) {
      print (e.toString());
    }
  }
    try {
      i2c.beginTransmission(0x44);  
      i2c.write(2);
      i2c.endTransmission();
    } catch (Exception e) {
      print (e.toString());
    }
}
