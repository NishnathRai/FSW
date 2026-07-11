#include <avr/interrupt.h>

#define PORTB  0x25
#define DDRB   0x24

#define TCCR0A 0x44 
#define TCCR0B 0x45 
#define TIMSK0 0x6E
#define TIFR0  0x35 

#define SREG  0x5F


volatile int times = 0;
ISR(TIMER0_OVF_vect) {
    volatile unsigned char* portb_pointer = (volatile unsigned char*)PORTB;
    times++;
    if(times == 61){
        times = 0;
        *portb_pointer = ~(*portb_pointer);
    }
    return ;
}

void start_delay_1_sec_for_led(){
    volatile unsigned char* ttcr0a_pointer = (volatile unsigned char*)TCCR0A;
    volatile unsigned char* ttcr0b_pointer = (volatile unsigned char*)TCCR0B;
    volatile unsigned char* timsk0_pointer = (volatile unsigned char*)TIMSK0;
   

    *ttcr0a_pointer = 0x00;
    *ttcr0b_pointer = 0b00000101;
    *timsk0_pointer = 0b00000001; 
    return ;
}


int main(){   
    //set direction of portb to output
    volatile unsigned char* ddrb_pointer = (volatile unsigned char*)DDRB;
    *ddrb_pointer = 0xFF;
    //enable global interrupt
    // sei();
    unsigned char* sreg_pointer = (unsigned char*)SREG;
    *sreg_pointer = *sreg_pointer | 0b10000000; 


    /// set portb to high
    volatile unsigned char* portb_pointer = (volatile unsigned char*)PORTB;
    *portb_pointer = 0xFF;
   
    //start timer0 to generate interrupt every 16.384 ms
    start_delay_1_sec_for_led();
    while(1){}
    return 0;
}
