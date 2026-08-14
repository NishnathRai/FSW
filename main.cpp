#include "./src/module-blink/blink.hpp"
#include "./src/module-uart/uard.hpp"
#include "./src/module-cmd/cmd.hpp"
#include "./src/module-setRealtime/setRealtime.hpp"


int main()
{   
    // get instance  
    UartTask& uart = UartTask::get_instance();
    BlinkTask& blink = BlinkTask::get_instance();
    setRealtimeTask& setRealtime = setRealtimeTask::get_instance();

    // init 
    uart.initTask();
    blink.initTask();
    setRealtime.initTask();





    while(1){
        // here we only have a single thread to process we are not doing with mutipile threads so we need to do a sertain amount of task and move forward . 

        // ex:- on an led from cmd from uard 
        // 1, uart task take take the cmd from uard interface pass it to module-cmd and it is done
        // 2, cmd reads the cmd, decodes it and send it module-Blink
        // 3, when blink task comes it goes into process task rotine and then make teh led on  
        
        // our task's
        // Uart task 
        // cmd task
        // tlm task
        // blink 

        
        uart.task();
        blink.task();
        setRealtime.task();
        
    }
    return 0;
}