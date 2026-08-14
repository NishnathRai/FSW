#pragma once
#include "../include-base-task/baseTask.hpp"
#include "../module-datacenter/dataCenter.hpp"
#include <stdint.h>

class setRealtimeTask : public BaseTask{
private:
    static constexpr int MAX_CMDS = 1;
    using cmdfun_t = bool(setRealtimeTask::*)(void*); 
    bool processCmd(){
        bool return_value = 1 ;
        for(uint16_t i=0;i<this->nxt_cmd_queue_cap;i++){
            Cmd& nxt_cmd = this->nxt_cmd_queue[i];
            cmdfun_t fun = this->table[ nxt_cmd.cmd_code ];
            return_value &= (this->*fun)(&nxt_cmd);
        }
        return true;
    }
    bool collectTlm();
    bool postTlm() ;
    bool processTaskRoutine() ;
public:
    bool initTask();
    static setRealtimeTask& get_instance();
    cmdfun_t table[MAX_CMDS] =
    {
        &setRealtimeTask::setRealtime_cmd
    };
    // cmds implementation's
    bool setRealtime_cmd(void* cmd);   
};