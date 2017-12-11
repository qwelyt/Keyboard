#include "Keyboard.h"
#include "Key.h"

/** Order of things
  	scan for pressed buttons
   save scan result in 'state'
   compare new state with last state
    if(state has changed) {
 		   check layer
 		   translate to scan codes (just check the layer, really)
 		   build
 		   send
 		   save state
 	  }
*/

const byte numCols = 7;
const byte numRows = 5;
const byte layers = 3; // Normal, raise, lower
const byte scanRounds = 2;

int blockPin  = A0;
int delayTimeInMSBetweenScans = 10;

// Setup matrix. Map the pins.
int cols[numCols] = {7, 8, 9, 10, 11, 12, 13};
int rows[numRows] = {2, 3, 4, 5, 6};

struct ModPosition{
  byte row;
  byte col;
  byte val;
};

const ModPosition lower = {4,2,2};
const ModPosition raise = {4,3,1};

uint8_t keys[layers][numRows][numCols] = {
  { // Normal (0)
    {Key::NONE      , Key::NONE      , Key::K1    , Key::K2    , Key::K3        , Key::K4    , Key::K5}
    , {Key::TAB     , Key::NONE      , Key::Q     , Key::W     , Key::E         , Key::R     , Key::T}
    , {Key::ESC     , Key::NONE      , Key::A     , Key::S     , Key::D         , Key::F     , Key::G}
    , {Key::L_SHFT  , Key::BS_N_PIPE , Key::Z     , Key::X     , Key::C         , Key::V     , Key::B}
    , {Key::L_CTRL  , Key::L_SUPR    , MOD::LOWER , MOD::RAISE , Key::L_ALT     , Key::NONE  , Key::SPACE}
  }
  , { // Raise (1)
    {Key::NONE      , Key::NONE   , Key::F1    , Key::F2    , Key::F3    , Key::F4   , Key::F5}
    , {Key::TAB     , Key::NONE   , Key::NONE  , Key::NONE  , Key::NONE  , Key::NONE , Key::NONE}
    , {Key::ESC     , Key::NONE   , Key::NONE  , Key::NONE  , Key::NONE  , Key::NONE , Key::NONE}
    , {Key::L_SHFT  , Key::NONE   , Key::NONE  , Key::NONE  , Key::NONE  , Key::NONE , Key::NONE}
    , {Key::L_CTRL  , Key::L_SUPR , MOD::LOWER , MOD::RAISE , Key::L_ALT , Key::NONE , Key::NONE}
  }
  , { // Lower(2)
    {Key::NONE      , Key::NONE   , Key::GACC  , Key::NONE , Key::NONE , Key::NONE , Key::NONE}
    , {Key::TAB     , Key::NONE   , Key::MUTE  , Key::VOL_UP   , Key::VOL_DN   , Key::NONE , Key::NONE}
    , {Key::ESC     , Key::NONE   , Key::PLAY  , Key::PREV   , Key::NEXT   , Key::NONE , Key::NONE}
    , {Key::L_SHFT  , Key::NONE   , Key::NONE  , Key::NONE   , Key::NONE   , Key::NONE , Key::NONE}
    , {Key::L_CTRL  , Key::L_SUPR , MOD::LOWER , MOD::RAISE  , Key::L_ALT  , Key::NONE , Key::NONE}
  }
};

///////////////////////////
bool pressed[scanRounds][numRows][numCols] = {};
bool lastState[numRows][numCols] = {false};
bool state[numRows][numCols] = {false};
bool blocked = false;
uint8_t codes[numRows][numCols] = {Key::NONE};

///////////////////////////

void setup() {
  blocked = analogRead(blockPin) < 100;

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

///////////////////////////


void loop() {
  if (!blocked) {
    scan();
    if (stateHasChanged(lastState, state)) {
      buildAndSend(state);
      save(state);
    }
  }
}

/*******
  Scan
*******/
void scan() {
  for (byte b = 0; b < scanRounds; ++b) {
    debounce(b);
    delay(delayTimeInMSBetweenScans);
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

void save(bool state[numRows][numCols]) {
  for (byte b = 0; b < numRows; ++b) {
    for (byte c = 0; c < numCols; ++c) {
      lastState[b][c] = state[b][c];
    }
  }
}

/*****************
  Build and send
******************/

void buildAndSend(bool state[numRows][numCols]) {
  byte l = layer(state);
    
  for (byte b = 0; b < numRows; ++b) {
    for (byte c = 0; c < numCols; ++c) {
      if (state[b][c] == true) {
        addKeyToBuffer(keys[l][b][c]);
      }
    }
  }
  sendRest();
}

byte layer(bool state[numRows][numCols]) {
  if (state[lower.row][lower.col] == true) {
    return lower.val;
  } else if (state[raise.row][raise.col] == true) {
    return raise.val;
  } else {
    return 0;
  }
}

const byte keyLimit = 6;
byte keyPlace = 0;
uint8_t keyBuf[keyLimit];
uint8_t meta = 0x0;
void addKeyToBuffer(uint8_t key) {
  if (isKeyWithValue(key)) {
    if (isMeta(key)) {
      meta |= metaValue(key);
    } else {
      keyBuf[keyPlace++] = key;
    }

    if (keyPlace == keyLimit) {
      sendBuffer(meta, keyBuf);
    }
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

bool isKeyWithValue(uint8_t key) {
  return key != Key::NONE;
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
  Serial.print(" ");
  for (int j = 0; j < numCols; ++j) {
      Serial.print(" ");
      Serial.print(j);
  }
  for (int i = 0; i < numRows; ++i) {
    Serial.println();
    Serial.print(i);
    for (int j = 0; j < numCols; ++j) {
      Serial.print(" ");
      Serial.print(state[i][j], DEC);
    }
  }
  Serial.println();
}

void printLayer(){
  Serial.print("Lower: ");
  printPos(lower.row, lower.col);
  Serial.print(" || Raise: ");
  printPos(raise.row, raise.col);
  Serial.println();
}

void printPos(byte r, byte c){
  Serial.print(state[r][c], DEC);
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
