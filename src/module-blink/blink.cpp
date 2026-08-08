#include "blink.hpp"
#include <avr/io.h>
#include <util/delay.h>
static BlinkTask blink_singleton;

BlinkTask& BlinkTask::get_instance(){
    return blink_singleton;
}
bool BlinkTask::collectTlm(){
    return true;
}

bool BlinkTask::postTlm() {
    return true;
}

bool BlinkTask::processTaskRoutine() {
    return true;
}
bool BlinkTask::initTask(){
    // PB5 = Arduino Digital Pin 13 (Built-in LED)
    DDRB |= (1 << PB5);
    return true;
}

bool BlinkTask::actionBlink_cmd( void* params ){
    Cmd* cmd_ptr = (Cmd*)params;
    uint8_t* temp_params = (uint8_t*)cmd_ptr->param;
    if( *temp_params == 1 ){
        PORTB |= (1 << PB5);   // LED ON
    }
    else if( *temp_params == 0 ){
        PORTB &= ~(1 << PB5);  // LED OFF
    }
    return true;
}

