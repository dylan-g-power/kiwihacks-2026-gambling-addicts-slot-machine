#include "sounds.h"

// Pin 7 is the buzzer, so it is skipped in the LED list.
const int LED_PINS[] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14};
const int LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const int BUTTON_PIN = A1;
const int BUTTON_PRESS_THRESHOLD = 900;

const unsigned long IDLE_CHASE_MS = 200;
const unsigned long GAMBLE_FLASH_MS = 100;
const unsigned long GAMBLE_DURATION_MS = 6000;
const unsigned long JACKPOT_FLASH_MS = 100;
const unsigned long BUTTON_DEBUG_MS = 250;

enum GameState {
  STATE_IDLE,
  STATE_GAMBLE,
  STATE_JACKPOT
};

GameState gameState = STATE_IDLE;
int chaseIndex = 0;
bool gambleEvenOn = true;
bool jackpotOn = true;
bool buttonWasPressed = false;
unsigned long lastLedMs = 0;
unsigned long stateStartMs = 0;
unsigned long lastButtonDebugMs = 0;

const char* stateName(GameState state) {
  switch (state) {
    case STATE_IDLE: return "IDLE";
    case STATE_GAMBLE: return "GAMBLE";
    case STATE_JACKPOT: return "JACKPOT";
    default: return "?";
  }
}

void allLeds(int level) {
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(LED_PINS[i], level);
  }
}

void setChaseLed(int index) {
  allLeds(LOW);
  digitalWrite(LED_PINS[index], HIGH);
}

void setGambleLeds(bool evenOn) {
  for (int i = 0; i < LED_COUNT; i++) {
    bool isEven = (LED_PINS[i] % 2) == 0;
    digitalWrite(LED_PINS[i], (isEven == evenOn) ? HIGH : LOW);
  }
}

void enterState(GameState next) {
  gameState = next;
  stateStartMs = millis();
  lastLedMs = millis();
  Serial.print(F("STATE -> "));
  Serial.println(stateName(next));

  switch (next) {
    case STATE_IDLE:
      chaseIndex = 0;
      setChaseLed(chaseIndex);
      playTetrisTheme();
      Serial.println(F("Idle: chase LEDs + Tetris. Press switch on A1 to spin."));
      break;
    case STATE_GAMBLE:
      gambleEvenOn = true;
      setGambleLeds(gambleEvenOn);
      playGambleTheme();
      Serial.println(F("Gambling: fast alternate LEDs + tension music."));
      break;
    case STATE_JACKPOT:
      jackpotOn = true;
      allLeds(HIGH);
      playBigWin();
      Serial.println(F("JACKPOT! All LEDs blink + win music."));
      break;
  }
}

bool buttonPressed() {
  return analogRead(BUTTON_PIN) >= BUTTON_PRESS_THRESHOLD;
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Reset"));

  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  pinMode(BUTTON_PIN, INPUT);
  setupBuzzer();
  buttonWasPressed = buttonPressed();

  Serial.print(F("Buzzer pin "));
  Serial.print(BUZZER_PIN);
  Serial.println(F(" skipped for LEDs"));
  Serial.print(F("Button A1 raw="));
  Serial.println(analogRead(BUTTON_PIN));

  enterState(STATE_IDLE);
}

void loop() {
  updateSounds();

  unsigned long now = millis();
  bool pressed = buttonPressed();

  if (now - lastButtonDebugMs >= BUTTON_DEBUG_MS) {
    lastButtonDebugMs = now;
    Serial.print(F("["));
    Serial.print(stateName(gameState));
    Serial.print(F("] A1="));
    Serial.print(analogRead(BUTTON_PIN));
    Serial.print(F(" pressed="));
    Serial.print(pressed ? F("YES") : F("no"));
    Serial.print(F(" sound="));
    Serial.println((int)currentSound());
  }

  switch (gameState) {
    case STATE_IDLE:
      playTetrisTheme();
      if (now - lastLedMs >= IDLE_CHASE_MS) {
        lastLedMs = now;
        chaseIndex = (chaseIndex + 1) % LED_COUNT;
        setChaseLed(chaseIndex);
      }
      if (pressed && !buttonWasPressed) {
        Serial.println(F("Switch pressed — starting spin"));
        enterState(STATE_GAMBLE);
      }
      break;

    case STATE_GAMBLE:
      playGambleTheme();
      if (now - lastLedMs >= GAMBLE_FLASH_MS) {
        lastLedMs = now;
        gambleEvenOn = !gambleEvenOn;
        setGambleLeds(gambleEvenOn);
      }
      if (now - stateStartMs >= GAMBLE_DURATION_MS) {
        Serial.println(F("Spin finished — hitting jackpot"));
        enterState(STATE_JACKPOT);
      }
      break;

    case STATE_JACKPOT:
      if (now - lastLedMs >= JACKPOT_FLASH_MS) {
        lastLedMs = now;
        jackpotOn = !jackpotOn;
        allLeds(jackpotOn ? HIGH : LOW);
      }
      if (!isSoundPlaying() && (now - stateStartMs >= 2000) && !pressed) {
        Serial.println(F("Jackpot done — back to idle"));
        enterState(STATE_IDLE);
      }
      break;
  }

  buttonWasPressed = pressed;
}
