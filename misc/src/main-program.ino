#include "sound.h"
// turns on a strip of LEDs, with fancy patterns
int delayTimer = 200;
bool hasRun = false;
bool chorushasRun = false;
bool losehasRun = false;
int slotState = 3;

int flashTimer = 0;
int cycles = 0;

unsigned long flashMillis = 0;
unsigned long winMillis = 0;

unsigned long previousMillis = 0;
const unsigned long interval = 200; // Interval in milliseconds (0.5 seconds)

int currentPin = 2; // Track which pin is currently active

bool buttonWasReleased = true; // wait until the operator lets go

void setup() {
  // Initialize digital pins 2 through 14 as outputs
  for (int thisPin = 2; thisPin <= 14; thisPin++){
    pinMode(thisPin, OUTPUT);
  }
  pinMode(A0, INPUT);
  pinMode(A5, OUTPUT);
  setupBuzzer();
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  bool buttonPressed = (analogRead(A0) == 1023);

  if (!buttonPressed) {
    buttonWasReleased = true;
  }

  if (slotState == 0){
    cycles = 0;
    playTetrisTheme();
    // Trigger only when the 500ms interval has passed
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // 1. Turn OFF the previous LED safely
      if (currentPin == 2) {
        digitalWrite(14, LOW); // If we are at the start, turn off the last pin (14)
      } else {
        digitalWrite(currentPin - 1, LOW); // Otherwise, turn off the pin behind us
      }

      // 2. Turn ON the current LED
      digitalWrite(currentPin, HIGH);

      // 3. Move to the next pin for the next interval cycle
      currentPin++;
      
      // If we pass the last pin, wrap back around to the first pin (2)
      if (currentPin > 14) {
        currentPin = 2;
      }
    }
    // fire once on press, not while the button is held
    //if (buttonPressed && buttonWasReleased){
    //  buttonWasReleased = false;
    //  slotState = 1;
    //  hasRun = false;
    //  flashMillis = 0;
    //  cycles = 0;
    //}     
  }

  if (slotState == 1){
   if (!hasRun) {
      stopSound();
      playBigWin();
      hasRun = true;
   }

    flashMillis++;
    if (flashMillis < 50){
    digitalWrite(2,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(12,HIGH);

    digitalWrite(3,LOW);
    digitalWrite(5,LOW);
    digitalWrite(7,LOW);
    digitalWrite(9,LOW);
    digitalWrite(11,LOW);
    digitalWrite(13,LOW);
    }

    else{
    digitalWrite(3,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(11,HIGH);
    digitalWrite(13,HIGH);

    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
    digitalWrite(6,LOW);
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    digitalWrite(12,LOW);
    }

    if (flashMillis > 100){
      flashMillis = 0;
      cycles++; // one flash period (~100ms), not one loop
    }

    // stay in win until enough flashes AND the button is released
    if (cycles > 10 && buttonWasReleased){
      slotState = 0;
      hasRun = false;
      flashMillis = 0;
      cycles = 0;
      currentPin = 2;
      previousMillis = millis();
    }
  }

  if (slotState == 2){
    if (!chorushasRun) {
      stopSound();
      playLongGamblingChorus();
      chorushasRun = true;
   }
    winMillis++;
    if (winMillis < 50){
      digitalWrite(2,LOW);
      digitalWrite(3,HIGH);
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      digitalWrite(7,HIGH);
      digitalWrite(8,LOW);
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(13,HIGH);
      }
      else{
      for (int i = 2; i <= 13; i++) {
        digitalWrite(i, LOW);    // Set the pin LOW
      }
      }    
      if (winMillis > 100){
      winMillis = 0;
      cycles++; // one flash period (~100ms), not one loop
    }
  }

  if (slotState == 3){
    if (!losehasRun) {
      stopSound();
      playLoseSound();
      losehasRun = true;
   }
    for (int i = 2; i <= 13; i++) {
      digitalWrite(i, LOW);    // Set all pins LOW
    }
  }

updateSounds();

Serial.read();

if (analogRead(A0) == 1023){
  Serial.write(1);
}
}