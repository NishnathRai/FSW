#pragma once
// we do not have the power to use the new key word here. So , this file is the solution for us 

// here this file creates the static varables at the compile time using the static key , we use this varibale to exchance the data between two modules 
// at a time only one packet can wxchange between two modules
#include <stdint.h>
#include "../module-cmd/cmd.hpp"
#include "../include-base-task/baseTask.hpp"

constexpr uint8_t MAX_CMD_BUKETS_IN_DATA_CENTER = 1;
constexpr uint8_t MAX_TLM_BUKETS_IN_DATA_CENTER = 1;

enum class MODULE{
    BLINK,
    UART
};

enum class DATA_CENTER_INDEX{
    cmd_data_bucket_from_uart_to_blink
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
BaseTask* get_instance_through_m_code(MODULE m_num);
bool notify_module_about_cmd(Cmd* cmd_pointer);
bool notify_module_about_tlm(Tlm *tlm_pointer);
