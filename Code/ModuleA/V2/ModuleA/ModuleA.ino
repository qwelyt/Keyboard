// ModuleA
//
// Layout:
//  col(pin)|1(7)  |2(8)  |3(9)  |4(10)  |5(11)  |6(12)  |7(13)
//  row(pin)+------+------+------+------+-------+-------+-----
//   1(2)   |      |      |1     |2     |3      |4      |5
//   2(3)   |Tab   |      |Q     |W     |E      |R      |T
//   3(4)   |ESC   |      |A     |S     |D      |F      |G
//   4(5)   |Shft  |<>|   |Z     |X     |C      |V      |B
//   5(6)   |Ctrl  |GUI   |Alt   |Lower |Raise  |       |Space
//
// Modifier bitfield:
//  bit 0: left control
//  bit 1: left shift
//  bit 2: left alt
//  bit 3: left GUI (Win/Apple/Meta key)
//  bit 4: right control
//  bit 5: right shift
//  bit 6: right alt
//  bit 7: right GUI

#include "Keyboard.h"
#include "Key.h"

#define numCols 7
#define numRows 5
#define scanRounds 2

int blockPin = A0;
int delayTimeBetweenScans = 10;

int cols[] = {7, 8, 9, 10, 11, 12, 13};
int rows[] = {2, 3, 4, 5, 6};

uint8_t lastFrame[8] = {0};

uint8_t keys[numRows][numCols] = {
  {Key::NONE      , Key::NONE      , Key::K1    , Key::K2    , Key::K3        , Key::K4    , Key::K5}
  , {Key::TAB     , Key::NONE      , Key::Q     , Key::W     , Key::E         , Key::R     , Key::T}
  , {Key::ESC     , Key::NONE      , Key::A     , Key::S     , Key::D         , Key::F     , Key::G}
  , {Key::L_SHFT  , Key::BS_N_PIPE , Key::Z     , Key::X     , Key::C         , Key::V     , Key::B}
  , {Key::L_CTRL  , Key::L_SUPR    , Key::L_ALT , Key::NONE  , Key::NONE      , Key::NONE  , Key::SPACE} // Raise and lower not done
};

bool pressed[scanRounds][numRows][numCols] = {};

bool lastState[numRows][numCols] = {false};
bool state[numRows][numCols] = {false};

///////////////

void setup() {
  while(analogRead(blockPin) < 100){} // <-- Block everything if we short {blockPin} to GND
  // This should help is a reprogramming is needed. Just short those and it should behave as an arduino instead of a keyboard.
  
  for (byte b = 0; b < numRows; ++b) {
    setupRow(rows[b]);
  }
  for (byte b = 0; b < numCols; ++b) {
    setupCol(cols[b]);
  }
  initKeyboard();
  Serial.begin(115200);
}

void setupRow(byte pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void setupCol(byte pin) {
  pinMode(pin, INPUT_PULLUP);
}

///////////////

void loop() {
  scan();
  if (stateHasChanged(lastState, state)) {
    buildBufferAndSend(state);
    saveState(state);
  }
}

/*******
  Scan
*******/
void scan() {
  for (byte b = 0; b < scanRounds; ++b) {
    debounce(b);
    delay(delayTimeBetweenScans);
  }
  readState();
}

void debounce(byte pass) {
  for (byte b = 0; b < numRows; ++b) {
    pinLow(rows[b]);
    for (byte c = 0; c < numCols; ++c) {
      pressed[pass][b][c] = readPin(cols[c]);
    }
    pinHigh(rows[b]);
  }
}

void pinHigh(byte pin) {
  digitalWrite(pin, HIGH);
}
void pinLow(byte pin) {
  digitalWrite(pin, LOW);
}

bool readPin(byte pin) {
  if (!digitalRead(pin)) {
    return true;
  }
  return false;
}

void readState() {
  for (byte b = 0; b < numRows; b++) {
    for (byte c = 0; c < numCols; ++c) {
      bool isPressed = true;
      for (byte d = 0; d < scanRounds; ++d) {
        isPressed = isPressed && pressed[d][b][c];
      }
      state[b][c] = isPressed;
    }
  }
}


/********
  State
********/

bool stateHasChanged(bool lastState[numRows][numCols], bool state[numRows][numCols]) {
  for (byte b = 0; b < numRows; ++b) {
    for (byte c = 0; c < numCols; ++c) {
        if (lastState[b][c] != state[b][c]) {
          return true;
        }
    }
  }
  return false;
}

void saveState(bool state[numRows][numCols]) {
  for (byte b = 0; b < numRows; ++b) {
    for (byte c = 0; c < numCols; ++c) {
      lastState[b][c] = state[b][c];
    }
  }
}

/*****************
  Build and send
******************/

void buildBufferAndSend(bool state[numRows][numCols]) {
  for (byte b = 0; b < numRows; ++b) {
    for (byte c = 0; c < numCols; ++c) {
      if (state[b][c] == true) {
        addKeyToBuffer(keys[b][c]);
      }
    }
  }
  sendRest();
}

const byte keyLimit = 6;
byte keyPlace = 0;
uint8_t keyBuf[keyLimit];
uint8_t meta = 0x0;
void addKeyToBuffer(uint8_t key) {
  if (isMeta(key)) {
    meta |= metaValue(key);
  } else {
    keyBuf[keyPlace++] = key;
  }

  if (keyPlace == keyLimit) {
    sendBuffer(meta, keyBuf);
  }
}


void sendRest() {
  bool shouldSend = false;
  for (byte b = 0; b < keyLimit; ++b) {
    if (keyBuf[b] != 0x0) {
      shouldSend = true;
    }
  }
  if (shouldSend || stateHasChanged(lastState, state)) {
    sendBuffer(meta, keyBuf);
  }
}

void sendBuffer(uint8_t meta, uint8_t keyBuf[]) {
  sendKeyBuffer(meta, keyBuf);
  resetKeys();
}

void resetKeys() {
  for (byte b = 0; b < keyLimit; ++b) {
    keyBuf[b] = 0x0;
  }
  keyPlace = 0;
  meta = 0x0;
}


bool isMeta(uint8_t key) {
  if (key == Key::L_CTRL
      || key == Key::L_SHFT
      || key == Key::L_ALT
      || key == Key::L_SUPR
      || key == Key::R_CTRL
      || key == Key::R_SHFT
      || key == Key::R_ALT
      || key == Key::R_SUPR
  ) {
    return true;
  }
  return false;
}

uint8_t metaValue(uint8_t key) {
  switch (key) {
    case Key::L_CTRL:
      return Mod::LCTRL;
    case Key::L_SHFT:
      return Mod::LSHFT;
    case Key::L_ALT:
      return Mod::LALT;
    case Key::L_SUPR:
      return Mod::LSUPR;
    case Key::R_CTRL:
      return Mod::RCTRL;
    case Key::R_SHFT:
      return Mod::RSHFT;
    case Key::R_ALT:
      return Mod::RALT;
    case Key::R_SUPR:
      return Mod::RSUPR;
    default:
      return 0;
  }
}

/*****************
  Debugs
******************/

void printState() {
  for (int i = 0; i < numRows; ++i) {
    Serial.println();
    for (int j = 0; j < numCols; ++j) {
      Serial.print(" ");
      Serial.print(state[i][j], DEC);
    }
  }
  Serial.println();
}

void printLastState() {
  for (int i = 0; i < numRows; ++i) {
    Serial.println();
    for (int j = 0; j < numCols; ++j) {
      Serial.print(" ");
      Serial.print(lastState[i][j], DEC);
    }
  }
  Serial.println();
}

void printPressed() {
  for (int i = 0; i < numRows; ++i) {
    Serial.println();
    for (int j = 0; j < numCols; ++j) {
      Serial.print(" ");
      Serial.print(pressed[0][i][j], DEC);
    }
  }
  Serial.println();
}

