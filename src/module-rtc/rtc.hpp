#pragma once
#include <stdint.h>
#include "../include-base-task/baseTask.hpp"

class RtcTask : public BaseTask{
private:
    // the most important varable `The RTC time`
    uint16_t rtc_time;
    uint16_t rtc_time_s;
    uint16_t rtc_time_ms;
    bool processCmd();
    bool collectTlm();
    bool postTlm();
    bool processTaskRoutine();
public:
    static RtcTask& get_instance();
    RtcTask(){};
    bool initTask();
    uint16_t get_rtc_time();
    uint16_t get_rtc_time_s();
    uint16_t get_rtc_time_ms();
    bool task()
    {
        return processCmd() &&
               collectTlm() &&
               postTlm() &&
               processTaskRoutine();
    }    
};