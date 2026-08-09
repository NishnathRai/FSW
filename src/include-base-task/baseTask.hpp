//  here, i want to say define how a taks should propagate 
//  0, taskinit             init 
//  1, processCmd           take from cmd-module
//  2, collectTlm           from imu,regesters etc..
//  3, postTlm              to data center
//  4, processTaksRotine    the main rotine taks of that module
#pragma once
#include "../module-cmd/cmd.hpp"
#include "../module-tlm/tlm.hpp"


class BaseTask
{
private:
    static BaseTask* myTask_obj;
    virtual bool processCmd() = 0;
    virtual bool collectTlm() = 0;
    virtual bool postTlm() = 0;
    virtual bool processTaskRoutine() = 0;
public:
    static constexpr int MAX_QUEUE_CMDS = 15;
    BaseTask(){};
    virtual bool initTask() = 0;
    unsigned char nxt_cmd_queue_cap = 0; 
    Cmd nxt_cmd_queue[MAX_QUEUE_CMDS] ; 
    bool task()
    {
        return processCmd() &&
               collectTlm() &&
               postTlm() &&
               processTaskRoutine();
    }
};