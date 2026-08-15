// packect exchange between modules -> Always cmd packet 
// packect exchange between any modules and Uart -> Always tlm packet 
#include "./dataCenter.hpp"

// import modules
#include "../module-blink/blink.hpp"
#include "../module-uart/uard.hpp"
#include "../module-setRealtime/setRealtime.hpp"

//for cmd's
// +------
static CMD_LED cmd_params_from_uart_to_blink;
static Cmd cmd_from_uart_to_blink((unsigned char)MODULE::UART , 0, sizeof(CMD_LED), &cmd_params_from_uart_to_blink);
static DATA_BUCKET cmd_data_bucket_from_uart_to_blink(MODULE::UART, MODULE::BLINK, &cmd_from_uart_to_blink);
// ---
static CMD_SET_REALTIME cmd_params_from_uart_to_set_realtime;
static Cmd cmd_from_uart_to_set_realtime((unsigned char)MODULE::SET_REALTIME, 0, sizeof(CMD_SET_REALTIME), &cmd_data_bucket_from_uart_to_blink);
static DATA_BUCKET cmd_data_bucket_from_uart_to_set_realtime(MODULE::UART, MODULE::SET_REALTIME, &cmd_from_uart_to_set_realtime);
static DATA_BUCKET* cmd_data_center[MAX_CMD_BUKETS_IN_DATA_CENTER] =
// +------
{
    &cmd_data_bucket_from_uart_to_blink,
    &cmd_data_bucket_from_uart_to_set_realtime
};


Cmd* get_cmd_data_bucket_ptr(MODULE src, MODULE dst)
{
    for( int i=0 ;i<MAX_CMD_BUKETS_IN_DATA_CENTER;i++ ){
        if( cmd_data_center[i]->src == src && cmd_data_center[i]->des == dst ){
            return   (Cmd*)(cmd_data_center[i]->data);
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
    else if(MODULE::SET_REALTIME == m_num){
        return (BaseTask*)(&setRealtimeTask::get_instance());
    }
    return nullptr;
}

bool notify_module_about_cmd(Cmd* cmd_pointer){
    BaseTask* m_ptr = get_instance_through_m_code( (MODULE)cmd_pointer->m_code );
    m_ptr->nxt_cmd_queue[ m_ptr->nxt_cmd_queue_cap++ ] = *cmd_pointer;
    return true;
}

//for tlm's
// +------
static LED_TLM tlm_params_from_blink_to_uart;
static Tlm tlm_from_blink_to_uart( ( unsigned char)MODULE::UART, 2,  ( unsigned char)MODULE::BLINK, 0, sizeof(LED_TLM), &tlm_params_from_blink_to_uart );
static DATA_BUCKET tlm_data_bucket_from_blink_to_uart(MODULE::BLINK, MODULE::UART, &tlm_from_blink_to_uart);
// ---
static UART_TLM tlm_params_from_uart_to_uart;
static Tlm tlm_from_uart_to_uart( (unsigned char)MODULE::UART, 2,  ( unsigned char)MODULE::UART, 0, sizeof(UART_TLM), &tlm_params_from_uart_to_uart);
static DATA_BUCKET tlm_data_bucket_from_uart_to_uart(MODULE::UART, MODULE::UART, &tlm_from_uart_to_uart);
// ---
static SET_REALTIME_TLM tlm_params_from_set_realtime_to_uart;
static Tlm tlm_from_set_realtime_to_uart( (unsigned char)MODULE::UART, 2,  ( unsigned char)MODULE::SET_REALTIME, 0, sizeof(SET_REALTIME_TLM), &tlm_params_from_set_realtime_to_uart  );
static DATA_BUCKET tlm_data_bucket_from_set_realtime_to_uart(MODULE::SET_REALTIME, MODULE::UART, &tlm_from_set_realtime_to_uart);
// +------
static DATA_BUCKET* tlm_data_center[MAX_TLM_BUKETS_IN_DATA_CENTER] =
{
    &tlm_data_bucket_from_blink_to_uart,
    &tlm_data_bucket_from_uart_to_uart,
    &tlm_data_bucket_from_set_realtime_to_uart
};

bool notify_module_about_tlm(Tlm *tlm_pointer){
    UartTask* m_ptr = (UartTask*)get_instance_through_m_code( (MODULE)tlm_pointer->m_code );
    m_ptr->nxt_tlm_queue[ m_ptr->nxt_tlm_queue_cap++ ] = *tlm_pointer;
    return true;
}


Tlm* get_tlm_data_bucket_ptr(MODULE src, MODULE dst)
{
    for( int i=0 ;i<MAX_TLM_BUKETS_IN_DATA_CENTER;i++ ){
        if( tlm_data_center[i]->src == src && tlm_data_center[i]->des == dst ){
            return   (Tlm*)(tlm_data_center[i]->data);
        }
    }
    return nullptr;
}

Tlm* get_tlm_by_tlm_id(uint8_t tlm_id){
    return (Tlm*)tlm_data_center[tlm_id]->data;
}

