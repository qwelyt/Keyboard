sudo dfu-programmer atmega16u2 erase
echo "press enter to continue"
read cont
sudo dfu-programmer atmega16u2 flash --debug 1 $1
echo "press enter to continue"
read cont
sudo dfu-programmer atmega16u2 reset
