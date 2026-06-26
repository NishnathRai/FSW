davunishnath@Davus-MacBook-Air ~ % find / -name avr-g++ 2>/dev/null



/System/Volumes/Data/Users/davunishnath/Library/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++
/System/Volumes/Data/Users/davunishnath/Downloads/avr8-gnu-toolchain-darwin_universal 2/bin/avr-g++
/System/Volumes/Data/Users/davunishnath/Downloads/avr8-gnu-toolchain-darwin_universal/bin/avr-g++
/Users/davunishnath/Library/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++
/Users/davunishnath/Downloads/avr8-gnu-toolchain-darwin_universal 2/bin/avr-g++
/Users/davunishnath/Downloads/avr8-gnu-toolchain-darwin_universal/bin/avr-g++



/System/Volumes/Data/Users/davunishnath/Library/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++ -mmcu=atmega328p -DF_CPU=16000000UL -Os -std=c++17 -c main.cpp -o main.o

/System/Volumes/Data/Users/davunishnath/Library/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++ -mmcu=atmega328p main.o -o main.elf


/System/Volumes/Data/Users/davunishnath/Library/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-objcopy -O ihex -R .eeprom main.elf main.hex

ls /dev/cu.*

 /dev/cu.usbmodem1101 

avrdude     -c arduino -p m328p -P /dev/cu.usbmodem1101  -b 115200   -U flash:w:main.hex:i