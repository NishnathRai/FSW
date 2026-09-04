#pragma once
#include "../include-base-task/baseTask.hpp"
#include <stdint.h>
#include "i2c_config.hpp"

class I2cTask : public BaseTask
{
private:
    bool processCmd();
    bool collectTlm();
    bool postTlm();
    bool processTaskRoutine();
    //helper functions
    uint8_t i2c_8_bit_data_reg [ SIZE_i2c_8_bit_slaves ][ SIZE_i2c_8_bit_data_reg+1 ] = 
    { 
        {0x68,
        0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44,
        0x45, 0x46, 0x47, 0x48, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} 
    }; 
public:
    bool initTask();
    static I2cTask& get_instance();
    /// helper functions
    uint8_t get_slave_data(uint8_t slave_add,uint8_t slave_reg);
};