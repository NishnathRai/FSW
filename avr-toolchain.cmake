# AVR Toolchain File
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# Specify the toolchain path
set(AVR_TOOLCHAIN_PATH "/Users/davunishnath/Library/Arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin")

# Set compilers
set(CMAKE_C_COMPILER ${AVR_TOOLCHAIN_PATH}/avr-gcc)
set(CMAKE_CXX_COMPILER ${AVR_TOOLCHAIN_PATH}/avr-g++)
set(CMAKE_ASM_COMPILER ${AVR_TOOLCHAIN_PATH}/avr-gcc)

# Never use native compilers
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
