/********************************************
High chair blinky
by Tim Bartlett
simple no-debounce version
Uses FastLED library and NeoPixels

pin 9 = NeoPixel data (single pixel)
pin 10 = button, red
pin 11 = button, green
pin 12 = button, blue
pin 13 = button, yellow
********************************************/

#include <FastLED.h>
#define NUM_LEDS 4
#define DATA_PIN 9

CRGB leds[NUM_LEDS];
uint8_t button[] = {10, 11, 12, 13};
uint8_t red[] = {255, 0, 0, 255};
uint8_t green[] = {0, 255, 0, 255};
uint8_t blue[] = {0, 0, 255, 0};

void setup() {
  for (int p=0; p<4; p++){
    pinMode(button[p], INPUT);
  }  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  for (int b = 0; b < 4; b++){
    if (digitalRead(button[b]) == HIGH){
      leds[b].setRGB(red[b], green[b], blue[b]);
    }
    else if (digitalRead(button[b]) == LOW){
        leds[b] = CRGB::Black;
    }
    FastLED.show();
  }
  delay(10);
}
