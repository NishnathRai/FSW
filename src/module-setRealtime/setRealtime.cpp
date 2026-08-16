#include "./setRealtime.hpp"
#include "../module-cmd/cmd.hpp"
#include "../module-tlm/tlm.hpp"
#include "../module-rtc/rtc.hpp"

setRealtimeTask setRealtimeTask_box;

setRealtimeTask& setRealtimeTask::get_instance(){
    return setRealtimeTask_box;
}

bool setRealtimeTask::setRealtime_cmd(void* cmd){
    Cmd* cmd_obj = (Cmd*)cmd;
    CMD_SET_REALTIME* cmd_set_realtime = (CMD_SET_REALTIME*)(cmd_obj->param);
    if( cmd_set_realtime->tlm_id >= MAX_TLM_BUKETS_IN_DATA_CENTER  ){
        return false;
    }
    Tlm* tlm_from_set_realtime_to_uart = get_tlm_data_bucket_ptr( MODULE::SET_REALTIME, MODULE::UART );
    SET_REALTIME_TLM* param = (SET_REALTIME_TLM*)tlm_from_set_realtime_to_uart->param;
    param->action_set_real_time[ cmd_set_realtime->tlm_id ] = (uint8_t)cmd_set_realtime->action;
    return true;
}  

bool setRealtimeTask::initTask(){
    Tlm* tlm_from_set_realtime_to_uart = get_tlm_data_bucket_ptr( MODULE::SET_REALTIME, MODULE::UART );
    SET_REALTIME_TLM* param = (SET_REALTIME_TLM*)tlm_from_set_realtime_to_uart->param;
    for(uint8_t i=0;i<MAX_TLM_BUKETS_IN_DATA_CENTER;i++){
        param->action_set_real_time[i] = 0; 
    }
    return true;
};
bool setRealtimeTask::collectTlm(){
    return true;
};
bool setRealtimeTask::postTlm(){
    return true;
};
bool setRealtimeTask::processTaskRoutine(){
    static uint16_t rtc_time_old = 0;
    static uint16_t rtc_time_new = 0;
    static uint8_t loop_i_s = 0 ;
    Tlm* tlm_from_rtc_to_uart = get_tlm_data_bucket_ptr( MODULE::RTC, MODULE::UART );
    RTC_TLM* rtc_time_new_obj = (RTC_TLM*)(tlm_from_rtc_to_uart->param);
    rtc_time_new = rtc_time_new_obj->rtc_time_s;
    if( rtc_time_new==rtc_time_old && loop_i_s == 0 ){
        return true;
    }
    else{
        rtc_time_old = rtc_time_new;
    }
    // wait for 1 sec ,should be non blocking 
    Tlm* tlm_from_set_realtime_to_uart = get_tlm_data_bucket_ptr( MODULE::SET_REALTIME, MODULE::UART );
    SET_REALTIME_TLM* param = (SET_REALTIME_TLM*)(tlm_from_set_realtime_to_uart->param);
    // for(uint8_t i=0; i<MAX_TLM_BUKETS_IN_DATA_CENTER; i++){
    // }  --> sorry to say we cannot use the loop as we transfer only one packt at a time using satatic vaiable for looping 
    if( param->action_set_real_time[loop_i_s] == 1){
        Tlm* tlm_ptr = get_tlm_by_tlm_id( loop_i_s );
        notify_module_about_tlm( tlm_ptr );
    }
    if( loop_i_s >= (uint8_t)MAX_TLM_BUKETS_IN_DATA_CENTER-1 ){
        loop_i_s = (uint8_t)0;
    }
    else{
        loop_i_s += (uint8_t)1;
    }
    return true;
};

