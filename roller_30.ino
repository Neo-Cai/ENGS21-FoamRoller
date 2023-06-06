/*
Neo Cai and Maria Cristofo
Spring 2023 
ENGS 21 Group 3 Roller Project 
For the purposes of Prototype 3.0 user testing, this program creates a progress bar-like timer for neopixel LEDs 
that wrap around a roller. It plays a short melody to indicate start and finish and has a button that turns it on and off.
*/

// Import libraries
#include <Adafruit_NeoPixel.h>
#include "LSM6DS3.h"
#include "pitches.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Iniitilizaing Pins
#define PIN        5 // LED Pin number
#define Button_PIN 10 // Button Pin number
#define BUZZER_PIN 7 // Speaker Pin Number (External)
#define ButtonLED_PIN 9 // Button LED Pin Number
#define NUMPIXELS 12 // Popular NeoPixel ring size
#define Waker_PIN 0 // LED Waker

// Initializing Objects and Variables
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // Initializes adafruit Neo Pixels
Adafruit_NeoPixel pixelsWake(NUMPIXELS, Waker_PIN, NEO_GRB + NEO_KHZ800); // Initializes adafruit Neo Pixels

#define DELAYVAL 5000 // Time (in milliseconds) to pause between pixels

LSM6DS3 myIMU(I2C_MODE, 0x6A); 

volatile bool buttonOn = false; // Button state is off first, 0 is "on" and 1 is "off" so a press is a "falling action"
volatile bool rolling = true; // starts as true, as user starts rolling
volatile float prevX = 6; // start above 5
volatile float prevY = 6;

int melody0[] = { // Starting music theme (star trek)
  NOTE_D4, NOTE_G4, NOTE_C5, 
  NOTE_B4, NOTE_G4, NOTE_E4, NOTE_A4,
  NOTE_D5
};

int noteDurations0[] = { // Note durations for theme
  8, 16, 4, 
  8, 16, 16, 16,
  2,
};

int melody1[] = { // Ending theme (jingle bells)
  // Notes goes here
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
};
 
int noteDurations1[] = { // Note durations for theme
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
};

// interuptHandler allows code to intervene at any part of the void loop when button is pressed
void interuptHandler() { 
  buttonOn = !buttonOn; // When pressed, button state flips
  if(buttonOn) {
    digitalWrite(ButtonLED_PIN, HIGH);
    pixels.setPixelColor(1, pixels.Color(0, 255, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
  } // If button is TRUE (on) the light is on
  else {
    digitalWrite(ButtonLED_PIN, LOW);
  } // Else, off
}

void delayChecker(int time, bool state){ 
  for(int i = 1; i <= time/1000; i++){
    if(buttonOn==!state){
      break;
    } else { 
      delay(1000);
    }
  }
}

// Set up 
void setup() {
  pinMode(Button_PIN, INPUT_PULLUP); // Set up button with Pullup 
  pinMode(ButtonLED_PIN, OUTPUT); // Set up button LED
  digitalWrite(ButtonLED_PIN, LOW); // Set button LED as off by default

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000) // Trinket specific code we don't need 
  clock_prescale_set(clock_div_1);
#endif

 Serial.begin(9600);
 myIMU.begin();

 // remove this for serial to work connected to battery pack
  // while (!Serial);
  // if (myIMU.begin() != 0) {
  //     Serial.println("Device error");
  // } else {
  //     Serial.println("Device OK!");
  // }


  pixels.begin(); // Set up neopixel strips 
  pixelsWake.begin();
  attachInterrupt(digitalPinToInterrupt(Button_PIN), interuptHandler, FALLING); // Set up interupt, (button pin, method name, button action that trigers it)
}

// checks if gyro meets certain boundaries
bool isRolling(float x, float y){
  // not rolling: gyro x and y are within abs 5 of zero
  // rolling: gyro x and y not within abs 5 of 0
  x = abs(x);
  y = abs(y); // get absolute values

  // Serial.print("x and y: ");
  // Serial.print(x);
  // Serial.print(" ");
  // Serial.println(y);

  // Serial.print(prevX);
  // Serial.print(" ");
  // Serial.println(prevY);

  if((x > 2 && y > 2) && (prevX > 2 && prevY > 2)){
    // rolling
    prevX = x;
    prevY = y;
    Serial.print(prevX);
    Serial.print(" ");
    Serial.println(prevY);
    return true;
  } else{
    // not rolling
    prevX = x;
    prevY = y;
    return false;
  }

}

// Main Loop
void loop() {

  // Wake function, draws current from battery every 15 seconds to keep awake
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        pixelsWake.setPixelColor(i, pixels.Color(255, 255, 255));
        pixelsWake.show();   // Send the updated pixel colors to the hardware. 
  }  

  float gyroX = myIMU.readFloatGyroX();
  float gyroY = myIMU.readFloatGyroY();
  rolling = isRolling(gyroX, gyroY);
  Serial.print("check outside: ");
  Serial.println(rolling);

  if (rolling){
    Serial.print("rollingL ");
    Serial.println(rolling);
    Serial.print("button on: ");
    Serial.println(buttonOn);
  }
  // If button is on
  if (buttonOn && rolling){
    // Play opening melody
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

    // Light up LEDs (red to green)
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      float gyroX = myIMU.readFloatGyroX();
      float gyroY = myIMU.readFloatGyroY();
      rolling = isRolling(gyroX, gyroY);
      // roll is 1 (true) or 0 (false)
      Serial.print("check inside 1: ");
      Serial.println(rolling);
      // Check if button is on or off 
      if (buttonOn && rolling){ 
        // if on, light next pixel (gradually gets more green)
        pixels.setPixelColor(i, pixels.Color(255-((200/12)*i), 0+((200/12)*i), 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
        pixelsWake.setPixelColor(i, pixels.Color(0, 0, 0));// turn off wake LED
        pixelsWake.show();
        delayChecker(DELAYVAL, true); // Pause before next pass through loop
      } else {
        // if off, end for loop and go to the next one
        pixels.clear();
        pixels.show();
        break;
      }
    }
  }

  // if on, enter for loop - Green
  if(buttonOn && rolling) {

    // Light up all LEDs Green for completion
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        // check if button is on or off 
        float gyroX = myIMU.readFloatGyroX();
        float gyroY = myIMU.readFloatGyroY();
        rolling = isRolling(gyroX, gyroY);
        // roll is 1 (true) or 0 (false)
        Serial.print("check inside 2: ");
        Serial.println(rolling);
      if(buttonOn && rolling) {
        pixelsWake.clear();// turn off wake LED
        pixelsWake.show();
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

    // Play ending theme
  
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
