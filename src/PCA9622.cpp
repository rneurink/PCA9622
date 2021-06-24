#include "PCA9622.h"
#include "I2C_coms.h"

/**
 * @brief This function instantiates the class object
 */
PCA9622::PCA9622(uint8_t i2c_address, uint8_t output_enable_pin) {
    _i2c_address = i2c_address;
    _OE_pin = output_enable_pin;
}

void PCA9622::begin() {
    i2c_init();
    pinMode(_OE_pin, OUTPUT);
    digitalWrite(_OE_pin, HIGH); // Disable outputs

    writeRegister(PCA9622_MODE1, 0x6F);
    delay(10);
}

void PCA9622::softwareReset() {
    i2c_write_byte(PCA9622_I2C_SW_RESET, 0xA5, 0x5A);
    delayMicroseconds(20);
}

/**
 * @brief Reads the value from the specified register
 * 
 * @param reg_addr the register address to read from
 * @return uint8_t the value from the specified register
 */
uint8_t PCA9622::readRegister(uint8_t reg_addr) {
    uint8_t data;
    i2c_read_byte(_i2c_address, reg_addr, &data);
    return data;
}

/**
 * @brief Writes a given value to the specified register
 * 
 * @param reg_addr the register address to write to
 * @param data the data to write to the register
 * @param addressType the I2C address type to write to 
 */
void PCA9622::writeRegister(uint8_t reg_addr, uint8_t data, EAddressType addressType) {
    i2c_write_byte(getAddress(addressType), reg_addr, data);
}

/*
 *  PRIVATE
 */ 

/**
 * @brief Resolves the EAddressType into an address
 * 
 * @param addressType the I2C address type to write to. This translates to the stored addresses in the class
 * @return uint8_t the I2C address translated from the addressType
 */
uint8_t PCA9622::getAddress(EAddressType addressType) {
    uint8_t i2c_address;
    switch (addressType)
    {
    case EAddressType::AllCall:
        i2c_address = _i2c_address_all_call;
        break;
    case EAddressType::SubCall1:
        i2c_address = _i2c_address_sub_1;
        break;
    case EAddressType::SubCall2:
        i2c_address = _i2c_address_sub_2;
        break;
    case EAddressType::SubCall3:
        i2c_address = _i2c_address_sub_3;
        break;
    case EAddressType::Normal:
    default:
    i2c_address = _i2c_address;
        break;
    }
    return i2c_address;
}