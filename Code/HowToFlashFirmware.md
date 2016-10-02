# Flash the UNO using dfu-programmer
Short guide for how to flash the Arduino Uno using `dfu-programmer`

## Requriments
	* Arduino-keyboard-0.3.hex
	* Arduino-usbserial-uno.hex
	* dfu-programmer
	* dfu-util
	* Arduino Uno v3 (duh)

## Tell the Uno to enter 'flash-mode'
Create a connection between the pins closest to the USB-connection.
Right besid the USB-port there should be a reset-button. Near that button,
there should be 6 pins. The two pins in the front row are the pins you want
to create a connection between.

### Flash to keyboard
You will probably need root to execute the commands.
```
# dfu-programmer atmega16u2 erase
# dfu-programmer atmega16u2 flash --debug 1 Arduino-keyboard-0.3.hex
# dfu-programmer atmega16u2 reset
```
### Flash to normal UsbSerial
You will probably need root to execute the commands.
```
# dfu-programmer atmega16u2 erase
# dfu-programmer atmega16u2 flash --debug 1 Arduino-usbserial-uno.hex
# dfu-programmer atmega16u2 reset
```

## Check what firmware the Arduino currently has
```
% lsusb
```
If you see `Atmel Corp. LUFA Keyboard Demo Application`, the Uno has the keyboard-0.3.hex.
If you see `Arduino SA Uno (CDC ACM)`, the Uno has the usbserial-uno.hex.
If you see `Atmel Corp. atmega16u2 DFU bootloader`, the Uno is ready to be flashed with new firmware.

## After flashing
After you have flashed the Uno you need to power-cycle it for the new firmware to boot up.
You have to do this even if you haven't flashed it and just put in in 'flash-mode'.
