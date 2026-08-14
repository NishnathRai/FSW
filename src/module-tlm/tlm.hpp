#pragma once
// here in this file we have all the tlm packets structure
#include <stdint.h>
#include "../module-datacenter/dataCenter_config.hpp"

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


class  __attribute__((packed))  LED_TLM{
public:
    uint8_t action;  // 0 - OFF , 1 - ON
};
class  __attribute__((packed))  UART_TLM{
public:
    uint8_t working;
};
class  __attribute__((packed)) SET_REALTIME_TLM{
public:
    uint8_t action_set_real_time[MAX_TLM_BUKETS_IN_DATA_CENTER];
};