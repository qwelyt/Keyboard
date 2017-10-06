#include "Keyboard.h"
#include "Key.h"

/** Order of things
 * 	scan for pressed buttons
 * save scan result in 'state'
 * compare new state with last state
 *  if(state has changed) {
 *		check layer
 *		translate to scan codes
 *		build
 *		send
 *		save state
 *	}
 */

byte numCols = 7;
byte numRows = 5;
byte layers = 3; // Normal, raise, lower
byte scanRounds = 2;

int blockPin  = A0;
int delayTimeInMSBetweenScans = 10;

// Setup matrix. Map the pins.
int cols[numCols] = {7,8,9,10,11,12,13};
int rows[numRows] = {2,3,4,5,6};
typedef struct {
	byte row;
	byte col;
} ModPosition;

ModPosition lower;
lower.row = 5;
lower.col = 3;

ModPosition raise;
raise.row = 5;
raise.col = 4;

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
		{Key::NONE      , Key::NONE   , Key::MUTE  , Key::VOLDN , Key::VOLUP , Key::NONE , Key::NONE}
		, {Key::TAB     , Key::NONE   , Key::PLAY  , Key::PREV  , Key::NEXT  , Key::NONE , Key::NONE}
		, {Key::ESC     , Key::NONE   , Key::NONE  , Key::NONE  , Key::NONE  , Key::NONE , Key::NONE}
		, {Key::L_SHFT  , Key::NONE   , Key::NONE  , Key::NONE  , Key::NONE  , Key::NONE , Key::NONE}
		, {Key::L_CTRL  , Key::L_SUPR , MOD::LOWER , MOD::RAISE , Key::L_ALT , Key::NONE , Key::NONE}	
	}
};

bool pressed[scanRounds][numRows][numCols] = {};
bool lastState[numRows][numCols] = {false};
bool state[numRows][numCols] = {false};
bool blocked = false;

///////////////////////////

void setup() {
	blocked = analogRead(blockPin) < 100;
	
	for(byte b=0; b < numRows; ++b){
		setupRow(rows[b]);
	}
	
	for(byte b=0; b<numCols; ++b){
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


void loop(){
	if(!blocked){
		scan();
		if(stateHasChanged(lastState, state)){
			translate(state);
			buildAndSend(translation);
			save(state);
		}
	}
}