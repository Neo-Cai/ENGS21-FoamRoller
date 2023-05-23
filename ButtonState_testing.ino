// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        5 // 
#define Button_PIN 0 // 
#define NUMPIXELS 12 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 5000 // Time (in milliseconds) to pause between pixels

// Button initilization
volatile bool buttonOn = false; //0 is "on" and 1 is "off"

// interuptHandler allows code to intervene at anypart of the void loop when button is pressed
void interuptHandler() { 
  buttonOn = !buttonOn;
}
void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  pinMode(Button_PIN, INPUT_PULLUP);

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
    pixels.clear(); // Set all pixel colors to 'off'

    // Lights up each LED every "DELAYVAL" 
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // Check if button is on or off 
      if (buttonOn){ 
        // if on, light next pixel
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
        delay(DELAYVAL); // Pause before next pass through loop
      } else {
        // if off, end for loop and go to the next one
        break;
      }
    }

    // if on, enter for loop 
    if(buttonOn) {
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        // check if button is on or off 
        if(buttonOn) {
          // if on, lights everything red
          pixels.setPixelColor(i, pixels.Color(255, 0, 0));

          pixels.show();   // Send the updated pixel colors to the hardware.
        } else {
          // if off, exit for loop
          break;
        }       
      }
    }
    // wait 5 seconds before restarting cycle 
    delay(5000);
  } 
}
