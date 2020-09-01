/*
 Button

 Turns on and off the built-in LED when the built-in button is
 pressed.

 Ported to Maple from the Arduino example 27 May 2011
 by Marti Bolivar
 Arduino pin  2 = B7
Arduino pin  3 = B6
Arduino pin  4 = B5
Arduino pin  5 = B4
Arduino pin  6 = B3
Arduino pin  7 = A15
Arduino pin  8 = A12
Arduino pin  9 = A11
Arduino pin 10 = A10
Arduino pin 11 = A9
Arduino pin 12 = A8
Arduino pin 13 = B15
Arduino pin 14 = B14
Arduino pin 15 = B13
Arduino pin 16 = null
Arduino pin 17 = C13, onboard led, input only.
Arduino pin 18 = C14
Arduino pin 19 = C15
Arduino pin 20 = A0
Arduino pin 21 = A1
Arduino pin 22 = A2
Arduino pin 23 = A3
Arduino pin 24 = A4
Arduino pin 25 = A5
Arduino pin 26 = A6
Arduino pin 27 = A7
Arduino pin 28 = B0
Arduino pin 29 = B1
Arduino pin 30 = B10
Arduino pin 31 = B11
*/
#define  BOARD_BUTTON_PIN  20 

void setup() {
  // Initialize the built-in LED pin as an output:
  pinMode(17, OUTPUT);
  // Initialize the built-in button (labeled BUT) as an input:
  pinMode(BOARD_BUTTON_PIN, INPUT);
}

#define BUTTON_DEBOUNCE_DELAY 1
uint8 isButtonPressed(uint8 pin=BOARD_BUTTON_PIN,
                      uint32 pressedLevel=BOARD_BUTTON_PRESSED_LEVEL) {
    if (digitalRead(pin) == pressedLevel) {
        delay(BUTTON_DEBOUNCE_DELAY);
        while (digitalRead(pin) == pressedLevel)
            ;
        return true;
    }
    return false;
}

void loop() {
    // Check if the button is pressed.
    if (isButtonPressed()) {
        // If so, turn the LED from on to off, or from off to on:
        digitalWrite(17,!digitalRead(17));// Turn the LED from off to on, or on to off
    }
}
