#pragma once
// here in this file , well well he having all the tlm packets structure 
#include <stdint.h>


class Tlm{
public:
    uint16_t size;
    unsigned char m_code;
    unsigned char cmd_code;
    unsigned char sm_code;
    unsigned char scmd_code;
    void* param;
    Tlm() = default;
    Tlm(unsigned char m, unsigned char c,unsigned char sm, unsigned char sc,uint16_t s,void* p)
        : m_code(m), cmd_code(c), size(s), param(p), sm_code(sm) ,scmd_code(sc)
    {
    }
};


class LED_TLM{
public:
    uint8_t action;  // 0 - OFF , 1 - ON
};