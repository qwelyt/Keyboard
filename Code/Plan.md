# Plans to get it all to work


## Keys
The keys must be debounced. This is pretty simple. Just scan the same key twice with a short delay in between. `delay(50)` seems to be a good amount.
A simple debounce logic could look like this

```
	byte val1=digitalRead(pin);
	if(!val1){
		delay(50);
		byte val2 = digitalRead(pin);
		if(!val2){
			pressed = true;
		}
	}
```

This will debounce one key. But having a 50ms delay *for each key* seems like frustration galore.
The whole keyboard needs to be scanned. Then 50ms delay. Run the second scan. If a key is registered
as pressed on both scans, it probably really is pressed.
A simple way could be

```
	int pins[numOfKeys] = {2,3....};
	int pressed[numOfKeys] = {0};

	void debounce(){
		int check1[numOfKeys];
		boolean shouldDelay = false; // Speed optimization!
		for(int i=0; i<numOfKeys; ++i){
			check1[i] = digitalRead(pins[i]);
			if(!check1[i]){
				shouldDelay = true;
			}
		}

		if(shouldDelay){
			delay(50);
		}

		int check2[numOfKeys];
		for(int i=0; i<numOfKeys; ++i){
			check2[i] = digitalRead(pins[i]);
		}

		for(int i=0; i<numOfKeys; ++i){
			if(check1[i] + check2[i] == 0){
				pressed[i] = 1;
			}
		}
	}
	
```

This scans all keys and checks which were pressed during both readings.

## Keeping state
As we don't want to hammer the computer with data, we don't want to send anything unless something has
actually  changed. So if the user presses the 'W' key, we don't want to send a buffer saying that 'W' is pressed
every loop.
What we want to do is at during every loop, we look for changes in our matrix. If something has changed since 
our last scan, we send new data to the computer.

So. What needs to be done during every loop is something like this.
```
	void loop(){
		scanMatrix();
		if(stateHasChanged()){
			buildDataFramesAndSend();
			saveNewState();
		}
	}

	void scanMatrix(){
		readKeysPass1();
		delay(50);
		readKeysPass2();
	}

	boolean stateHasChanged(){
		if(savedState != justReadState){
			return true;
		}
		return false;
	}

	void buildDataFramesAndSend(){
		[...]
	}

	void saveNewState(){
		savedState = justReadState;
	}
```
