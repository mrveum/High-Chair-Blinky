/********************************************
High chair blinky
by Tim Bartlett
simple no-debounce version
single light
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

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  for (int b = 0; b < 4; b++){
    if (digitalRead(button[b]) == HIGH){
      switch (b){
        case 0:
          leds[b] = CRGB::Red;
          break;
        case 1:
          leds[b] = CRGB::Green;
          break;
        case 2:
          leds[b] = CRGB::Blue;
          break;
        case 3:
          leds[b] = CRGB::Yellow;
          break;
      }
    }  
    else if (digitalRead(button[b]) == LOW){
        leds[b] = CRGB::Black;
    }
    FastLED.show();
  }
}
