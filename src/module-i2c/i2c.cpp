#include "i2c.hpp"
#include <stdint.h>
#include "../include-reg/reg.hpp"
#include "../module-datacenter/dataCenter.hpp"

volatile uint8_t* twbr_ptr = (uint8_t*)TWBR;
volatile uint8_t* twcr_ptr = (uint8_t*)TWCR;
volatile uint8_t* twsr_ptr = (uint8_t*)TWSR;
volatile uint8_t* twdr_ptr = (uint8_t*)TWDR;
volatile uint8_t* twar_ptr = (uint8_t*)TWAR;
volatile uint8_t* twamr_ptr = (uint8_t*)TWAMR;
volatile uint8_t* sreg_ptr = (uint8_t*)SREG;

I2cTask i2c_box;

I2cTask& I2cTask::get_instance(){
    return i2c_box;
}

bool I2cTask::processCmd(){
    return true;
}
bool I2cTask::collectTlm(){
    return true;
}
bool I2cTask::postTlm(){
    return true;
}

uint8_t I2cTask::get_slave_data(uint8_t slave_add,uint8_t slave_reg){
    uint8_t STAGE = 0;
    uint8_t data = 0;
    uint16_t loop_limit = 10;                              // <-- safety cap
    do{ 
        if(STAGE == 0){ 
            STAGE++;
        }
        else if(STAGE == 1){
            *twcr_ptr |= (1<<7)|(1<<5);                  //send start
            STAGE++;
        }
        else if( STAGE == 2 && (*twsr_ptr==0x48) ){       //start cond has been transmitted no ack has been received
            data =  0xFE;                                 //some random 
            break;
        }
        else if( STAGE == 2 && (*twsr_ptr==0x08) ){      //start cond has been transmitted ack has been received 
            *twcr_ptr &=  ~(1<<5);                       //clear send start bit 
            STAGE++;
        }
        else if(STAGE == 3){
            *twdr_ptr = (slave_add<<1) | (1);            //load slave add and read bit 
            *twcr_ptr |=  (1<<7);                        //set int
            STAGE++;
        }
        else if( STAGE == 4 &&  (*twsr_ptr==0x40) ){     //add + R has been transmitted and ack has been rec
            *twdr_ptr = slave_reg;                       //load slave reg add  
            *twcr_ptr |=  (1<<7);                        //set int
            STAGE++;
        }
        else if(STAGE == 5 && (*twsr_ptr==0x58) ){      //data byte has been rec and ack has been sent 
            data = *twdr_ptr;                           //copyed the data 
            *twcr_ptr |=  (1<<4);                       //send stop
            STAGE++;
        }
        else if(STAGE ==6 && ( (*twcr_ptr & (1<<4)) == 0 ) ){
            STAGE++;
        }
        else{
            ///we need to also protect our sytem which can stuck in some stage for long time 
            loop_limit--;
        }

    }while( STAGE<=6 && loop_limit>0 );
    if(loop_limit==0){
        data = *twsr_ptr;
    }
    return data;
}
bool I2cTask::processTaskRoutine(){
    Tlm* tlm_ptr = get_tlm_by_tlm_id(4);
    I2C_TLM* i2c_tlm = (I2C_TLM*)(tlm_ptr->param);
    for(uint8_t i=0; i<SIZE_i2c_8_bit_slaves; i++){
        i2c_tlm->i2c_8_bit_data_reg[i][0] = this->i2c_8_bit_data_reg[i][0];
        for(uint8_t j=1; j< (SIZE_i2c_8_bit_data_reg+1) ; j++){
            i2c_tlm->i2c_8_bit_data_reg_data[ i ][ j-1 ] = this->get_slave_data(i2c_tlm->i2c_8_bit_data_reg[i][0],this->i2c_8_bit_data_reg[i][j]);
            i2c_tlm->i2c_8_bit_data_reg[i][j] = this->i2c_8_bit_data_reg[i][j];
        }
    }
    // i2c_tlm->i2c_8_bit_data_reg[0][0] = 12;
    return true;
}
bool I2cTask::initTask(){
    *twbr_ptr = 12; //400 KHz 
    *twcr_ptr = (1 << 2);         // TWEN only — enable I2C,  no interrupt
    // *twsr_ptr|= 0x00000000;  //prescale inital value is 00
    *sreg_ptr|= 0b10000000;
    return true;
}