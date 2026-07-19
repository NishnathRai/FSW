#include "./uard.hpp"
#include "../include-reg/reg.hpp"
#include "../module-cmd/cmd.hpp"

volatile char* udr0_ptr   = (char*)UDR0;
volatile char* ubrr0h_ptr = (char*)UBRR0H;
volatile char* ubrr0l_ptr = (char*)UBRR0L;
volatile char* ucsr0a_ptr = (char*)UCSR0A;
volatile char* ucsr0b_ptr = (char*)UCSR0B;
volatile char* ucsr0c_ptr = (char*)UCSR0C;

UartTask UartTask_box;

UartTask& UartTask::get_instance() {
    return UartTask_box;
};

// Helper functions

bool sendByte_helper(unsigned char*  data){
    while( !( *ucsr0a_ptr & (1<<5)  ) ){}
    (*udr0_ptr) = data[ 0 ];
    return true;
}


// 9600 bits/sec 
bool UartTask::initTask(){
    *ubrr0h_ptr = 0;
    *ubrr0l_ptr = 207;
    *ucsr0a_ptr = 1 << 1;
    *ucsr0b_ptr =  0b00011000;
    *ucsr0c_ptr =  0b00100110;
    return true;
}

bool UartTask::collectTlm(){
    return true;
}

bool UartTask::postTlm() {
    return true;
}

bool UartTask::processTaskRoutine() {
    return true;
}

// CMD'S

bool UartTask::startUart_cmd(void *param){
    return true;
}

bool UartTask::stopUart_cmd(void *param){
    return true;
}

#include <stdint.h>



uint16_t crc16_ccitt(const unsigned char* data, uint16_t length)
{
    uint16_t  crc = 0xFFFF;

    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= (uint16_t)data[i] << 8;

        for (uint16_t j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }

    return crc;
}




//  STAGE  +---------------------------------------------------------+
//    1    |    2   |   3    |    4     |    5       |    6   |   7  |
// +-------+--------+--------+----------+------------+--------+------+
// | START | LENGTH | MODULE | CMD/TLM  | PAYLOAD    | CRC16  | END  |
// +-------+--------+--------+----------+------------+--------+------+
// | 1 B   | 2 B    | 1 B    | 1 B      | N bytes    | 2 B    | 1 B  |
//   SIZE  +---------------------------------------------------------+



bool UartTask::sendData_cmd(void *param){
    Cmd *temp = (Cmd*)(param);
    unsigned char *data = (unsigned char*)temp->param;
    unsigned char *size_data =  (unsigned char*)(&temp->size);
    unsigned char data_start_stop[] = { uart_start_stop_byte  };
    sendByte_helper( &data_start_stop[0] );                                    // START
    sendByte_helper( &(size_data[0]) );   sendByte_helper( &(size_data[1]) );  // LENGTH
    sendByte_helper( &(temp->m_code) );                                        // MODULE
    sendByte_helper( &(temp->cmd_code) );                                      // CMD/TLM
    for(uint16_t i=0;i< temp->size ;i++){ 
        sendByte_helper( &data[i] );                                           // PAYLOAD 
    }
    uint16_t crc_temp = crc16_ccitt(data, temp->size);
    unsigned char* crc = (unsigned char*)(&crc_temp);
    sendByte_helper( crc );sendByte_helper( &(crc[1]) );           // CRC16
    sendByte_helper( &data_start_stop[0] );                                    // END
    return true;
}

