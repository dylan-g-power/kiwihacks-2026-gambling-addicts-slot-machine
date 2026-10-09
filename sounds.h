#ifndef SLOT_SOUNDS_H
#define SLOT_SOUNDS_H

// Non-blocking buzzer player. Include in your main sketch:
//
//   #include "sounds.h"
//
//   void setup() {
//     setupBuzzer();
//   }
//
//   void loop() {
//     updateSounds();         // required every loop
//     playTetrisTheme();      // idle music (loops)
//     playGambleChorus();     // spinning / tension (plays once)
//     playBigWin();           // jackpot sting (plays once)
//     playLoseSound();        // defeat sting (plays once)
//   }
//
// Start functions never delay(). Playback is advanced by updateSounds().
// Starting one song cancels whichever was playing.

#include <Arduino.h>

#ifndef BUZZER_PIN
#define BUZZER_PIN 7
#endif

#ifndef REST
#define REST 0
#endif

#ifndef NOTE_GS3
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1046
#define NOTE_D6  1175
#define NOTE_E6  1318
#define NOTE_F6  1397
#define NOTE_G6  1568
#define NOTE_A6  1760
#define NOTE_C7  2093
#endif

enum SoundId {
  SOUND_NONE = 0,
  SOUND_TETRIS,
  SOUND_GAMBLE,
  SOUND_BIG_WIN,
  SOUND_LOSE
};

struct SoundNote {
  int freq;
  uint16_t toneMs;
  uint16_t stepMs;
};

struct SoundPlayer {
  const SoundNote* notes;
  uint16_t count;
  uint16_t index;
  unsigned long nextMs;
  SoundId current;
  bool loop;
};

inline SoundPlayer& soundPlayer() {
  static SoundPlayer player = { nullptr, 0, 0, 0, SOUND_NONE, false };
  return player;
}

#define TNOTE(freq, div) { (freq), \
    (uint16_t)((((div) > 0) ? ((60000L * 4) / 144 / (div)) \
                            : ((60000L * 4) / 144 / -(div)) * 3 / 2) * 9 / 10), \
    (uint16_t)(((div) > 0) ? ((60000L * 4) / 144 / (div)) \
                           : ((60000L * 4) / 144 / -(div)) * 3 / 2) }
#define BNOTE(freq, toneMs, gap) { (freq), (uint16_t)(toneMs), (uint16_t)((toneMs) * (gap)) }
#define PAUSE(ms) { REST, 0, (uint16_t)(ms) }

inline const SoundNote* tetrisNotes(uint16_t& count) {
  static const SoundNote notes[] = {
    TNOTE(NOTE_E5, 4), TNOTE(NOTE_B4, 8), TNOTE(NOTE_C5, 8), TNOTE(NOTE_D5, 4), TNOTE(NOTE_C5, 8), TNOTE(NOTE_B4, 8),
    TNOTE(NOTE_A4, 4), TNOTE(NOTE_A4, 8), TNOTE(NOTE_C5, 8), TNOTE(NOTE_E5, 4), TNOTE(NOTE_D5, 8), TNOTE(NOTE_C5, 8),
    TNOTE(NOTE_B4, -4), TNOTE(NOTE_C5, 8), TNOTE(NOTE_D5, 4), TNOTE(NOTE_E5, 4),
    TNOTE(NOTE_C5, 4), TNOTE(NOTE_A4, 4), TNOTE(NOTE_A4, 8), TNOTE(REST, 4),

    TNOTE(NOTE_D5, -4), TNOTE(NOTE_F5, 8), TNOTE(NOTE_A5, 4), TNOTE(NOTE_G5, 8), TNOTE(NOTE_F5, 8),
    TNOTE(NOTE_E5, -4), TNOTE(NOTE_C5, 8), TNOTE(NOTE_E5, 4), TNOTE(NOTE_D5, 8), TNOTE(NOTE_C5, 8),
    TNOTE(NOTE_B4, 4), TNOTE(NOTE_B4, 8), TNOTE(NOTE_C5, 8), TNOTE(NOTE_D5, 4), TNOTE(NOTE_E5, 4),
    TNOTE(NOTE_C5, 4), TNOTE(NOTE_A4, 4), TNOTE(NOTE_A4, 4), TNOTE(REST, 4),

    TNOTE(NOTE_E5, 2), TNOTE(NOTE_C5, 2),
    TNOTE(NOTE_D5, 2), TNOTE(NOTE_B4, 2),
    TNOTE(NOTE_C5, 2), TNOTE(NOTE_A4, 2),
    TNOTE(NOTE_GS4, 2), TNOTE(NOTE_B4, 4), TNOTE(REST, 8),
    TNOTE(NOTE_E5, 2), TNOTE(NOTE_C5, 2),
    TNOTE(NOTE_D5, 2), TNOTE(NOTE_B4, 2),
    TNOTE(NOTE_C5, 4), TNOTE(NOTE_E5, 4), TNOTE(NOTE_A5, 2),
    TNOTE(NOTE_GS5, 2)
  };
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

inline const SoundNote* bigWinNotes(uint16_t& count) {
  static const SoundNote notes[] = {
    BNOTE(NOTE_C5, 45, 1.15f), BNOTE(NOTE_D5, 45, 1.15f), BNOTE(NOTE_E5, 45, 1.15f),
    BNOTE(NOTE_G5, 45, 1.15f), BNOTE(NOTE_A5, 45, 1.15f), BNOTE(NOTE_C6, 45, 1.15f),
    BNOTE(NOTE_D6, 45, 1.15f), BNOTE(NOTE_E6, 45, 1.15f), BNOTE(NOTE_G6, 45, 1.15f),
    BNOTE(NOTE_A6, 45, 1.15f),

    BNOTE(NOTE_C7, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f), BNOTE(NOTE_E6, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f),
    BNOTE(NOTE_C7, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f), BNOTE(NOTE_E6, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f),
    BNOTE(NOTE_C7, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f), BNOTE(NOTE_E6, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f),
    BNOTE(NOTE_C7, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f), BNOTE(NOTE_E6, 60, 1.25f), BNOTE(NOTE_G6, 60, 1.25f),

    BNOTE(NOTE_G6, 100, 1.2f), BNOTE(NOTE_A6, 100, 1.2f), BNOTE(NOTE_C7, 500, 1.2f)
  };
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

// Flattened playLongGamblingChorus(): 6 slowing wheel cycles + suspense hook.
inline const SoundNote* gambleNotes(uint16_t& count) {
  static const SoundNote notes[] = {
    // cycle 0, duration 110
    BNOTE(NOTE_G4, 110, 1.30f), BNOTE(NOTE_D5, 110, 1.30f), BNOTE(NOTE_B4, 110, 1.30f),
    BNOTE(NOTE_E5, 110, 1.30f), BNOTE(NOTE_C5, 110, 1.30f), BNOTE(NOTE_G5, 110, 1.30f),
    PAUSE(100),
    // cycle 1, duration 125
    BNOTE(NOTE_G4, 125, 1.30f), BNOTE(NOTE_D5, 125, 1.30f), BNOTE(NOTE_B4, 125, 1.30f),
    BNOTE(NOTE_E5, 125, 1.30f), BNOTE(NOTE_C5, 125, 1.30f), BNOTE(NOTE_G5, 125, 1.30f),
    PAUSE(100),
    // cycle 2, duration 140
    BNOTE(NOTE_G4, 140, 1.30f), BNOTE(NOTE_D5, 140, 1.30f), BNOTE(NOTE_B4, 140, 1.30f),
    BNOTE(NOTE_E5, 140, 1.30f), BNOTE(NOTE_C5, 140, 1.30f), BNOTE(NOTE_G5, 140, 1.30f),
    PAUSE(100),
    // cycle 3, duration 155, pitch step-up
    BNOTE(NOTE_C5, 155, 1.30f), BNOTE(NOTE_D5, 155, 1.30f), BNOTE(NOTE_E5, 155, 1.30f),
    BNOTE(NOTE_E5, 155, 1.30f), BNOTE(NOTE_C5, 155, 1.30f), BNOTE(NOTE_G5, 155, 1.30f),
    PAUSE(100),
    // cycle 4, duration 170
    BNOTE(NOTE_C5, 170, 1.30f), BNOTE(NOTE_D5, 170, 1.30f), BNOTE(NOTE_E5, 170, 1.30f),
    BNOTE(NOTE_E5, 170, 1.30f), BNOTE(NOTE_C5, 170, 1.30f), BNOTE(NOTE_G5, 170, 1.30f),
    PAUSE(100),
    // cycle 5, duration 185, higher hook pitches
    BNOTE(NOTE_C5, 185, 1.30f), BNOTE(NOTE_D5, 185, 1.30f), BNOTE(NOTE_E5, 185, 1.30f),
    BNOTE(NOTE_E5, 185, 1.30f), BNOTE(NOTE_A5, 185, 1.30f), BNOTE(NOTE_C6, 185, 1.30f),
    PAUSE(100),
    // suspense hook
    BNOTE(NOTE_A5, 180, 1.4f), BNOTE(NOTE_B5, 180, 1.4f), BNOTE(NOTE_C6, 400, 1.2f)
  };
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

inline const SoundNote* loseNotes(uint16_t& count) {
  static const SoundNote notes[] = {
    BNOTE(NOTE_G4, 120, 1.1f), BNOTE(NOTE_F4, 120, 1.1f),
    BNOTE(NOTE_E4, 120, 1.1f), BNOTE(NOTE_DS4, 120, 1.1f),
    PAUSE(100),
    BNOTE(NOTE_D4, 250, 1.2f), BNOTE(NOTE_C4, 250, 1.2f),
    BNOTE(NOTE_B3, 250, 1.2f), BNOTE(NOTE_A3, 250, 1.2f),
    BNOTE(NOTE_GS3, 800, 1.1f)
  };
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

#undef TNOTE
#undef BNOTE
#undef PAUSE

inline void setupBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
}

inline void stopSound() {
  SoundPlayer& p = soundPlayer();
  p.notes = nullptr;
  p.count = 0;
  p.index = 0;
  p.current = SOUND_NONE;
  p.loop = false;
  noTone(BUZZER_PIN);
}

inline bool isSoundPlaying() {
  return soundPlayer().current != SOUND_NONE;
}

inline SoundId currentSound() {
  return soundPlayer().current;
}

inline void startSound(SoundId id, const SoundNote* notes, uint16_t count, bool loop) {
  SoundPlayer& p = soundPlayer();
  p.notes = notes;
  p.count = count;
  p.index = 0;
  p.nextMs = millis();
  p.current = id;
  p.loop = loop;
}

inline void playTetrisTheme() {
  if (currentSound() == SOUND_TETRIS) {
    return;
  }
  uint16_t count = 0;
  startSound(SOUND_TETRIS, tetrisNotes(count), count, true);
}

inline void playGambleChorus() {
  if (currentSound() == SOUND_GAMBLE) {
    return;
  }
  uint16_t count = 0;
  startSound(SOUND_GAMBLE, gambleNotes(count), count, false);
}

inline void playLongGamblingChorus() {
  playGambleChorus();
}

inline void playGambleTheme() {
  playGambleChorus();
}

inline void playBigWin() {
  if (currentSound() == SOUND_BIG_WIN) {
    return;
  }
  uint16_t count = 0;
  startSound(SOUND_BIG_WIN, bigWinNotes(count), count, false);
}

inline void playLoseSound() {
  if (currentSound() == SOUND_LOSE) {
    return;
  }
  uint16_t count = 0;
  startSound(SOUND_LOSE, loseNotes(count), count, false);
}

inline void updateSounds() {
  SoundPlayer& p = soundPlayer();
  if (p.current == SOUND_NONE || p.notes == nullptr) {
    return;
  }

  unsigned long now = millis();
  if ((long)(now - p.nextMs) < 0) {
    return;
  }

  if (p.index >= p.count) {
    if (p.loop) {
      p.index = 0;
    } else {
      stopSound();
      return;
    }
  }

  const SoundNote& note = p.notes[p.index++];
  if (note.freq > 0) {
    tone(BUZZER_PIN, note.freq, note.toneMs);
  } else {
    noTone(BUZZER_PIN);
  }
  p.nextMs = now + note.stepMs;
}

#endif
