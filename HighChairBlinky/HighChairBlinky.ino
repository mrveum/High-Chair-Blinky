/********************************************
High chair blinky
by Tim Bartlett
Uses FastLED library and NeoPixels

pin 9 = NeoPixel data (single pixel)
pin 10 = button, red
pin 11 = button, green
pin 12 = button, blue
pin 13 = button, yellow

modes:
1) 1 button = 1 light momentary
2) 1 button = 1 light on until next light
3) typewriter
4) sequencer
********************************************/

#include <FastLED.h>
#define NUM_LEDS 4
#define DATA_PIN 9

CRGB leds[NUM_LEDS];
uint8_t button[] = {10, 11, 12, 13};
uint8_t bHue[] = {0, 90, 160, 64};
CRGB altLeds[NUM_LEDS];

bool lastButton[] = {LOW, LOW, LOW, LOW};
bool currentButton[] = {LOW, LOW, LOW, LOW};
uint32_t currentMillis = 0;
uint32_t prevMillis = 0;
uint32_t ledOn = 10000; // time that local led stays on after last activity
uint8_t mode = 1;
uint8_t typ = 0;
uint8_t counter = 0;
uint16_t stepSpeed = 800; // speed for sequencer mode 4
bool pause = false;

void setup() {
  for (int p=0; p<4; p++){
    pinMode(button[p], INPUT);
  }  
  currentMillis = millis();
  prevMillis = currentMillis;
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // limit my draw to 130mA at 5v of power draw
  FastLED.setMaxPowerInVoltsAndMilliamps(5,130); 
}

void loop() {
  while (mode == 1){
    modeOne();
    switchMode();
  }
  while (mode == 2){
    modeTwo();
    switchMode();
  }
  while (mode == 3){
    modeThree();
    switchMode();
  }
  while (mode == 4){
    modeFour();
//    switchMode();
  }
}

void modeOne(){ //one button = one light
  for (int b = 0; b < 4; b++){
    if (digitalRead(button[b]) == HIGH){
      leds[b].setHue(bHue[b]);
    }
    else if (digitalRead(button[b]) == LOW){
      leds[b] = CRGB::Black;
    }
    FastLED.show();
  }
}

void modeTwo(){ //one on until next on
  currentMillis = millis();
  for (int b = 0; b < 4; b++){
    currentButton[b] = debounce(lastButton[b], button[b]);
    if (lastButton[b] == LOW && currentButton[b] == HIGH) {
      leds[b].setHue(bHue[b]);
      FastLED.show();
      leds[b] = CRGB::Black;
      prevMillis = currentMillis;
    }
    lastButton[b] = currentButton[b];
  }  
  if (currentMillis - prevMillis == ledOn) {
    for (int c = 0; c < 4; c++){
      leds[c] = CRGB::Black;
    }
  FastLED.show();
  } 
}

void modeThree(){ //typewriter mode
  currentMillis = millis();
  for (int b = 0; b < 4; b++){
    currentButton[b] = debounce(lastButton[b], button[b]);
    if (lastButton[b] == LOW && currentButton[b] == HIGH) {
      prevMillis = currentMillis;
      leds[typ].setHue(bHue[b]);
      FastLED.show();
      typ++;
      if (typ == 4){
        for (int a = 0; a < 4; a++){
          leds[a] = CRGB::Black;
        }
        typ = 0;
      }
    }
    lastButton[b] = currentButton[b];
  }
  if (currentMillis - prevMillis == ledOn) {
    wipeClear();
  }
}

void modeFourSetup() {
  pause = false;
  for (int b = 0; b < 4; b++){
    leds[b].setHue(bHue[b]);
    leds[b] %= 50;
  }
  delay(250);
  currentMillis = millis();
  prevMillis = currentMillis;
  typ = 0;
  counter = 0;
}

void modeFour(){ //sequencer
  if (typ == 4){
    typ = 0;
  }  
  currentMillis = millis();
  leds[typ].maximizeBrightness();
  FastLED.show();
  switchMode(); 
  for (int b = 0; b < 4; b++){
    currentButton[b] = debounce(lastButton[b], button[b]);
    if (lastButton[b] == LOW && currentButton[b] == HIGH) {
      leds[typ].setHue(bHue[b]);
      FastLED.show();
      counter = 0;
    }
  }
  if (currentMillis - prevMillis >= stepSpeed){
    prevMillis = currentMillis;
    leds[typ] %= 50;
    FastLED.show();
    typ++;
    counter++;
  }
  
  if (counter >= 61) {
    pause = true;
    for (int p = 0; p < 4; p++){
      altLeds[p] = leds[p];
    }
    clearAll();
  }
  while (pause == true){
    for (int b = 0; b < 4; b++){
      currentButton[b] = debounce(lastButton[b], button[b]);
      if (lastButton[b] == LOW && currentButton[b] == HIGH) {
        pause = false;
        counter = 0;
        for (int p = 0; p < 4; p++){
          leds[p] = altLeds[p];
        }
        FastLED.show();
        delay(150);
      }
    }
  }
}

void switchMode(){
  if (digitalRead(button[0]) == HIGH && digitalRead(button[1]) == HIGH && digitalRead(button[2]) == HIGH && digitalRead(button[3]) == HIGH){
    bool modeWait = true;
    for (int all=0; all<4; all++){
      leds[all].setRGB(20, 20, 20);
      FastLED.show();
    }
    delay(500);
    while (modeWait == true){
      for (int b = 0; b < 4; b++){
        if (digitalRead(button[b]) == HIGH){
          leds[b].setRGB(255,255,255);
          FastLED.show();
          mode = (b+1);
          modeWait = false;
        }
      }
    }
    clearAll();
    if (mode == 4){
      modeFourSetup();
    }
    typ = 0;
    currentMillis = millis();
    prevMillis = currentMillis;
  }
}

boolean debounce(boolean last, byte btn){
  boolean current = digitalRead(btn);
  if (last != current) {
    delay(5);
    current = digitalRead(btn);
  }
  return current;
}

void shiftLeft(int pHue){
  leds[0] = leds[1];
  leds[1] = leds[2];
  leds[2] = leds[3];
  leds[3].setHue(pHue);
  FastLED.show();
  for (int sl = 0; sl < 3; sl++){
    delay(120);
    leds[0] = leds[1];
    leds[1] = leds[2];
    leds[2] = leds[3];
    leds[3] = CRGB::Black;
    FastLED.show();
  }
}

void shiftClear(){
  for (int sl = 0; sl < 4; sl++){
    delay(120);
    leds[0] = leds[1];
    leds[1] = leds[2];
    leds[2] = leds[3];
    leds[3] = CRGB::Black;
    FastLED.show();
  }
}

void wipeClear(){
  for (int a = 0; a < 4; a++){
    leds[a] = CRGB::Black;
    FastLED.show();
    delay(200);
  }
}

void clearAll(){
  for (int a = 0; a < 4; a++){
    leds[a] = CRGB::Black;
    FastLED.show();
  }
}
