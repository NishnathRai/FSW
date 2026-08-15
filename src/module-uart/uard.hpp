#pragma once

#include "../include-base-task/baseTask.hpp"
#include "../module-tlm/tlm.hpp"
#define uart_start_stop_byte 0x7E

class UartTask : public BaseTask
{
private:
    using CmdFn = bool (UartTask::*)(void*);
    static constexpr int MAX_CMDS = 3;
    bool processCmd() override
    {
        bool final = 1 ;
        for(unsigned int i=0;i< this->nxt_cmd_queue_cap ;i++){
            Cmd& nxt_cmd = this->nxt_cmd_queue[i];
            if (nxt_cmd.cmd_code >= MAX_CMDS)
            {continue;}
            // return false;
            CmdFn temp_fun = this->table[nxt_cmd.cmd_code];
            final &= (this->*temp_fun)(&nxt_cmd);
        }
        this->nxt_cmd_queue_cap = 0;
        return final && processTlm();
    }
    // only for uartTask
    bool processTlm(){
        bool final = 1;
        for(unsigned int i=0;i< this->nxt_tlm_queue_cap; i++){
            Tlm& nxt_tlm = this->nxt_tlm_queue[i];
            if(nxt_tlm.cmd_code != 2)
            {continue;}
            // return fasle;
            CmdFn temp_fun = this->table[2];
            final &= (this->*temp_fun)(&nxt_tlm);
        }
        this->nxt_tlm_queue_cap = 0;
        return true;
    }
    bool collectTlm();
    bool postTlm() ;
    bool processTaskRoutine() ;

    CmdFn table[MAX_CMDS] =
    {
        &UartTask::stopUart_cmd,  // 0
        &UartTask::startUart_cmd, // 1
        &UartTask::sendData_cmd   // 2
    };
    // cmds implementation's
    bool stopUart_cmd(void*);
    bool startUart_cmd(void*);
    bool sendData_cmd(void*);
public:
    Tlm nxt_tlm_queue[MAX_QUEUE_CMDS] ; 
    unsigned char nxt_tlm_queue_cap = 0;
    static UartTask& get_instance();
    bool initTask();
};

