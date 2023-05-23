// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include "pitches.h"
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN        5 // LED Pin Number for Both LED's 
#define BUZZER_PIN 7 // Speaker Pin Number (External)
#define buttonPin 0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12 // Popular NeoPixel ring size

// Neopixel LED Setup 
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 2500 // Time (in milliseconds) to pause between pixels

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

// Button initilization
volatile int buttonState = 1;


void interuptHandler() { 
  if (buttonState==0){
    buttonState=1;
  }
  else {
    buttonState=0;
  }
}
void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // initialize button as input - changed to INPUT_PULLUP for new button
  //These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  //Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
   clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  attachInterrupt(digitalPinToInterrupt(buttonPin), interuptHandler, CHANGE);
}

void loop() {
  while(buttonState==0) {
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
      
    pixels.clear(); // Set all pixel colors to 'off'

    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.

    // MODE 1: Solid Green Entire Time
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));

      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop
    }

    // Ends Red
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright red color:
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));

      pixels.show();   // Send the updated pixel colors to the hardware.
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
    delay(3000);
    pixels.clear();

    // MODE 2: Gradually goes from purple to green with each tick
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
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(200-((200/12)*i), 0+((255/12)*i), 255-((255/12)*i)));

      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop
    }
    // Ends Green
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));

      pixels.show();   // Send the updated pixel colors to the hardware.
    } 
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
    delay(5000);
    pixels.clear();

    // MODE 3: Gradually goes from red to green with each tick
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
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(255-((255/12)*i), 0+((255/12)*i), 0));

      pixels.show();   // Send the updated pixel colors to the hardware.

      delay(DELAYVAL); // Pause before next pass through loop
    }
    // Ends Green
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));

      pixels.show();   // Send the updated pixel colors to the hardware.
    } 
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
    delay(5000);
    pixels.clear();

    // MODE 4: Whole Strip gradually goes from one color to another as time passes
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
    int tickTime = 30;
    for(int i=0; i<tickTime; i++) { // For each pixe
      for(int j=0; j<tickTime; j++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(j, pixels.Color(255-((255/tickTime)*i), 0+((255/tickTime)*i), 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      }
      delay(1000); // Pause before next pass through loop
    }
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
    delay(5000);
    pixels.clear();

    // MODE 5: Already lit ticks of the strip gradually goes from one color to another as time passes
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
    for(int i=0; i<NUMPIXELS; i++) { // For each pixe
      for(int j=0; j<i+1; j++) { // For each pixel...
      pixels.setPixelColor(j, pixels.Color(255-((255/12)*i), 0+((255/12)*i), 0));
      pixels.show();   
      }
      delay(DELAYVAL); // Pause before next pass through loop
    }
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
    pixels.clear();
      
    delay(5000);  
  }
}
