#include "i2c.hpp"

static I2C obj;

I2C& I2C::get_i2c_instence(){
    return obj;
}