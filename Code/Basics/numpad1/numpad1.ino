// Numpad 1
// Only send data when the
// data frame changes.

#define numRows 3
#define numCols 3
int rows[] = {4,5,6};
int cols[] = {10,11,12};

uint8_t lastFrame[8] = {0};

uint8_t key[numRows][numCols] = {
	{0x59,0x5A,0x5B},
	{0x5C,0x5D,0x5E},
	{0x5F,0x60,0x61}
};

int pressed[2][numRows][numCols] = {
	{
		{0,0,0},
		{0,0,0},
		{0,0,0}
	},
	{
		{0,0,0},
		{0,0,0},
		{0,0,0}
	}
};

int lastState[numRows][numCols] = {0};
int state[numRows][numCols] = {0};

/////////////

void setup() {
	Serial.begin(9600);
	for(int i=0; i<numRows; ++i){
		setupRow(rows[i]);
	}
	for(int i=0; i<numCols; ++i){
		setupCol(cols[i]);
	}

}

void setupRow(int pin){
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
}

void setupCol(int pin){
	pinMode(pin, INPUT_PULLUP);
}

/////////////

void loop() {
	scan();
	//printState();
	if(stateHasChanged(lastState, state)){
		buildBufferAndSend(state);
	}
	saveState(state);
	//delay(1000);
}

/*********
*  Scan  *
*********/
void scan(){
	debounce(0);
	delay(50);
	debounce(1);
	readState();
}

void debounce(int pass){
	for(int i=0; i<numRows; ++i){
		pinLow(rows[i]);
		for(int j=0; j<numCols; ++j){
			pressed[pass][i][j] = readPin(cols[j]);
		}
		pinHigh(rows[i]);
	}
}

void pinHigh(int pin){
	digitalWrite(pin, HIGH);
}

void pinLow(int pin){
	digitalWrite(pin, LOW);
}

int readPin(int pin){
	int val = digitalRead(pin);
	if(!val){
		return 1;
	}
	return 0;
}

void readState(){
	for(int i=0; i<numRows; ++i){
		for(int j=0; j<numCols; ++j){
			if(pressed[0][i][j] + pressed[1][i][j] == 2){
				state[i][j] = 1;
			} else {
				state[i][j] = 0;
			}
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


/****************
*  State check  *
****************/

boolean stateHasChanged(int lastState[numRows][numCols], int state[numRows][numCols]){
	for(int i=0; i<numRows; ++i){
		for(int j=0; j<numCols; ++j){
			if(lastState[i][j] != state[i][j]){
				return true;
			}
		}
	}
	return false;
}

/********************************
*  Build, send, and save state  *
********************************/

void buildBufferAndSend(int state[numRows][numCols]){
	for(int i=0; i<numRows; ++i){
		for(int j=0; j<numCols; ++j){
			if(state[i][j] == 1){
				addKeyToBuffer(key[i][j]);
			}
		}
	}
	sendRest();
}


const int keyLimit = 6;
int keyPlace=0;
uint8_t keys[keyLimit];
uint8_t meta = 0x0;
void addKeyToBuffer(uint8_t key){
	if(isMeta(key)){
		meta|= key;
	} else {
		keys[keyPlace++]=key;
	}

	if(keyPlace == keyLimit){
		uint8_t buf[8];
		buildBuffer(meta, keys, buf);
		send(buf);
		resetKeys();
	}
}

void sendRest(){
	boolean shouldSend = false;
	for(int i=0; i<keyLimit; ++i){
		if(keys[i] != 0x0){
			shouldSend = true;
		}
	}
	if(shouldSend || stateHasChanged(lastState, state)){
		uint8_t buf[8];
		buildBuffer(meta, keys, buf);
		send(buf);
		resetKeys();
	}
}

void resetKeys(){
	for(int i=0; i<keyLimit; ++i){
		keys[i] = 0x0;
	}
	keyPlace=0;
}

boolean isMeta(uint8_t key){
	return false;
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

void saveState(int state[numRows][numCols]){
	for(int i=0; i<numRows; ++i){
		for(int j=0; j<numCols; ++j){
			lastState[i][j] = state[i][j];
		}
	}
}
