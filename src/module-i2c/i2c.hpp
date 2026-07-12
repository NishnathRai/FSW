class I2C{
public:
    I2C() = default;
    static I2C& get_i2c_instence();
};


enum class I2C_status{
    UNKNOWN
};