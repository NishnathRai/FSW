#include "blink.hpp"
#include <avr/io.h>
#include <util/delay.h>

static Blink blink_singleton;

Blink& Blink::get_blink_instence(){
    return blink_singleton;
}

bool Blink::blink( ACTION_to_BLINK action ){
    // PB5 = Arduino Digital Pin 13 (Built-in LED)
    DDRB |= (1 << PB5);

    while (true)
    {
        PORTB |= (1 << PB5);   // LED ON
        _delay_ms(500);

        PORTB &= ~(1 << PB5);  // LED OFF
        _delay_ms(500);
    }

}