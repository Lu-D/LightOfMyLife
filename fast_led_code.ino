#define FASTLED_INTERNAL
#include <FastLED.h>

#define NUM_LEDS 170
#define DATA_PIN1 0
#define DATA_PIN2 4
#define DATA_PIN3 2
#define DATA_PIN4 15
#define DATA_PIN5 5

//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];
CRGB leds5[NUM_LEDS];

#define BRIGHTNESS         200
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN1,COLOR_ORDER>(leds1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN2,COLOR_ORDER>(leds2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN3,COLOR_ORDER>(leds3, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN4,COLOR_ORDER>(leds4, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN5,COLOR_ORDER>(leds5, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, confetti, sinelon, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds1, NUM_LEDS, gHue, 7);
  fill_rainbow( leds2, NUM_LEDS, gHue, 7);
  fill_rainbow( leds3, NUM_LEDS, gHue, 7);
  fill_rainbow( leds4, NUM_LEDS, gHue, 7);
  fill_rainbow( leds5, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds1[ random16(NUM_LEDS) ] += CRGB::White;
    leds2[ random16(NUM_LEDS) ] += CRGB::White;
    leds3[ random16(NUM_LEDS) ] += CRGB::White;
    leds4[ random16(NUM_LEDS) ] += CRGB::White;
    leds5[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds1, NUM_LEDS, 10);
  fadeToBlackBy( leds2, NUM_LEDS, 10);
  fadeToBlackBy( leds3, NUM_LEDS, 10);
  fadeToBlackBy( leds4, NUM_LEDS, 10);
  fadeToBlackBy( leds5, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds1[pos] += CHSV( gHue + random8(64), 200, 255);
  leds2[pos] += CHSV( gHue + random8(64), 200, 255);
  leds3[pos] += CHSV( gHue + random8(64), 200, 255);
  leds4[pos] += CHSV( gHue + random8(64), 200, 255);
  leds5[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds1, NUM_LEDS, 20);
  fadeToBlackBy( leds2, NUM_LEDS, 20);
  fadeToBlackBy( leds3, NUM_LEDS, 20);
  fadeToBlackBy( leds4, NUM_LEDS, 20);
  fadeToBlackBy( leds5, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds1[pos] += CHSV( gHue, 255, 192);
  leds2[pos] += CHSV( gHue, 255, 192);
  leds3[pos] += CHSV( gHue, 255, 192);
  leds4[pos] += CHSV( gHue, 255, 192);
  leds5[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds1[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds2[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds3[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds4[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    leds5[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds1, NUM_LEDS, 20);
  fadeToBlackBy( leds2, NUM_LEDS, 20);
  fadeToBlackBy( leds3, NUM_LEDS, 20);
  fadeToBlackBy( leds4, NUM_LEDS, 20);
  fadeToBlackBy( leds5, NUM_LEDS, 20);

  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds1[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    leds2[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    leds3[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    leds4[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    leds5[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
