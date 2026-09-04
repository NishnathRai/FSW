#pragma once
// we do not have the power to use the new key word here. So , this file is the solution for us 

// here this file creates the static varables at the compile time using the static key , we use this varibale to exchance the data between two modules 
// at a time only one packet can wxchange between two modules
#include <stdint.h>
#include "dataCenter_config.hpp"
#include "../module-cmd/cmd.hpp"
#include "../module-tlm/tlm.hpp"
#include "../include-base-task/baseTask.hpp"

enum class MODULE{
    BLINK,
    UART,
    SET_REALTIME,
    RTC,
    I2C
};



// MODULE-SOURCE MODULE-DESTINATION PACKET(void*) 
class DATA_BUCKET{
public:
    MODULE src;
    MODULE des;
    void* data;
    DATA_BUCKET(MODULE s, MODULE d, void* da)
        : src(s), des(d), data(da)
    {}
};

Cmd* get_cmd_data_bucket_ptr(MODULE src, MODULE dst);
Tlm* get_tlm_data_bucket_ptr(MODULE src, MODULE dst);
Tlm* get_tlm_by_tlm_id(uint8_t tlm_id);
BaseTask* get_instance_through_m_code(MODULE m_num);
bool notify_module_about_cmd(Cmd* cmd_pointer);
bool notify_module_about_tlm(Tlm *tlm_pointer);
