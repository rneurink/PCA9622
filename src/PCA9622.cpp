/**
 * @file PCA9622.cpp
 * @author rneurink (ruben.neurink@gmail.com)
 * @brief Arduino driver to control the PCA9622
 * @version 1.1.0
 * @date 2021-06-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "PCA9622.h"
#include "I2C_coms.h"

/*----------------------- Initialisation functions --------------------------*/

/**
 * @brief This function instantiates the class object
 * 
 * @param i2c_address 
 */
PCA9622::PCA9622(uint8_t i2c_address) {
    _i2c_address = i2c_address;
}

/**
 * @brief This function instantiates the class object with an ~OE pin
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
    if (_OE_pin != 0xFF) {
        pinMode(_OE_pin, OUTPUT);
    }
    disableOutputs();

    wakeUp();
    // Set all outputs to PWM_AND_GROUP_CONTROL
    uint8_t buffer[] = {0xFF, 0xFF, 0xFF, 0xFF};
    writeMultiRegister(PCA9622_LED_OUT0 | PCA9622_AI_ALL, buffer, 4); // Sets the led output state
}

/**
 * @brief Resets the PCA9622 @warning resets all PCA9622 devices on the I2C Bus! @note the PCA9266 resets in low power mode so make sure to call @ref wakeUp to power up the device
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
 * @brief Sets the I2C address @note this only sets the library internal address. This does not change the I2C address of the device!
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
    writeRegister(PCA9622_MODE1, (readRegister(PCA9622_MODE1) & ~(PCA9622_Configuration::SLEEP)) | PCA9622_Configuration::WAKEUP);
    delayMicroseconds(500);
}

/**
 * @brief Sets the I2C sub address 1. @note this also sets the class variable
 * 
 * @param address The I2C sub address 1 to set
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setSubAddress1(uint8_t address, EAddressType addressType) {
    uint8_t retVal = writeRegister(PCA9622_SUB_ADR1, address, addressType);
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
    uint8_t retVal = writeRegister(PCA9622_SUB_ADR2, address, addressType);
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
    uint8_t retVal = writeRegister(PCA9622_SUB_ADR3, address, addressType);
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

/**
 * @brief Enables group dimming through the GRPPWM register. 
 * See @ref setGroupPWM to set the group dimming
 * 
 * @param addressType the I2C address type to write to 
 */
void PCA9622::enableGroupDimming(EAddressType addressType) {
    writeRegister(PCA9622_MODE2, readRegister(PCA9622_MODE2) & ~(1 << 5), addressType);
}

/**
 * @brief Enables group blinking through the GRPPWM and GRPFREQ register. 
 * See @ref setGroupPWM and @ref setGroupFrequency to set the group blinking cuty cycle and interval
 * 
 * @param addressType the I2C address type to write to 
 */
void PCA9622::enableGroupBlinking(EAddressType addressType) {
    writeRegister(PCA9622_MODE2, readRegister(PCA9622_MODE2) | (1 << 5), addressType);
}

/**
 * @brief Sets the output state of a led channel @note LED channel specified by the library not the device!. This will change 3 outputs if RGB like configuration is set and 4 outputs on RGBA like configurations
 * 
 * @param led The led to set the output state of. For RGB like configurations 0..4 (output 15 is skipped) and RGBA like configurations 0..3
 * @param ledState The state of the led to set. See @ref LED_State
 */
void PCA9622::setLEDOutputState(uint8_t led, LED_State ledState) {
    if (_led_configuration < 6) {// RGB like
        if (led > 4) led = 4;
        uint8_t currentState[4];
        readMultiRegister(PCA9622_LED_OUT0 | PCA9622_AI_ALL, currentState, 4);
        
        uint32_t mask = (uint32_t)0x3F << (led * 6);;
        uint32_t state = ((uint32_t)currentState[0] & 0xFF) | (((uint32_t)currentState[1] << 8) & 0xFF00) | (((uint32_t)currentState[2] << 16) & 0xFF0000) | (((uint32_t)currentState[3] << 24) & 0xFF000000);
        state &= ~mask;
        
        state |= ((uint32_t)ledState << (4 + (led * 6))) | ((uint32_t)ledState << (2 + (led * 6))) | ((uint32_t)ledState << (0 + (led * 6)));
        currentState[0] = state & 0xFF;
        currentState[1] = (state >> 8) & 0xFF;
        currentState[2] = (state >> 16) & 0xFF;
        currentState[3] = (state >> 24) & 0xFF;
        writeMultiRegister(PCA9622_LED_OUT0 | PCA9622_AI_ALL, currentState, 4);
    } else { // RGBA like
        if (led > 3) led = 3;
        writeRegister(PCA9622_LED_OUT0 + led, ((uint8_t)ledState << 6) | ((uint8_t)ledState << 4) | ((uint8_t)ledState << 2) | ((uint8_t)ledState << 0));
    }
}

/**
 * @brief Sets the output state of a led channel @note LED channel specified by the device not the library. So it will always change 4 outputs
 * 
 * @param led The led to set the output state of. from 0..3. LED 0 controls output 0..3 and so on
 * @param ledState The state of the led to set. See @ref LED_State
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setOutputState(uint8_t led, LED_State ledState, EAddressType addressType) {
    uint8_t state = ((uint8_t)ledState << 6) | ((uint8_t)ledState << 4) | ((uint8_t)ledState << 2) | ((uint8_t)ledState << 0);
    writeRegister(PCA9622_LED_OUT0 + led, state, addressType);
}

/**
 * @brief Sets the output state of an output
 * 
 * @param output The output to set the state of. from 0..15
 * @param ledState The state of the led to set. See @ref LED_State
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setPWMOutputState(uint8_t output, LED_State ledState, EAddressType addressType) {
    if (output <= 3) {
        writeRegister(PCA9622_LED_OUT0, ((readRegister(PCA9622_LED_OUT0) & ~(0x3 << output * 2)) | ((uint8_t)ledState << output * 2)), addressType);
    } else if (output <= 7) {
        writeRegister(PCA9622_LED_OUT1, ((readRegister(PCA9622_LED_OUT1) & ~(0x3 << (output % 4) * 2)) | ((uint8_t)ledState << (output % 4) * 2)), addressType);
    } else if (output <= 11) {
        writeRegister(PCA9622_LED_OUT2, ((readRegister(PCA9622_LED_OUT2) & ~(0x3 << (output % 4) * 2)) | ((uint8_t)ledState << (output % 4) * 2)), addressType);
    } else if (output <= 15) {
        writeRegister(PCA9622_LED_OUT3, ((readRegister(PCA9622_LED_OUT3) & ~(0x3 << (output % 4) * 2)) | ((uint8_t)ledState << (output % 4) * 2)), addressType);
    }
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
 * @return 0:success
 * @return 1:data too long to fit in transmit buffer
 * @return 2:received NACK on transmit of address
 * @return 3:received NACK on transmit of data
 * @return 4:other error
 */
uint8_t PCA9622::writeRegister(uint8_t regAddress, uint8_t data, EAddressType addressType) {
    return i2c_write_byte(getAddress(addressType), regAddress, data);
}

/**
 * @brief Writes a given buffer to the specified start address and subsequent addresses
 * 
 * @param startAddress the register start address to write to
 * @param data the data to write to the registers
 * @param count the amount of data to write
 * @param addressType the I2C address type to write to 
 * @return 0:success
 * @return 1:data too long to fit in transmit buffer
 * @return 2:received NACK on transmit of address
 * @return 3:received NACK on transmit of data
 * @return 4:other error
 */
uint8_t PCA9622::writeMultiRegister(uint8_t startAddress, uint8_t *data, uint8_t count, EAddressType addressType) {
    return i2c_write_multi(getAddress(addressType), startAddress, data, count);
}

/**
 * @brief Reads from the specified start address and subsequent addresses to the given buffer
 * 
 * @param startAddress the register start address to read from
 * @param data the data buffer to read to
 * @param count the amount of data to read
 */
uint8_t PCA9622::readMultiRegister(uint8_t startAddress, uint8_t *data, uint8_t count) {
    return i2c_read_multi(_i2c_address, startAddress, data, count);
}

/**
 * @brief Drives the ~OE pin low and enables the outputs of the PCA9622
 * 
 */
void PCA9622::enableOutputs() {
    if (_OE_pin != 0xFF) {
        digitalWrite(_OE_pin, LOW);
    }
}

/**
 * @brief Drives the ~OE pin high and disables the outputs of the PCA9622
 * 
 */
void PCA9622::disableOutputs() {
    if (_OE_pin != 0xFF) {
        digitalWrite(_OE_pin, HIGH);
    }
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
 * When DMBLNK is set to 1, togheter with the group frequency defines a group blinking pattern in which the value will determine the duty cycle of the ON/OFF ratio as a percentage
 * 
 * @param value The pwm duty cycle
 * @param addressType the I2C address type to write to 
 */
void PCA9622::setGroupPWM(uint8_t value, EAddressType addressType) {
    writeRegister(PCA9622_GRPPWM, value, addressType);
}

/**
 * @brief Sets the blinking frequency
 * When DMBLNK is 0 this register does nothing
 * When DMBLNK is 1 this register is used to program the blinking delay
 * 
 * @param ms the blinking delay in ms from 42..10666. Not all values are valid and it will find the closest valid value.
 * @param addressType the I2C address type to write to 
 * @return uint16_t The real time in ms that the register will be programmed to as not all values are valid
 */
uint16_t PCA9622::setGroupFrequency(uint16_t ms, EAddressType addressType) {
    if (ms < 42) ms = 42;
    if (ms > 10666) ms = 10666;
    uint8_t regValue = (uint8_t)((((uint32_t)ms * 24) - 1000) / 1000);
    uint16_t actualValue = (uint16_t)(((regValue + 1) * 1000) / 24);

    writeRegister(PCA9622_GRPFREQ, regValue, addressType);
    return actualValue;
}


/*----------------------- RGB control functions -----------------------------*/

/**
 * @brief Sets the LED color according to the set LED configuration @ref setLEDConfiguration
 * 
 * @param led The LED to set the color of. If the led configuration is set to RGB or alike (3 color channels) a maximum of 5 leds are supported
 * @param red The red color value from 0 to 0xFF
 * @param green The green color value from 0 to 0xFF
 * @param blue The blue color value from 0 to 0xFF
 * @param addressType the I2C address type to write to
 */
void PCA9622::setLEDColor(uint8_t led, uint8_t red, uint8_t green, uint8_t blue, EAddressType addressType) {
    uint8_t buffer[3];
    fillLEDbuffer(red, green, blue, buffer);
    writeMultiRegister((PCA9622_PWM0 + (3 * led)) | PCA9622_AI_INDIVIDUAL, buffer, 3, addressType);
}

/**
 * @brief 
 * 
 * @param led The LED to set the color of. If the led configuration is set to RGBA or alike (4 color channels) a maximum of 4 leds are supported
 * @param red The red color value from 0 to 0xFF
 * @param green The green color value from 0 to 0xFF
 * @param blue The blue color value from 0 to 0xFF
 * @param amber The amber color value from 0 to 0xFF. Could also be white or another color of course
 * @param addressType the I2C address type to write to
 */
void PCA9622::setLEDColor(uint8_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t amber, EAddressType addressType) {
    uint8_t buffer[4];
    fillLEDbuffer(red, green, blue, amber, buffer);
    writeMultiRegister((PCA9622_PWM0 + (4 * led)) | PCA9622_AI_INDIVIDUAL, buffer, 4, addressType);
}

/**
 * @brief Sets all LEDs color according to the set LED configuration @ref setLEDConfiguration
 * 
 * @param red The red color value from 0 to 0xFF
 * @param green The green color value from 0 to 0xFF
 * @param blue The blue color value from 0 to 0xFF
 * @param addressType the I2C address type to write to
 */
void PCA9622::setAllLEDColor(uint8_t red, uint8_t green, uint8_t blue, EAddressType addressType) {
    uint8_t buffer[3*5];
    fillLEDbuffer(red, green, blue, buffer, 5);
    writeMultiRegister(PCA9622_PWM0 | PCA9622_AI_INDIVIDUAL, buffer, 15, addressType);
}

/**
 * @brief Sets all LEDs color according to the set LED configuration @ref setLEDConfiguration
 * 
 * @param red The red color value from 0 to 0xFF
 * @param green The green color value from 0 to 0xFF
 * @param blue The blue color value from 0 to 0xFF
 * @param amber The amber color value from 0 to 0xFF. Could also be white or another color of course
 * @param addressType the I2C address type to write to
 */
void PCA9622::setAllLEDColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t amber, EAddressType addressType) {
    uint8_t buffer[4*4];
    fillLEDbuffer(red, green, blue, amber, buffer, 4);
    writeMultiRegister(PCA9622_PWM0 | PCA9622_AI_INDIVIDUAL, buffer, 16, addressType);
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

/**
 * @brief Fills a led buffer acording to the set LED configuration @ref setLEDConfiguration
 * 
 * @param red The red color value from 0 to 0xFF
 * @param green The green color value from 0 to 0xFF
 * @param blue The blue color value from 0 to 0xFF
 * @param buffer The buffer to fill
 * @param ledCount The amount of leds that the buffer should address
 */
void PCA9622::fillLEDbuffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t *buffer, uint8_t ledCount) {
    for (uint8_t i = 0; i < ledCount; i++) {
        switch (_led_configuration) {
            case RGB:
                buffer[0 + (i * 3)] = red;
                buffer[1 + (i * 3)] = green;
                buffer[2 + (i * 3)] = blue;
                break;
            case GRB:
                buffer[0 + (i * 3)] = green;
                buffer[1 + (i * 3)] = red;
                buffer[2 + (i * 3)] = blue;
                break;
            case BGR:
                buffer[0 + (i * 3)] = blue;
                buffer[1 + (i * 3)] = green;
                buffer[2 + (i * 3)] = red;
                break;
            case RBG:
                buffer[0 + (i * 3)] = red;
                buffer[1 + (i * 3)] = blue;
                buffer[2 + (i * 3)] = green;
                break;
            case GBR:
                buffer[0 + (i * 3)] = green;
                buffer[1 + (i * 3)] = blue;
                buffer[2 + (i * 3)] = red;
                break;
            case BRG:
                buffer[0 + (i * 3)] = blue;
                buffer[1 + (i * 3)] = red;
                buffer[2 + (i * 3)] = green;
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Fills a led buffer acording to the set LED configuration @ref setLEDConfiguration
 * 
 * @param red The red color value from 0 to 0xFF
 * @param green The green color value from 0 to 0xFF
 * @param blue The blue color value from 0 to 0xFF
 * @param amber The amber color value from 0 to 0xFF. Could also be white or another color of course 
 * @param buffer The buffer to fill
 * @param ledCount The amount of leds that the buffer should address
 */
void PCA9622::fillLEDbuffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t amber, uint8_t *buffer, uint8_t ledCount) {
    for (uint8_t i = 0; i < ledCount; i++) {
        switch (_led_configuration) {
            case RGBA:
                buffer[0 + (i * 4)] = red;
                buffer[1 + (i * 4)] = green;
                buffer[2 + (i * 4)] = blue;
                buffer[3 + (i * 4)] = amber;
                break;
            case GRBA:
                buffer[0 + (i * 4)] = green;
                buffer[1 + (i * 4)] = red;
                buffer[2 + (i * 4)] = blue;
                buffer[3 + (i * 4)] = amber;
                break;
            case BGRA:
                buffer[0 + (i * 4)] = blue;
                buffer[1 + (i * 4)] = green;
                buffer[2 + (i * 4)] = red;
                buffer[3 + (i * 4)] = amber;
                break;
            case RBGA:
                buffer[0 + (i * 4)] = red;
                buffer[1 + (i * 4)] = blue;
                buffer[2 + (i * 4)] = green;
                buffer[3 + (i * 4)] = amber;
                break;
            case GBRA:
                buffer[0 + (i * 4)] = green;
                buffer[1 + (i * 4)] = blue;
                buffer[2 + (i * 4)] = red;
                buffer[3 + (i * 4)] = amber;
                break;
            case BRGA:
                buffer[0 + (i * 4)] = blue;
                buffer[1 + (i * 4)] = red;
                buffer[2 + (i * 4)] = green;
                buffer[3 + (i * 4)] = amber;
                break;
            case ARGB:
                buffer[0 + (i * 4)] = amber;
                buffer[1 + (i * 4)] = red;
                buffer[2 + (i * 4)] = green;
                buffer[3 + (i * 4)] = blue;
                break;
            case AGRB:
                buffer[0 + (i * 4)] = amber;
                buffer[1 + (i * 4)] = green;
                buffer[2 + (i * 4)] = red;
                buffer[3 + (i * 4)] = blue;
                break;
            case ABGR:
                buffer[0 + (i * 4)] = amber;
                buffer[1 + (i * 4)] = blue;
                buffer[2 + (i * 4)] = green;
                buffer[3 + (i * 4)] = red;
                break;
            case ARBG:
                buffer[0 + (i * 4)] = amber;
                buffer[1 + (i * 4)] = red;
                buffer[2 + (i * 4)] = blue;
                buffer[3 + (i * 4)] = green;
                break;
            case AGBR:
                buffer[0 + (i * 4)] = amber;
                buffer[1 + (i * 4)] = green;
                buffer[2 + (i * 4)] = blue;
                buffer[3 + (i * 4)] = red;
                break;
            case ABRG:
                buffer[0 + (i * 4)] = amber;
                buffer[1 + (i * 4)] = blue;
                buffer[2 + (i * 4)] = red;
                buffer[3 + (i * 4)] = green;
                break;
            default:
                break;
        }
    }
}

