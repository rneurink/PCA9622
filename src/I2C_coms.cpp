/**
 * @file I2C_coms.cpp
 * @author rneurink (ruben.neurink@gmail.com)
 * @brief Contains i2c implementation of the platform
 * @version 1.0.0
 * @date 2021-06-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "I2C_coms.h"

//#define I2C_DEBUG

int8_t i2c_init() {
    //Wire.begin(); Best to pull this out of the library
    return 0;
}

int8_t i2c_write_multi(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *pdata, uint32_t count) {
    Wire.beginTransmission(((deviceAddress) >> 1) & 0x7F);
    Wire.write(registerAddress);
#ifdef I2C_DEBUG
    Serial.print("\tWriting "); Serial.print(count); Serial.print(" to addr 0x"); Serial.print(registerAddress, HEX); Serial.print(": ");
#endif
    while(count--) {
        Wire.write((uint8_t)pdata[0]);
#ifdef I2C_DEBUG
        Serial.print("0x"); Serial.print(pdata[0], HEX); Serial.print(", ");
#endif
        pdata++;
    }
#ifdef I2C_DEBUG
    Serial.println();
#endif
    return Wire.endTransmission();
}

int8_t i2c_read_multi(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *pdata, uint32_t count){
    Wire.beginTransmission(((deviceAddress) >> 1) & 0x7F);
    Wire.write(registerAddress);
    Wire.endTransmission(false); // Dont send a stop bit
    Wire.requestFrom(((deviceAddress) >> 1) & 0x7F, count);
#ifdef I2C_DEBUG
    Serial.print("\tReading "); Serial.print(count); Serial.print(" from addr 0x"); Serial.print(registerAddress, HEX); Serial.print(": ");
#endif

    while (count--) {
        pdata[0] = Wire.read();
#ifdef I2C_DEBUG
        Serial.print("0x"); Serial.print(pdata[0], HEX); Serial.print(", ");
#endif
        pdata++;
    }
#ifdef I2C_DEBUG
    Serial.println();
#endif
    return 0;
}

int8_t i2c_write_byte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) {
    return i2c_write_multi(deviceAddress, registerAddress, &data, 1);
}

// int8_t i2c_write_word(uint8_t deviceAddress, uint8_t registerAddress, uint16_t data) {
//     uint8_t buff[2];
//     buff[1] = data & 0xFF;
//     buff[0] = data >> 8;
//     return i2c_write_multi(deviceAddress, registerAddress, buff, 2);
// }

// int8_t i2c_write_Dword(uint8_t deviceAddress, uint8_t registerAddress, uint32_t data) {
//     uint8_t buff[4];

//     buff[3] = data & 0xFF;
//     buff[2] = data >> 8;
//     buff[1] = data >> 16;
//     buff[0] = data >> 24;

//     return i2c_write_multi(deviceAddress, registerAddress, buff, 4);
// }

int8_t i2c_read_byte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data) {
    return i2c_read_multi(deviceAddress, registerAddress, data, 1);
}

// int8_t i2c_read_word(uint8_t deviceAddress, uint8_t registerAddress, uint16_t *data) {
//     uint8_t buff[2];
//     int r = i2c_read_multi(deviceAddress, registerAddress, buff, 2);

//     uint16_t tmp;
//     tmp = buff[0];
//     tmp <<= 8;
//     tmp |= buff[1];
//     *data = tmp;

//     return r;
// }

// int8_t i2c_read_Dword(uint8_t deviceAddress, uint8_t registerAddress, uint32_t *data) {
//     uint8_t buff[4];
//     int r = i2c_read_multi(deviceAddress, registerAddress, buff, 4);

//     uint32_t tmp;
//     tmp = buff[0];
//     tmp <<= 8;
//     tmp |= buff[1];
//     tmp <<= 8;
//     tmp |= buff[2];
//     tmp <<= 8;
//     tmp |= buff[3];

//     *data = tmp;

//     return r;
// }
