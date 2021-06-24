#ifndef __PCA9622_H
#define __PCA9622_H

#include <Arduino.h>
#include <Wire.h>

#define PCA9622_I2C_ALL_CALL    0xE0
#define PCA9622_I2C_SW_RESET    0x0C
#define PCA9622_I2C_SUB_1       0xE2
#define PCA9622_I2C_SUB_2       0xE4
#define PCA9622_I2C_SUB_3       0xE8

// Registers
#define PCA9622_MODE1           0x00 // Mode register 1
#define PCA9622_MODE2           0x01 // Mode register 2
#define PCA9622_PWM0            0x02 // Brightness control LED0
#define PCA9622_GRPPWM          0x12 // Group duty cycle control
#define PCA9622_GRPFREQ         0x13 // Group frequency
#define PCA9622_LED_OUT0        0x14 // LED output state 0
#define PCA9622_LED_OUT1        0x15 // LED output state 1
#define PCA9622_LED_OUT2        0x16 // LED output state 2
#define PCA9622_LED_OUT3        0x17 // LED output state 3
#define PCA9622_SUB_ADR1        0x18 // I2C subaddress 1
#define PCA9622_SUB_ADR2        0x19 // I2C subaddress 2
#define PCA9622_SUB_ADR3        0x1A // I2C subaddress 3
#define PCA9622_ALL_CALL        0x1B // I2C All call address

enum EAddressType {
    Normal,
    AllCall,
    SubCall1,
    SubCall2,
    SubCall3
};

class PCA9622
{
    public:
        PCA9622(uint8_t i2c_address, uint8_t output_enable_pin); // Constructor

        void begin();
        void softwareReset();

        uint8_t readRegister(uint8_t reg_addr);
        void writeRegister(uint8_t reg_addr, uint8_t data, EAddressType addressType = EAddressType::Normal);

    protected:
    private:
        uint8_t _OE_pin;

        uint8_t _i2c_address;
        uint8_t _i2c_address_all_call = PCA9622_I2C_ALL_CALL;
        uint8_t _i2c_address_sw_reset = PCA9622_I2C_SW_RESET;
        uint8_t _i2c_address_sub_1 = PCA9622_I2C_SUB_1;
        uint8_t _i2c_address_sub_2 = PCA9622_I2C_SUB_2;
        uint8_t _i2c_address_sub_3 = PCA9622_I2C_SUB_3;

        uint8_t getAddress(EAddressType addressType);
};

#endif