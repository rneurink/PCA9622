#include "PCA9622.h"

#define OUTPUT_ENABLE_PIN 7

// Initialize the class with a specified I2C address and Output Enable pin
PCA9622 device(0xA2, OUTPUT_ENABLE_PIN);

void setup() {
    //Serial.begin(115200);

    Wire.begin();
    Wire.setClock(400000);

    device.begin();

    device.writeRegister(PCA9622_LED_OUT0, 0xAA);
    device.writeRegister(PCA9622_LED_OUT1, 0xAA);
    device.writeRegister(PCA9622_LED_OUT2, 0xAA);
    device.writeRegister(PCA9622_LED_OUT3, 0xAA);

    for(uint8_t i = 0; i <=16; i++) {
        device.writeRegister(PCA9622_PWM0 + i, 0x00);
        delay(10);
    }

    for(uint8_t i = 0; i <=16; i+=2) {
        device.writeRegister(PCA9622_PWM0 + i, 0x20);
        delay(10);
    }

    digitalWrite(7, LOW);
}

void loop() {

    
}