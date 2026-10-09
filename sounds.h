#ifndef SLOT_SOUNDS_H
#define SLOT_SOUNDS_H

// Non-blocking buzzer player for Arduino piezo buzzers.
//
// Hardware Pin: Connected to Analog Pin A5 by default (can be overridden before include).
//
// Usage in your main sketch:
//
//   #include "sounds.h"
//
//   void setup() {
//     setupBuzzer();
//   }
//
//   void loop() {
//     updateSounds();              // Required every loop iteration
//
//     // Trigger any sound non-blockingly:
//     // playTetrisTheme();         // Idle background music (loops)
//     // playGambleTheme();         // Short tension theme (loops)
//     // playLongGamblingChorus();  // Rolling suspense chorus (plays once)
//     // playBigWin();              // Jackpot fanfare (plays once)
//     // playLoseSound();           // Defeat sting (plays once)
//     // stopSound();               // Stop whatever is playing immediately
//   }
//
// None of the play* functions block or use delay(). Starting a new sound
// seamlessly cancels and replaces the previous one.

#include <Arduino.h>

#ifndef BUZZER_PIN
#define BUZZER_PIN A5
#endif

#ifndef REST
#define REST 0
#endif

// Note frequencies (Hz)
#ifndef NOTE_GS3
#define NOTE_GS3 208
#endif
#ifndef NOTE_A3
#define NOTE_A3  220
#endif
#ifndef NOTE_B3
#define NOTE_B3  247
#endif
#ifndef NOTE_C4
#define NOTE_C4  262
#endif
#ifndef NOTE_D4
#define NOTE_D4  294
#endif
#ifndef NOTE_DS4
#define NOTE_DS4 311
#endif
#ifndef NOTE_E4
#define NOTE_E4  330
#endif
#ifndef NOTE_F4
#define NOTE_F4  349
#endif
#ifndef NOTE_G4
#define NOTE_G4  392
#endif
#ifndef NOTE_GS4
#define NOTE_GS4 415
#endif
#ifndef NOTE_A4
#define NOTE_A4  440
#endif
#ifndef NOTE_B4
#define NOTE_B4  494
#endif
#ifndef NOTE_C5
#define NOTE_C5  523
#endif
#ifndef NOTE_D5
#define NOTE_D5  587
#endif
#ifndef NOTE_E5
#define NOTE_E5  659
#endif
#ifndef NOTE_F5
#define NOTE_F5  698
#endif
#ifndef NOTE_G5
#define NOTE_G5  784
#endif
#ifndef NOTE_GS5
#define NOTE_GS5 831
#endif
#ifndef NOTE_A5
#define NOTE_A5  880
#endif
#ifndef NOTE_B5
#define NOTE_B5  988
#endif
#ifndef NOTE_C6
#define NOTE_C6  1046
#endif
#ifndef NOTE_D6
#define NOTE_D6  1175
#endif
#ifndef NOTE_E6
#define NOTE_E6  1318
#endif
#ifndef NOTE_F6
#define NOTE_F6  1397
#endif
#ifndef NOTE_G6
#define NOTE_G6  1568
#endif
#ifndef NOTE_A6
#define NOTE_A6  1760
#endif
#ifndef NOTE_C7
#define NOTE_C7  2093
#endif

enum SoundId {
  SOUND_NONE = 0,
  SOUND_TETRIS,
  SOUND_GAMBLE,
  SOUND_BIG_WIN,
  SOUND_GAMBLING_CHORUS,
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

// --- Melody Definitions ---

inline const SoundNote* tetrisNotes(uint16_t& count) {
  static const SoundNote notes[] = {
#define TNOTE(freq, div) { (freq), \
    (uint16_t)((((div) > 0) ? ((60000L * 4) / 144 / (div)) \
                            : ((60000L * 4) / 144 / -(div)) * 3 / 2) * 9 / 10), \
    (uint16_t)(((div) > 0) ? ((60000L * 4) / 144 / (div)) \
                           : ((60000L * 4) / 144 / -(div)) * 3 / 2) }
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
#undef TNOTE
  };
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

inline const SoundNote* bigWinNotes(uint16_t& count) {
#define BNOTE(freq, tone, gap) { (freq), (uint16_t)(tone), (uint16_t)((tone) * (gap)) }
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
#undef BNOTE
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

inline const SoundNote* gambleNotes(uint16_t& count) {
#define BNOTE(freq, tone, gap) { (freq), (uint16_t)(tone), (uint16_t)((tone) * (gap)) }
  static const SoundNote notes[] = {
    BNOTE(NOTE_C5, 55, 1.12f), BNOTE(NOTE_E5, 55, 1.12f), BNOTE(NOTE_G5, 55, 1.12f), BNOTE(NOTE_C6, 55, 1.12f),
    BNOTE(NOTE_G5, 50, 1.12f), BNOTE(NOTE_E5, 50, 1.12f),
    BNOTE(NOTE_D5, 45, 1.10f), BNOTE(NOTE_F5, 45, 1.10f), BNOTE(NOTE_A5, 45, 1.10f), BNOTE(NOTE_D6, 45, 1.10f),
    BNOTE(NOTE_A5, 40, 1.10f), BNOTE(NOTE_F5, 40, 1.10f),
    BNOTE(NOTE_E5, 40, 1.08f), BNOTE(NOTE_G5, 40, 1.08f), BNOTE(NOTE_B5, 40, 1.08f), BNOTE(NOTE_E6, 40, 1.08f),
    BNOTE(NOTE_C6, 70, 1.15f), BNOTE(NOTE_E6, 70, 1.15f), BNOTE(NOTE_G6, 90, 1.20f),
    BNOTE(NOTE_E6, 50, 1.10f), BNOTE(NOTE_C6, 50, 1.10f), BNOTE(REST, 80, 1.00f)
  };
#undef BNOTE
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

// 1. Long Gambling Chorus (Rolling suspense + Cliffhanger hook)
inline const SoundNote* longGamblingChorusNotes(uint16_t& count) {
#define GNOTE(freq, dur, pause) { (freq), (uint16_t)(dur), (uint16_t)((uint16_t)((dur) * 1.30f) + (pause)) }
#define HNOTE(freq, dur, gap)   { (freq), (uint16_t)(dur), (uint16_t)((dur) * (gap)) }
  static const SoundNote notes[] = {
    // --- Phase 1: The Extended Rolling Sequence ---
    // Cycle 0 (duration 110ms)
    GNOTE(NOTE_G4, 110, 0), GNOTE(NOTE_D5, 110, 0), GNOTE(NOTE_B4, 110, 0),
    GNOTE(NOTE_E5, 110, 0), GNOTE(NOTE_C5, 110, 0), GNOTE(NOTE_G5, 110, 100),

    // Cycle 1 (duration 125ms)
    GNOTE(NOTE_G4, 125, 0), GNOTE(NOTE_D5, 125, 0), GNOTE(NOTE_B4, 125, 0),
    GNOTE(NOTE_E5, 125, 0), GNOTE(NOTE_C5, 125, 0), GNOTE(NOTE_G5, 125, 100),

    // Cycle 2 (duration 140ms)
    GNOTE(NOTE_G4, 140, 0), GNOTE(NOTE_D5, 140, 0), GNOTE(NOTE_B4, 140, 0),
    GNOTE(NOTE_E5, 140, 0), GNOTE(NOTE_C5, 140, 0), GNOTE(NOTE_G5, 140, 100),

    // Cycle 3 (duration 155ms - Pitch step-up)
    GNOTE(NOTE_C5, 155, 0), GNOTE(NOTE_D5, 155, 0), GNOTE(NOTE_E5, 155, 0),
    GNOTE(NOTE_E5, 155, 0), GNOTE(NOTE_C5, 155, 0), GNOTE(NOTE_G5, 155, 100),

    // Cycle 4 (duration 170ms)
    GNOTE(NOTE_C5, 170, 0), GNOTE(NOTE_D5, 170, 0), GNOTE(NOTE_E5, 170, 0),
    GNOTE(NOTE_E5, 170, 0), GNOTE(NOTE_C5, 170, 0), GNOTE(NOTE_G5, 170, 100),

    // Cycle 5 (duration 185ms - Pitch step-up to climax)
    GNOTE(NOTE_C5, 185, 0), GNOTE(NOTE_D5, 185, 0), GNOTE(NOTE_E5, 185, 0),
    GNOTE(NOTE_E5, 185, 0), GNOTE(NOTE_A5, 185, 0), GNOTE(NOTE_C6, 185, 100),

    // --- Phase 2: The Slower Suspense Hook ---
    HNOTE(NOTE_A5, 180, 1.40f),
    HNOTE(NOTE_B5, 180, 1.40f),
    HNOTE(NOTE_C6, 400, 1.20f)
  };
#undef GNOTE
#undef HNOTE
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

// 2. Lose Sound (The Shock drop -> Defeat descent -> Flat "wah-wah" finish)
inline const SoundNote* loseNotes(uint16_t& count) {
#define LNOTE(freq, tone, gap) { (freq), (uint16_t)(tone), (uint16_t)((tone) * (gap)) }
  static const SoundNote notes[] = {
    // Phase 1: The Shock (Quick falling drop)
    LNOTE(NOTE_G4,  120, 1.10f),
    LNOTE(NOTE_F4,  120, 1.10f),
    LNOTE(NOTE_E4,  120, 1.10f),
    { NOTE_DS4, 120, (uint16_t)((uint16_t)(120 * 1.10f) + 100) }, // Includes dramatic 100ms pause

    // Phase 2: The Defeat Descent (Slower, plodding steps)
    LNOTE(NOTE_D4,  250, 1.20f),
    LNOTE(NOTE_C4,  250, 1.20f),
    LNOTE(NOTE_B3,  250, 1.20f),
    LNOTE(NOTE_A3,  250, 1.20f),

    // Phase 3: The Ultimate "Wah-Wah" Bottom Out
    LNOTE(NOTE_GS3, 800, 1.10f)
  };
#undef LNOTE
  count = sizeof(notes) / sizeof(notes[0]);
  return notes;
}

// --- Player Core ---

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

// --- Playback Triggers ---

inline void playTetrisTheme() {
  if (currentSound() == SOUND_TETRIS) {
    return;
  }
  uint16_t count = 0;
  const SoundNote* notes = tetrisNotes(count);
  startSound(SOUND_TETRIS, notes, count, true);
}

inline void playGambleTheme() {
  if (currentSound() == SOUND_GAMBLE) {
    return;
  }
  uint16_t count = 0;
  const SoundNote* notes = gambleNotes(count);
  startSound(SOUND_GAMBLE, notes, count, true);
}

inline void playBigWin() {
  if (currentSound() == SOUND_BIG_WIN) {
    return;
  }
  uint16_t count = 0;
  const SoundNote* notes = bigWinNotes(count);
  startSound(SOUND_BIG_WIN, notes, count, false);
}

inline void playLongGamblingChorus(bool loop = false) {
  if (currentSound() == SOUND_GAMBLING_CHORUS) {
    return;
  }
  uint16_t count = 0;
  const SoundNote* notes = longGamblingChorusNotes(count);
  startSound(SOUND_GAMBLING_CHORUS, notes, count, loop);
}

inline void playLoseSound(bool loop = false) {
  if (currentSound() == SOUND_LOSE) {
    return;
  }
  uint16_t count = 0;
  const SoundNote* notes = loseNotes(count);
  startSound(SOUND_LOSE, notes, count, loop);
}

// Convenient aliases
inline void playGamblingChorus(bool loop = false) { playLongGamblingChorus(loop); }
inline void playLoseTheme(bool loop = false)      { playLoseSound(loop); }

// --- Engine Tick ---

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

#endif // SLOT_SOUNDS_H