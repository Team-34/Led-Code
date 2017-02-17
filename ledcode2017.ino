#include <LED.h>
#include <FastLED.h>
#include <Wire.h>

//Here we define the Pin that the LEDs are plugged into
#define DATA_PIN 2
/*this is the LED type, ask Mr. Wiggles, if you are unsure,
but it is almost always WS2812*/
# define LED_TYPE WS2812
//I would probably leave this as is
# define COLOR_ORDER GRB
//The number of LEDs in the strip; Adrianna would happily count them for you
# define NUM_LEDS 258
//arbitrary brightness value between 0 and 255
#define BRIGHTNESS 200
//I dont think this does anything, but leave it there in case
#define FRAMES_PER_SECOND 120
//Defines the strip for the library;
CRGB leds[NUM_LEDS];

//0 is for Blue, 1 is for Red
int color = 0;
bool isClimb = false;
CRGB alliance;

void setup() {
	//clears the strip
	FastLED.clearData();
	FastLED.clear();
	delay(3000);//insanity delay
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);//adds the leds to the library
	FastLED.setBrightness(BRIGHTNESS);//sets brightness
	Serial.begin(9600);//begin serial communication
	//this may or may not do anything
	if (Serial.available() > 0) {
		lightCode(Serial.read());
	}
}
//shhh, leave the next 4 here and dont do anything
typedef void(*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
//the demo code
void demo() {
	// Call the current pattern function once, updating the 'leds' array
	gPatterns[gCurrentPatternNumber]();

	// send the 'leds' array out to the actual LED strip
	FastLED.show();
	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / FRAMES_PER_SECOND);

	// do some periodic updates
	EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
	EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
//start of the demo code
void nextPattern()
{
	// add one to the current pattern number, and wrap around at the end
	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow()
{
	// FastLED's built-in rainbow generator
	fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
	// built-in FastLED rainbow, plus some random sparkly glitter
	rainbow();
	addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter)
{
	if (random8() < chanceOfGlitter) {
		leds[random16(NUM_LEDS)] += CRGB::White;
	}
}

void confetti()
{
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy(leds, NUM_LEDS, 10);
	int pos = random16(NUM_LEDS);
	leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, NUM_LEDS, 20);
	int pos = beatsin16(13, 0, NUM_LEDS);
	leds[pos] += CHSV(gHue, 255, 192);
}

void bpm() {
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	uint8_t BeatsPerMinute = 62;
	CRGBPalette16 palette = PartyColors_p;
	uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
	for (int i = 0; i < NUM_LEDS; i++) { //9948
		leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
	}
}

void juggle() {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy(leds, NUM_LEDS, 20);
	byte dothue = 0;
	for (int i = 0; i < 8; i++) {
		leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}
//end of the demo code
void lightCode(char command) {
	if (color == 0)
	{
		alliance = CRGB::Blue;
	}
	else if (color == 1) {
		alliance = CRGB::Red;
	}
	//command 0 and 1 set alliance color
	if (command == '0') {
		color = 1;
		alliance = CRGB::Red;
		for (int x = 0; x <= NUM_LEDS; x++) {
			leds[x] = CRGB::Red;
		}
		FastLED.show();
	}
	else if (command == '1') {
		color = 0;
		alliance = CRGB::Blue;
		for (int x = 0; x <= NUM_LEDS; x++) {
			leds[x] = CRGB::Blue;
		}
		FastLED.show();
	}
	//this runs the last 30 seconds code
	else if (command == '2') {
		if (color == 0) {
			for (int x = 0; x <= NUM_LEDS; x++) {
				leds[x] = alliance;
			}
			FastLED.show();
			delay(150);

			for (int x = 0; x <= NUM_LEDS; x++) {
				leds[x] = CRGB::Black;
			}
			FastLED.show();
			delay(150);
		}
	}
	//climbing code
	else if (command == '3') {
		for (int x = 0; x <= NUM_LEDS; x++) {
			leds[x] = CRGB::Green;
			FastLED.show();
		}
		while (true) {
			for (int x = 0; x <= NUM_LEDS; x++) {
				leds[x] = CRGB::Green;
			}
			FastLED.show();
			delay(150);

			for (int x = 0; x <= NUM_LEDS; x++) {
				leds[x] = CRGB::Black;
			}
			FastLED.show();
			delay(150);
		}
	}
	//not aligned with goal
	else if (command == '4') {
		for (int i = 0; i <= NUM_LEDS; i++)
		{
			leds[i] = CRGB::Red;
		}
		FastLED.show();
		delay(500);
		for (int i = 0; i <= NUM_LEDS; i++) {
			leds[i] = CRGB::Black;
		}
		delay(500);
	}
	//somewhat aligned
	else if (command == '5') {
		for (int i = 0; i <= NUM_LEDS; i++) {
			leds[i] = CRGB::Orange;
		}
		delay(250);
		for (int i = 0; i <= NUM_LEDS; i++) {
			leds[i] = CRGB::Black;
		}
		delay(250);
	}
	//very aligned
	else if (command == '6') {
		for (int i = 0; i <= NUM_LEDS; i++) {
			leds[i] = CRGB::Green;
			FastLED.show();
		}
		for (int i = 0; i <= NUM_LEDS; i++) {
			leds[i] = CRGB::Black;
		}
		delay(100);
	}
	//run the demo reel
	else if (command == '7') {
		demo();
	}
}

void loop() {
	if (Serial.available() > 0) {
		char bi = Serial.read();
		Serial.println(bi);
		lightCode(bi);
	}
	//demo;
	//lightCode(1);
	//delay(10000);
	//lightCode(2);
	//lightCode(3);
	//demo();
}
