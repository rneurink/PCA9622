#ifndef __PCA9622_H
#define __PCA9622_H

#include <Arduino.h>
#include <Wire.h>

#define PCA9622_I2C_ALL_CALL    0xE0
#define PCA9622_I2C_SW_RESET    0x06
#define PCA9622_I2C_SUB_1       0xE2
#define PCA9622_I2C_SUB_2       0xE4
#define PCA9622_I2C_SUB_3       0xE8

// Registers
#define PCA9622_MODE1           0x00 // Mode register 1
#define PCA9622_MODE2           0x01 // Mode register 2
#define PCA9622_PWM0            0x02 // Brightness control LED0
// PWM1 on 0x03, PWM2 on 0x04 etc... to PWM15 on 0x11
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

// Auto increment flags
#define PCA9622_NO_AI           0x00 // No auto increment
#define PCA9622_AI_ALL          0x80 // Auto increment all registers. roll over at 0x1B to 0x00
#define PCA9622_AI_INDIVIDUAL   0xA0 // Auto increment individual brightness only. roll over at 0x11 to 0x02
#define PCA9622_AI_GLOBAL       0xC0 // Auto increment global control registers only. roll over at 0x13 to 0x12 
#define PCA9622_AI_INDI_GLOBAL  0xE0 // Auto increment individual and global registers only. roll over at 0x13 to 0x02

enum LED_Configuration {
    RGB,
    GRB,
    BGR,
    RBG,
    GBR,
    BRG,
    RGBA,
    GRBA,
    BGRA,
    RBGA,
    GBRA,
    BRGA,
};

enum LED_State {
    OFF = 0x00,
    ON = 0x01,
    PWM_CONTROL = 0x02,
    PWM_AND_GROUP_CONTROL = 0x03
};

enum EAddressType {
    Normal,
    AllCall,
    SubCall1,
    SubCall2,
    SubCall3
};

enum PCA9622_Configuration {
    ALL_CALL_OFF = 0 << 0,
    ALL_CALL_ON = 1 << 0,
    SUB_3_OFF = 0 << 1,
    SUB_3_ON = 1 << 1,
    SUB_2_OFF = 0 << 2,
    SUB_2_ON = 1 << 2,
    SUB_1_OFF = 0 << 3,
    SUB_1_ON = 1 << 3,
    SLEEP = 1 << 4,
    WAKEUP = 0 << 4
};

class PCA9622
{
    public:
        PCA9622(uint8_t i2c_address, uint8_t outputEnablePin); // Constructor
        PCA9622(uint8_t i2c_address, uint8_t outputEnablePin, LED_Configuration ledConfiguration); // Constructor with specific led configuration

        void begin();
        void softwareReset();

        void setOutputEnablePin(uint8_t outputEnablePin);
        void setLEDConfiguration(LED_Configuration ledConfiguration);
        void setI2CAddress(uint8_t i2c_address);

        void sleep();
        void wakeUp();
        void setSubAddress1(uint8_t address, EAddressType addressType = EAddressType::Normal);
        void setSubAddress2(uint8_t address, EAddressType addressType = EAddressType::Normal);
        void setSubAddress3(uint8_t address, EAddressType addressType = EAddressType::Normal);
        void setAllCallAddress(uint8_t address, EAddressType addressType = EAddressType::Normal);
        void configure(uint8_t configuration, EAddressType addressType = EAddressType::Normal);
        void enableGlobalDimming(EAddressType addressType = EAddressType::Normal);
        void enableGlobalBlinking(EAddressType addressType = EAddressType::Normal);

        uint8_t readRegister(uint8_t regAddress);
        void writeRegister(uint8_t regAddress, uint8_t data, EAddressType addressType = EAddressType::Normal);

        void writeMultiRegister(uint8_t startAddress, uint8_t *data, uint8_t count, EAddressType addressType = EAddressType::Normal);

        void enableOutputs();
        void disableOutputs();

        void setPWMOutput(uint8_t output, uint8_t value, EAddressType addressType = EAddressType::Normal);
        void setAllPWMOutputs(uint8_t value, EAddressType addressType = EAddressType::Normal);

        void setGroupPWM(uint8_t value, EAddressType addressType = EAddressType::Normal);
        uint16_t setGroupFrequency(uint16_t ms, EAddressType addressType = EAddressType::Normal);

        void test();

    protected:
    private:
        uint8_t _OE_pin;

        uint8_t _i2c_address;
        uint8_t _i2c_address_all_call = PCA9622_I2C_ALL_CALL;
        uint8_t _i2c_address_sw_reset = PCA9622_I2C_SW_RESET;
        uint8_t _i2c_address_sub_1 = PCA9622_I2C_SUB_1;
        uint8_t _i2c_address_sub_2 = PCA9622_I2C_SUB_2;
        uint8_t _i2c_address_sub_3 = PCA9622_I2C_SUB_3;

        LED_Configuration _led_configuration = RGB;

        uint8_t getAddress(EAddressType addressType);
        uint8_t fillLEDbuffer(uint8_t *buffer, LED_Configuration ledConfiguration);
};

#endif