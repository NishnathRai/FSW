#pragma once
#include "../include-base-task/baseTask.hpp"


class BlinkTask : public BaseTask
{
private:
    using CmdFn = bool (BlinkTask::*)(void*);
    static constexpr int MAX_CMDS = 1;
    bool processCmd() override
    {
        bool final = 1 ;
        for(unsigned int i=0;i< this->nxt_cmd_queue_cap ;i++){
            Cmd& nxt_cmd = nxt_cmd_queue[i];
            if (nxt_cmd.cmd_code >= MAX_CMDS)
            return false;
            CmdFn temp_fun = table[nxt_cmd.cmd_code];
            final &= (this->*temp_fun)(&nxt_cmd);
        }
        this->nxt_cmd_queue_cap = 0;
        return final;
    }
    bool collectTlm();
    bool postTlm() ;
    bool processTaskRoutine() ;

    CmdFn table[MAX_CMDS] =
    {
        &BlinkTask::actionBlink_cmd
    };
    // cmds implementation's
    bool actionBlink_cmd(void*);
public:
    static BlinkTask& get_instance();
    bool   initTask();
};
