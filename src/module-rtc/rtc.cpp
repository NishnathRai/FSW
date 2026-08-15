#include "../module-rtc/rtc.hpp"
#include "../include-reg/reg.hpp"

volatile uint8_t* tcnt1h_ptr = (uint8_t*)TCNT1H;
volatile uint8_t* tcnt1l_ptr = (uint8_t*)TCNT1L;
volatile uint8_t* tccr1b_ptr = (uint8_t*)TCCR1B;

RtcTask RtcTask_box;
RtcTask& RtcTask::get_instance(){
    return RtcTask_box; 
}
bool RtcTask::initTask(){
    this->rtc_time    = 0;
    this->rtc_time_s  = 0;
    this->rtc_time_ms = 0;

    *tccr1b_ptr = 0b00000100;

    return true;
}
uint16_t RtcTask::get_rtc_time(){
    return this->rtc_time;
}
uint16_t RtcTask::get_rtc_time_s(){
    return this->rtc_time_s;
}
uint16_t RtcTask::get_rtc_time_ms(){
    return this->rtc_time_ms;
}
bool RtcTask::processCmd(){
    return true;
};
bool RtcTask::collectTlm(){
    return true;
};
bool RtcTask::postTlm(){
    return true;
};
// 31,250 in hexadecimal is 0x7A12.Breakdown
bool RtcTask::processTaskRoutine(){
    this->rtc_time = ( *tcnt1h_ptr << 8 ) | ( *tcnt1l_ptr );
    if( this->rtc_time >=31250  ){
        //we completed 500 ms
        *tcnt1h_ptr = 0;
        *tcnt1l_ptr = 0;
        this->rtc_time_ms = (this->rtc_time_ms +1 ) % 65536 ;
        if( (this->rtc_time_ms & 1 )== 0 ){
            this->rtc_time_s = (this->rtc_time_s +1 ) % 65536 ;
        }
    }
    return true;
};