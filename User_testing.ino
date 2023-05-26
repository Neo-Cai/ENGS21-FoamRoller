// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include "pitches.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        5 // 
#define Button_PIN 0 // 
#define BUZZER_PIN 7 // Speaker Pin Number (External)
#define ButtonLED_PIN 1 //
#define NUMPIXELS 12 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 5000 // Time (in milliseconds) to pause between pixels

// Button initilization
volatile bool buttonOn = false; //0 is "on" and 1 is "off"

int melody0[] = {
  NOTE_D4, NOTE_G4, NOTE_C5, 
  NOTE_B4, NOTE_G4, NOTE_E4, NOTE_A4,
  NOTE_D5
};

int noteDurations0[] = {
  8, 16, 4, 
  8, 16, 16, 16,
  2,
};

int melody1[] = {
  // Notes goes here
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
};
 
int noteDurations1[] = {
  // Notes duration goes here
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
};

// interuptHandler allows code to intervene at anypart of the void loop when button is pressed
void interuptHandler() { 
  buttonOn = !buttonOn;
  if(buttonOn) {digitalWrite(ButtonLED_PIN, HIGH);}
  else {digitalWrite(ButtonLED_PIN, LOW);}
}

void clearPixels(int currIndex){
  for (int i = 0; i < currIndex; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show(); 
  }
}

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  pinMode(Button_PIN, INPUT_PULLUP);
  pinMode(ButtonLED_PIN, OUTPUT);
  digitalWrite(ButtonLED_PIN, LOW);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  attachInterrupt(digitalPinToInterrupt(Button_PIN), interuptHandler, FALLING);
}

void loop() {
  // If button is on
  if (buttonOn){
    int size0 = sizeof(noteDurations0) / sizeof(int);
    for (int thisNote = 0; thisNote < size0; thisNote++) {
      // to calculate the note duration, take one second divided by the note type
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration0 = 1000 / noteDurations0[thisNote];
        tone(BUZZER_PIN, melody0[thisNote], noteDuration0);
                
        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration0 * 1.60;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(BUZZER_PIN);
    }
    // Lights up each LED every "DELAYVAL" 
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // Check if button is on or off 
      if (buttonOn){ 
        // if on, light next pixel
        pixels.setPixelColor(i, pixels.Color(255-((255/12)*i), 0+((214/12)*i), 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
        delay(DELAYVAL); // Pause before next pass through loop
      } else {
        // if off, end for loop and go to the next one
        pixels.clear();
        pixels.show();
        break;
      }
    }
  }
  // if on, enter for loop - Green
  if(buttonOn) {
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        // check if button is on or off 
      if(buttonOn) {
          // if on, lights everything red
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));

        pixels.show();   // Send the updated pixel colors to the hardware.
      } else {
          // if off, exit for loop
        pixels.clear();
        pixels.show();
        break;
      }       
    }
    int size1 = sizeof(noteDurations1) / sizeof(int); 
    for (int thisNote = 0; thisNote < size1; thisNote++) {
    // to calculate the note duration, take one second divided by the note type
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration1 = 1000 / noteDurations1[thisNote];
      tone(BUZZER_PIN, melody1[thisNote], noteDuration1);
              
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration1 * 1.60;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(BUZZER_PIN);
    }
  }
    // wait 5 seconds before restarting cycle 
  pixels.clear();
  pixels.show();
  delay(5000);
} 

