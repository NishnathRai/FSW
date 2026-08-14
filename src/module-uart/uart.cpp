#include "./uard.hpp"
#include "../include-reg/reg.hpp"
#include "../module-cmd/cmd.hpp"
#include "../module-tlm/tlm.hpp"
#include "../module-datacenter/dataCenter.hpp"
#include "../module-blink/blink.hpp"

volatile char *udr0_ptr = (char *)UDR0;
volatile char *ubrr0h_ptr = (char *)UBRR0H;
volatile char *ubrr0l_ptr = (char *)UBRR0L;
volatile char *ucsr0a_ptr = (char *)UCSR0A;
volatile char *ucsr0b_ptr = (char *)UCSR0B;
volatile char *ucsr0c_ptr = (char *)UCSR0C;

UartTask UartTask_box;

UartTask &UartTask::get_instance()
{
    return UartTask_box;
};

// Helper functions

bool sendByte_helper(unsigned char *data)
{
    while (!(*ucsr0a_ptr & (1 << 5)))
    {
    }
    (*udr0_ptr) = data[0];
    return true;
}

uint16_t crc16_ccitt(const unsigned char *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
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

// try to read the data if available if we are in stage 1
// if we got the start byte keep on looping untile we we get the full data
bool accept_cmds()
{
    static unsigned char a=0;
    static Cmd *data = nullptr;
    static uint8_t STAGE = 1;
    static uint16_t size = 0;
    static uint16_t crc_temp = 0;
    static unsigned char m_code = 0;
    static unsigned char cmd_code = 0;
    static bool length_started = 0;
    static unsigned char loop_limit = 200;
    do
    {
        if (*ucsr0a_ptr & (1 << 7))
        {
            if (STAGE == 1 && *udr0_ptr == uart_start_stop_byte)
            {
                STAGE++;
            }
            else if (STAGE == 2)
            {
                if (length_started == 0)
                {
                    (&size)[0] = *udr0_ptr;
                    length_started = 1;
                }
                else
                {
                    (&size)[1] = *udr0_ptr;
                    length_started = 0;
                    STAGE++;
                }
            }
            else if (STAGE == 3)
            {
                m_code = *udr0_ptr;
                STAGE++;
            }
            else if (STAGE == 4)
            {
                cmd_code = *udr0_ptr;
                /// now we have every thing src_mdl_num , dst_mdl_num let's ask data center for the static varable
                data = get_cmd_data_bucket_ptr(MODULE::UART, MODULE(m_code));
                data->cmd_code = cmd_code;
                data->m_code = m_code;
                data->size = size;
                STAGE++;
            }
            else if (STAGE == 5)
            {
                if (size == 0)
                {
                    STAGE++;
                    continue;;
                }
                uint8_t* dta_ptr =  (uint8_t*)data->param;
                dta_ptr[ data->size - size ] = *udr0_ptr ;
                size--;
            }
            else if (STAGE == 6)
            {
                if (length_started == 0)
                {
                    (&crc_temp)[0] = *udr0_ptr;
                    length_started = 1;
                }
                else
                {
                    (&crc_temp)[1] = *udr0_ptr;
                    // if( crc_temp == crc16_ccitt( (unsigned char*)data->param , data->size ) ){
                    //     STAGE++;
                    // }
                    // else{
                    //     STAGE = 0;
                    STAGE++;
                    // }
                }
            }
            else if (STAGE == 7)
            {
                if (*udr0_ptr == uart_start_stop_byte)
                {
                    // Finally we got all the let us store in the datacenter and send it resp module as cmd
                    notify_module_about_cmd(data);
                    STAGE++;
                }
                else {
                    STAGE = 0;
                }
                STAGE = 1;
                break;
            }
        }
        // a = STAGE;
        // sendByte_helper(&a);
        loop_limit--;
    } while (STAGE > 1 && loop_limit > 0);
    return true;
}

// 9600 bits/sec
bool UartTask::initTask()
{
    *ubrr0h_ptr = 0;
    *ubrr0l_ptr = 207;
    *ucsr0a_ptr = 1 << 1;
    *ucsr0b_ptr = 0b00011000;
    *ucsr0c_ptr = 0b00100110;
    return true;
}

bool UartTask::collectTlm()
{
    return true;
}

bool UartTask::postTlm()
{
    return true;
}

bool UartTask::processTaskRoutine()
{
    return accept_cmds();
}

// CMD'S

bool UartTask::startUart_cmd(void *param)
{
    return true;
}

bool UartTask::stopUart_cmd(void *param)
{
    return true;
}

//  STAGE  +---------------------------------------------------------+
//    1    |    2   |   3    |    4     |    5       |    6   |   7  |
// +-------+--------+--------+----------+------------+--------+------+
// | START | LENGTH | MODULE | CMD/TLM  | PAYLOAD    | CRC16  | END  |
// +-------+--------+--------+----------+------------+--------+------+
// | 1 B   | 2 B    | 1 B    | 1 B      | N bytes    | 2 B    | 1 B  |
//   SIZE  +---------------------------------------------------------+

bool UartTask::sendData_cmd(void *param)
{
    Tlm *temp = (Tlm *)(param);
    unsigned char *data = (unsigned char *)temp->param;
    unsigned char *size_data = (unsigned char *)(&temp->size);
    unsigned char data_start_stop[] = {uart_start_stop_byte};
    sendByte_helper(&data_start_stop[0]); // START
    sendByte_helper(&(size_data[0]));
    sendByte_helper(&(size_data[1]));    // LENGTH
    sendByte_helper(&(temp->sm_code));   // MODULE
    sendByte_helper(&(temp->scmd_code)); // CMD/TLM
    for (uint16_t i = 0; i < temp->size; i++)
    {
        sendByte_helper(&data[i]); // PAYLOAD
    }
    uint16_t crc_temp = crc16_ccitt(data, temp->size);
    unsigned char *crc = (unsigned char *)(&crc_temp);
    sendByte_helper(crc);
    sendByte_helper(&(crc[1]));           // CRC16
    sendByte_helper(&data_start_stop[0]); // END
    return true;
}
