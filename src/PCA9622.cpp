#include "PCA9622.h"
#include "I2C_coms.h"

/*----------------------- Initialisation functions --------------------------*/

/**
 * @brief This function instantiates the class object
 * 
 * @param i2c_address The I2C address of the PCA9622 chip
 * @param outputEnablePin The arduino pin that is connected to the ~OE pin of the PCA9266
 */
PCA9622::PCA9622(uint8_t i2c_address, uint8_t outputEnablePin) {
    _i2c_address = i2c_address;
    _OE_pin = outputEnablePin;
}

/**
 * @brief This function instantiates the class object with a specified LED configuration
 * 
 * @param i2c_address The I2C address of the PCA9622 chip
 * @param outputEnablePin The arduino pin that is connected to the ~OE pin of the PCA9266
 * @param ledConfiguration The LED configuration in which the leds are attached to the outputs. See @ref LED_Configuration
 */
PCA9622::PCA9622(uint8_t i2c_address, uint8_t outputEnablePin, LED_Configuration ledConfiguration) {
    _i2c_address = i2c_address;
    _OE_pin = outputEnablePin;
    _led_configuration = ledConfiguration;
}

/**
 * @brief Initializes the I2C bus and the PCA9622
 * 
 */
void PCA9622::begin() {
    i2c_init();
    pinMode(_OE_pin, OUTPUT);
    disableOutputs();

    softwareReset();

    writeRegister(PCA9622_MODE1, 0x6E);
    delay(10);
}

/**
 * @brief Resets the PCA9622 @warning resets all PCA9622 devices on the I2C Bus!
 * 
 */
void PCA9622::softwareReset() {
    i2c_write_byte(PCA9622_I2C_SW_RESET, 0xA5, 0x5A);
    // Wait a few microseconds for the reset to complete. Ready after the specified bus free time. (100kHz: 4.7us, 400kHz: 1.3us, 1MHz: 0.5us)
    delayMicroseconds(20);
}


/*----------------------- Configuration functions ---------------------------*/

/**
 * @brief Sets the output enable pin used in the library. This pin is used to turn the outputs on or off
 * 
 * @param outputEnablePin 
 */
void PCA9622::setOutputEnablePin(uint8_t outputEnablePin) {
    _OE_pin = outputEnablePin;
}

/**
 * @brief Sets the LED configuration acording the @ref LED_Configuration enum
 * 
 * @param ledConfiguration 
 */
void PCA9622::setLEDConfiguration(LED_Configuration ledConfiguration) {
    _led_configuration = ledConfiguration;
}

/**
 * @brief Sets the I2C address
 * 
 * @param i2c_address 
 */
void PCA9622::setI2CAddress(uint8_t i2c_address) {
    _i2c_address = i2c_address;
}

void PCA9622::sleep() {
    i2c_write_byte(_i2c_address, PCA9622_MODE1, readRegister(PCA9622_MODE1) | PCA9622_Configuration::SLEEP);
}

void PCA9622::wakeUp() {
    i2c_write_byte(_i2c_address, PCA9622_MODE1, readRegister(PCA9622_MODE1) | PCA9622_Configuration::WAKEUP);
    delayMicroseconds(500);
}

void PCA9622::setSubAddress1(uint8_t address, EAddressType addressType) {
    uint8_t retVal = i2c_write_byte(getAddress(addressType), PCA9622_I2C_SUB_1, address);
    if (retVal == 0) {
        _i2c_address_sub_1 = address;
    }
}

void PCA9622::setSubAddress2(uint8_t address, EAddressType addressType) {
    uint8_t retVal = i2c_write_byte(getAddress(addressType), PCA9622_I2C_SUB_2, address);
    if (retVal == 0) {
        _i2c_address_sub_2 = address;
    }
}

void PCA9622::setSubAddress3(uint8_t address, EAddressType addressType) {
    uint8_t retVal = i2c_write_byte(getAddress(addressType), PCA9622_I2C_SUB_3, address);
    if (retVal == 0) {
        _i2c_address_sub_3 = address;
    }
}

void PCA9622::setAllCallAddress(uint8_t address, EAddressType addressType) {
    uint8_t retVal = i2c_write_byte(getAddress(addressType), PCA9622_ALL_CALL, address);
    if (retVal == 0) {
        _i2c_address_all_call = address;
    }
}

void PCA9622::configure(uint8_t configuration, EAddressType addressType) {
    i2c_write_byte(getAddress(addressType), PCA9622_MODE1, configuration);
}

void PCA9622::enableGlobalDimming(EAddressType addressType) {
    
}

void PCA9622::enableGlobalBlinking(EAddressType addressType) {

}


/*----------------------- General control functions -------------------------*/

/**
 * @brief Reads the value from the specified register
 * 
 * @param regAddress the register address to read from
 * @return uint8_t the value from the specified register
 */
uint8_t PCA9622::readRegister(uint8_t regAddress) {
    uint8_t data;
    i2c_read_byte(_i2c_address, regAddress, &data);
    return data;
}

/**
 * @brief Writes a given value to the specified register
 * 
 * @param regAddress the register address to write to
 * @param data the data to write to the register
 * @param addressType the I2C address type to write to 
 */
void PCA9622::writeRegister(uint8_t regAddress, uint8_t data, EAddressType addressType) {
    i2c_write_byte(getAddress(addressType), regAddress, data);
}



/**
 * @brief Drives the ~OE pin low and enables the outputs of the PCA9622
 * 
 */
void PCA9622::enableOutputs() {
    digitalWrite(_OE_pin, LOW);
}

/**
 * @brief Drives the ~OE pin high and disables the outputs of the PCA9622
 * 
 */
void PCA9622::disableOutputs() {
    digitalWrite(_OE_pin, HIGH);
}



/**
 * @brief Sets the pwm output of the driver
 * 
 * @param output The output to write to from 0..15
 * @param value The PWM value to write from 0..255
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setPWMOutput(uint8_t output, uint8_t value, EAddressType addressType) {
    i2c_write_byte(getAddress(addressType), PCA9622_PWM0 + output, value);
}

/**
 * @brief Sets all the pwm outputs of the driver
 * 
 * @param value The PWM value to write from 0..255
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setAllPWMOutputs(uint8_t value, EAddressType addressType) {
    uint8_t buffer[16];
    for (uint8_t i = 0; i < 16; i++) {
        buffer[i] = value;
    }
    i2c_write_multi(getAddress(addressType), PCA9622_PWM0 | PCA9622_AI_INDIVIDUAL, buffer, 16);
}

/**
 * @brief Sets the group duty cycle. 
 * When DMBLNK is set to 0, a 190Hz fixed frequency signal is superimposed with the 97kHz individual brightness control signal.
 * When DMBLNK is set to 1, togheter with the group frequency defines a global blinking pattern in which the value will determine the duty cycle of the ON/OFF ratio as a percentage
 * 
 * @param value 
 * @param addressType 
 */
void PCA9622::setGroupPWM(uint8_t value, EAddressType addressType) {
    i2c_write_byte(getAddress(addressType), PCA9622_GRPPWM, value);
}

uint16_t PCA9622::setGroupFrequency(uint16_t ms, EAddressType addressType) {
    if (ms < 42) ms = 42;
    if (ms > 10666) ms = 10666;
    uint8_t regValue = (uint8_t)((((uint32_t)ms * 24) - 1000) / 1000);
    uint16_t actualValue = (uint16_t)(((regValue + 1) * 1000) / 24);

    i2c_write_byte(getAddress(addressType), PCA9622_GRPFREQ, regValue);
    return actualValue;
}


/*----------------------- RGB control functions -----------------------------*/




/*------------------------- Helper functions --------------------------------*/

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




void PCA9622::test() {
    uint8_t buffer[] = {0x10, 0x20, 0x30};
    i2c_write_multi(_i2c_address, 0xE2, buffer, 3);
    delay(10);
    i2c_read_multi(_i2c_address, 0xE0, buffer, 3);
}