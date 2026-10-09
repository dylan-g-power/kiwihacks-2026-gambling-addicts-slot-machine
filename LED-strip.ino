// turns on a strip of LEDs, with fancy patterns
int delayTimer = 400;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  int ledpin = 0;
  for (int thisPin = 2; thisPin <= 13; thisPin++){
  pinMode(thisPin, OUTPUT);
  }
}

// the loop routine runs over and over again forever:
void loop() {

  for (int counter = 2; counter <= 10; counter++){
    for (int currentPin = 2; currentPin <= 13; currentPin++){
      digitalWrite(currentPin, HIGH);
      digitalWrite(currentPin - 1, LOW);
      delayTimer = delayTimer - 2;
      delay(delayTimer);
    }
    if (delayTimer > 6){
      break;
    }
  }
}
