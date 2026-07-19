// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
#pragma once

// usart registers
#define UDR0   0xC6
#define UBRR0H 0xC5
#define UBRR0L 0xC4
#define UCSR0A 0xC0
#define UCSR0B 0xC1
#define UCSR0C 0xC2

//  i2c registers
#define TWBR    0xB8
#define TWCR    0xBC
#define TWSR    0xB9
#define TWDR    0xBB
#define TWAR    0xBA
#define TWAMR   0xBD