#define internalLED 13

#include <FastLED.h>//LEDs https://github.com/FastLED/FastLED
//leds stuff
#define NUM_STRIPS 1
#define NUM_LEDS 7
#define BRIGHTNESS 10
#define LED_TYPE WS2812B
#define COLOR_ORDER BRG//RGB
#define FASTLED_ALLOW_INTERRUPTS 0
#define FRAMES_PER_SECOND 60
#define COOLING 55
#define SPARKING 120
CRGB leds[NUM_LEDS];
const int stripPin   = 2;

bool isInternalLedBlinking = false;

void displayRedLed(int mode=0){      
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(255, 0, 0);
    if (mode && i%2) leds[i].setRGB(0, 0, 0);
    FastLED.show();     
    }
}

void displayBlueLed(int mode=0){      
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 255, 0);
    if (mode && i%2) leds[i].setRGB(0, 0, 0);
    FastLED.show();     
    }
}

void displayRedLedMesure(int mesure=0){      
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(255, 0, 0);
    if (i>mesure) leds[i].setRGB(0, 0, 0);
    FastLED.show();     
    }
}



void setupLed(){
  pinMode(internalLED, OUTPUT);

  //leds stuff 
  FastLED.addLeds<LED_TYPE, stripPin, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );

}

void loopLed(){
  if(isInternalLedBlinking) {
    digitalWrite(internalLED, HIGH);   // allumage de la LED embarquée
    delay(500);                                     // pause d'une demi-seconde
    digitalWrite(internalLED, LOW);    // extinction de la LED
    delay(500);                                     // et repause d'une demi-seconde
  }
}
