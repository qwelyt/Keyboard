// ModuleA
//
// Layout:
//  col(pin)|1(6)  |2(7)  |3(8)  |4(9)  |5(10)  |6(11)  |7(12)
//  row(pin)+------+------+------+------+-------+-------+-----
//   1(5)   |      |      |F1    |F2    |F3     |F4     |F5
//   2(4)   |      |      |1     |2     |3      |4      |5 
//   3(3)   |Tab   |      |Q     |W     |E      |R      |T
//   4(2)   |ESC   |      |A     |S     |D      |F      |G
//   5(1)   |Shft  |<>|   |Z     |X     |C      |V      |B
//   6(0)   |Ctrl  |Meta  |Alt   |Mnu   |PS     |       |Space
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

#include "Key.h"

#define numCols 7
#define numRows 6
#define scanRounds 2

int cols[] = {6,7,8,9,10,11,12};
int rows[] = {0,1,2,3,4,5};

uint8_t lastFrame[8] = {0};

uint8_t keys[numRows][numCols] = {
    {Key::NONE      ,Key::NONE      ,Key::F1    ,Key::F2    ,Key::F3        ,Key::F4    ,Key::F5}
    ,{Key::NONE     ,Key::NONE      ,Key::K1    ,Key::K2    ,Key::K3        ,Key::K4    ,Key::K5}
    ,{Key::TAB      ,Key::NONE      ,Key::Q     ,Key::W     ,Key::E         ,Key::R     ,Key::T}
    ,{Key::ESC      ,Key::NONE      ,Key::A     ,Key::S     ,Key::D         ,Key::F     ,Key::G}
    ,{Key::L_SHFT    ,Key::BS_N_PIPE ,Key::Z     ,Key::X     ,Key::C         ,Key::V     ,Key::B}
    ,{Key::L_CTRL    ,Key::L_SUPR      ,Key::L_ALT  ,Key::MENU  ,Key::PRNT_SCRN ,Key::NONE  ,Key::SPACE}
};

bool pressed[scanRounds][numRows][numCols] = {};

bool lastState[numRows][numCols] = {false};
bool state[numRows][numCols] = {false};

///////////////

void setup(){
    Serial.begin(9600);
    for(byte b=0; b<numRows; ++b){
        setupRow(rows[b]);
    }
    for(byte b=0; b<numCols; ++b){
        setupCol(cols[b]);
    }
}

void setupRow(byte pin){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void setupCol(byte pin){
    pinMode(pin, INPUT_PULLUP);
}

///////////////

void loop(){
    scan();
    if(stateHasChanged(lastState, state)){
        buildBufferAndSend(state);
    }
    saveState(state);
}

/*******
* Scan *
*******/
void scan(){
    for(byte b=0; b<scanRounds; ++b){
        debounce(b);
        delay(50);
    }
    readState();
}

void debounce(byte pass){
    for(byte b=0; b<numRows; ++b){
        pinLow(rows[b]);
        for(byte c=0; c<numCols; ++c){
            pressed[pass][b][c] = readPin(cols[c]);
        }
        pinHigh(rows[b]);
    }
}

void pinHigh(byte pin){
    digitalWrite(pin, HIGH);
}
void pinLow(byte pin){
    digitalWrite(pin, LOW);
}

bool readPin(byte pin){
    if(!digitalRead(pin)){
        return true;
    }
    return false;
}

void readState(){
    for(byte b=0; b<numRows; b++){
        for(byte c=0; c<numCols; ++c){
            bool isPressed = true;
            for(byte d=0; d<scanRounds; ++d){
                isPressed = isPressed && pressed[d][b][c];
            }
            state[b][c] = isPressed;
        }
    }
}

void printState(){
  for(int i=0; i<numRows; ++i){
    Serial.println();
    for(int j=0; j<numCols; ++j){
      Serial.print(" ");
      Serial.print(state[i][j], DEC);
    }
  }
  Serial.println(); 
}

/********
* State *
********/

bool stateHasChanged(bool lastState[numRows][numCols], bool state[numRows][numCols]){
    for(byte b=0; b<numRows; ++b){
        for(byte c=0; c<numCols; ++c){
            if(state[b][c] == true){
                if(lastState[b][c] != state[b][c]){
                    return true;
                }
            }
        }
    }
    return false;
}

void saveState(bool state[numRows][numCols]){
    for(byte b=0; b<numRows; ++b){
        for(byte c=0; c<numCols; ++c){
            lastState[b][c] = state[b][c];
        }
    }
}

/*****************
* Build and send *
******************/

void buildBufferAndSend(bool state[numRows][numCols]){
    for(byte b=0; b<numRows; ++b){
        for(byte c=0; c<numCols; ++c){
            if(state[b][c] == true){
                addKeyToBuffer(keys[b][c]);
            }
        }
    }
    sendRest();
}

const byte keyLimit = 6;
byte keyPlace=0;
uint8_t keyBuf[keyLimit];
uint8_t meta = 0x0;
void addKeyToBuffer(uint8_t key){
    if(isMeta(key)){
        meta |= metaValue(key);
    } else {
        keyBuf[keyPlace++] = key;
    }

    if(keyPlace == keyLimit){
        sendBuffer(meta, keyBuf);
    }
}


void sendRest(){
    bool shouldSend = false;
    for(byte b=0; b<keyLimit; ++b){
        if(keyBuf[b] != 0x0){
            shouldSend = true;
        }
    }
    if(shouldSend || stateHasChanged(lastState, state)){
        sendBuffer(meta, keyBuf);
    }
}

void sendBuffer(uint8_t meta, uint8_t keyBuf[]){
    uint8_t buf[8];
    buildBuffer(meta, keyBuf, buf);
    send(buf);
    resetKeys();
}

void resetKeys(){
    for(byte b=0; b<keyLimit; ++b){
        keyBuf[b] = 0x0;
    }
    keyPlace=0;
}


bool isMeta(uint8_t key){
    if(0xDF < key < 0xE8){ // Metas go from E0 to E7. Check Keys.h
        return true;
    }
    return false;
}

uint8_t metaValue(uint8_t key){
    switch(key){
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

void buildBuffer(uint8_t meta, uint8_t keys[], uint8_t buf[]){
    buf[0] = meta;
    buf[1] = 0x0; // Reserved
	buf[2] = keys[0];
	buf[3] = keys[1];
	buf[4] = keys[2];
	buf[5] = keys[3];
	buf[6] = keys[4];
	buf[7] = keys[5];
}

void send(uint8_t buf[]){
    Serial.write(buf, 8);
}
