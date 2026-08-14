#pragma once
#include <stdint.h>
// TODO: cmd param value instead of pointer
// solution TODO: storeing the data at runtime :- we created a data-center 
class Cmd{
public:
    uint16_t size;
    unsigned char m_code;
    unsigned char cmd_code;
    void* param;
    Cmd() = default;
    Cmd(unsigned char m, unsigned char c,uint16_t s,void* p)
        : m_code(m), cmd_code(c), size(s), param(p)
    {
    }
};


// here in this file , well well he having all the cmd packets structure 

class __attribute__((packed)) CMD_LED {
public:
    uint8_t action;  // 0 - OFF , 1 - ON
};

class __attribute__((packed)) CMD_SET_REALTIME{
public:
    uint8_t tlm_id;
    uint8_t action;  // 0 - OFF , 1 - ON    
};
