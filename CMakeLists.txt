cmake_minimum_required(VERSION 3.16)

project(Blink C CXX)

set(MCU atmega328p)
set(F_CPU 16000000UL)

add_executable(blink.elf 
main.cpp
src/module-blink/blink.cpp
src/module-i2c/i2c.cpp
src/module-uart/uart.cpp
# src/module-cmd/cmd.cpp
src/module-datacenter/dataCenter.cpp
src/module-setRealtime/setRealtime.cpp
src/module-rtc/rtc.cpp
)

target_compile_options(blink.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -std=c++17
)

target_link_options(blink.elf PRIVATE
    -mmcu=${MCU}
)

add_custom_command(TARGET blink.elf POST_BUILD
    COMMAND /Users/davunishnath/Library/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-objcopy -O ihex -R .eeprom
            blink.elf
            blink.hex
)