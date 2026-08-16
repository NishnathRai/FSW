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

// 16 bit timer/counter registers
#define TCCR1A 0x80
#define TCCR1B 0x81
#define TCCR1C 0x82
#define TCNT1H 0x85
#define TCNT1L 0x84
#define OCR1AH 0x89
#define OCR1AL 0x88
#define OCR1BH 0x8B
#define OCR1BL 0x8A
#define ICR1H  0x87
#define ICR1L  0x86
#define TIMSK1 0x6F
#define TIFR1  0x36