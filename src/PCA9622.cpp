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

/**
 * @brief Sets the sleep bit. Turns off the oscillator and sets the chip to low power mode
 * 
 */
void PCA9622::sleep() {
    writeRegister(PCA9622_MODE1, readRegister(PCA9622_MODE1) | PCA9622_Configuration::SLEEP);
}

/**
 * @brief Disables the sleep bit. Turns on the oscillator, this takes about a maximum of 500us
 * 
 */
void PCA9622::wakeUp() {
    writeRegister(PCA9622_MODE1, readRegister(PCA9622_MODE1) | PCA9622_Configuration::WAKEUP);
    delayMicroseconds(500);
}

/**
 * @brief Sets the I2C sub address 1. @note this also sets the class variable
 * 
 * @param address The I2C sub address 1 to set
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setSubAddress1(uint8_t address, EAddressType addressType) {
    uint8_t retVal = writeRegister(PCA9622_I2C_SUB_1, address, addressType);
    if (retVal == 0) {
        _i2c_address_sub_1 = address;
    }
}

/**
 * @brief Sets the I2C sub address 2. @note this also sets the class variable
 * 
 * @param address The I2C sub address 2 to set
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setSubAddress2(uint8_t address, EAddressType addressType) {
    uint8_t retVal = writeRegister(PCA9622_I2C_SUB_2, address, addressType);
    if (retVal == 0) {
        _i2c_address_sub_2 = address;
    }
}

/**
 * @brief Sets the I2C sub address 3. @note this also sets the class variable
 * 
 * @param address The I2C sub address 3 to set
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setSubAddress3(uint8_t address, EAddressType addressType) {
    uint8_t retVal = writeRegister(PCA9622_I2C_SUB_3, address, addressType);
    if (retVal == 0) {
        _i2c_address_sub_3 = address;
    }
}

/**
 * @brief Sets the I2C all call address. @note this also sets the class variable
 * 
 * @param address The I2C all call address to set
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setAllCallAddress(uint8_t address, EAddressType addressType) {
    uint8_t retVal = writeRegister(PCA9622_ALL_CALL, address, addressType);
    if (retVal == 0) {
        _i2c_address_all_call = address;
    }
}

/**
 * @brief Configures the device according to the configuration param
 * 
 * @param configuration The configuration of the device, this can be made by bitwise OR ('|') the enum @ref PCA9622_Configuration
 * @param addressType the I2C address type to write to 
 */
void PCA9622::configure(uint8_t configuration, EAddressType addressType) {
    writeRegister(PCA9622_MODE1, configuration, addressType);
}

void PCA9622::enableGlobalDimming(EAddressType addressType) {
    writeRegister(PCA9622_MODE2, readRegister(PCA9622_MODE2) & ~(1 << 5), addressType);
}

void PCA9622::enableGlobalBlinking(EAddressType addressType) {
    writeRegister(PCA9622_MODE2, readRegister(PCA9622_MODE2) | (1 << 5), addressType);
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
uint8_t PCA9622::writeRegister(uint8_t regAddress, uint8_t data, EAddressType addressType) {
    return i2c_write_byte(getAddress(addressType), regAddress, data);
}

uint8_t PCA9622::writeMultiRegister(uint8_t startAddress, uint8_t *data, uint8_t count, EAddressType addressType) {
    i2c_write_multi(getAddress(addressType), startAddress, data, count);
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
    writeRegister(PCA9622_PWM0 + output, value, addressType);
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
    writeMultiRegister(PCA9622_PWM0 | PCA9622_AI_INDIVIDUAL, buffer, 16, addressType);
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
    writeRegister(PCA9622_GRPPWM, value, addressType);
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

void PCA9622::setLEDColor(uint8_t led, uint8_t red, uint8_t green, uint8_t blue, EAddressType addressType) {

}

void PCA9622::setLEDColor(uint8_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t amber, EAddressType addressType) {

}


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

void fillLEDbuffer(uint8_t *buffer, LED_Configuration ledConfiguration) {

}


void PCA9622::test() {
    uint8_t buffer[] = {0x10, 0x20, 0x30};
    i2c_write_multi(_i2c_address, 0xE2, buffer, 3);
    delay(10);
    i2c_read_multi(_i2c_address, 0xE0, buffer, 3);
}