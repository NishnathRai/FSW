#!/bin/bash

set -e

echo "Cleaning build directory..."
rm -rf build
mkdir build

echo "Configuring CMake..."
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=avr-toolchain.cmake

echo "Building..."
cmake --build build

echo "Finding Arduino..."

PORT=$(ls /dev/cu.usbmodem* /dev/cu.usbserial* 2>/dev/null | head -n 1)

if [ -z "$PORT" ]; then
    echo "❌ Arduino not found."
    exit 1
fi

echo "Arduino found at: $PORT"

echo "Flashing..."

avrdude \
    -c arduino \
    -p m328p \
    -P "$PORT" \
    -b 115200 \
    -U flash:w:build/blink.hex:i

echo "✅ Flash complete!"