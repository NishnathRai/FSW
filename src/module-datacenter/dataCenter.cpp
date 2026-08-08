// packect exchange between modules -> Always cmd packet 
// packect exchange between any modules and Uart -> Always tlm packet 
#include "./dataCenter.hpp"
#include "../module-cmd/cmd.hpp"
#include "../module-tlm/tlm.hpp"

// import modules
#include "../module-blink/blink.hpp"
#include "../module-uart/uard.hpp"

static CMD_LED cmd_params_from_uart_to_blink;
static Cmd cmd_from_uart_to_blink(0, 0, sizeof(CMD_LED), &cmd_params_from_uart_to_blink);
static DATA_BUCKET cmd_data_bucket_from_uart_to_blink(MODULE::UART, MODULE::BLINK, &cmd_from_uart_to_blink);

static DATA_BUCKET* data_center[MAX_BUKETS_IN_DATA_CENTER] =
{
    &cmd_data_bucket_from_uart_to_blink
};


Cmd* get_data_bucket_ptr(MODULE src, MODULE dst)
{
    for( int i=0 ;i<MAX_BUKETS_IN_DATA_CENTER;i++ ){
        if( data_center[i]->src == src && data_center[i]->des == dst ){
            return   (Cmd*)(data_center[i]->data);
        }
    }
    return nullptr;
}
BaseTask* get_instance_through_m_code(MODULE m_num){
    if(MODULE::BLINK == m_num){
        return (BaseTask*)(&BlinkTask::get_instance());
    }
    else if(MODULE::UART == m_num){
        return (BaseTask*)(&UartTask::get_instance());
    }
    return nullptr;
}

bool notify_module_about_cmd(Cmd* cmd_pointer){
    BaseTask* m_ptr = get_instance_through_m_code( (MODULE)cmd_pointer->m_code );
    m_ptr->nxt_cmd_queue[ m_ptr->nxt_cmd_queue_cap++ ] = *cmd_pointer;
}


