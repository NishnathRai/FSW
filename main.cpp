#include "./src/module-blink/blink.hpp"

int main()
{   
    while(1){
        Blink& blink_obj = Blink::get_blink_instence();
        blink_obj.blink(ACTION_to_BLINK::NOWAY);
    }
    return 0;
}