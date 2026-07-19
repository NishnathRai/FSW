#pragma once
#include <stdint.h>
// TODO: cmd param value instead of pointer
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