#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

int main()
{
    // PB5 = Arduino Digital Pin 13 (Built-in LED)
    DDRB |= (1 << PB5);

    while (true)
    {
        PORTB |= (1 << PB5);   // LED ON
        _delay_ms(500);

        PORTB &= ~(1 << PB5);  // LED OFF
        _delay_ms(500);
    }

    return 0;
}