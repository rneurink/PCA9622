#include "PCA9622.h"

#define OUTPUT_ENABLE_PIN 7

// Initialize the class with a specified I2C address and Output Enable pin
PCA9622 device(0xA2, OUTPUT_ENABLE_PIN);

void setup() {
    //Serial.begin(115200);

    pinMode(17, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(17), sw_reset, FALLING);

    Wire.begin();
    Wire.setClock(400000);

    device.begin();

    device.configure(ALL_CALL_ON | SUB_3_ON | SUB_2_ON | SUB_1_ON);

    device.writeRegister(PCA9622_LED_OUT0, 0xFF);
    device.writeRegister(PCA9622_LED_OUT1, 0xFF);
    device.writeRegister(PCA9622_LED_OUT2, 0xFF);
    device.writeRegister(PCA9622_LED_OUT3, 0xFF);

/* R 0 3 6 9
 * G 1 4 7 10
 * B 2 5 8 11
 */
    for (int j = 2; j < 16; j+=3) {
        device.setPWMOutput(j, 0x20, AllCall);
    }

    device.enableOutputs();
}

void loop() {
    for (int i = 0; i < 0x20; i++) {
        for (int j = 0; j < 16; j+=3) {
            device.setPWMOutput(j, i);
        }
        delay(10);
    }
    for (int i = 0x20; i >= 0; i--) {
        for (int j = 2; j < 16; j+=3) {
            device.setPWMOutput(j, i);
        }
        delay(10);
    }
    for (int i = 0; i < 0x20; i++) {
        for (int j = 1; j < 16; j+=3) {
            device.setPWMOutput(j, i);
        }
        delay(10);
    }
    for (int i = 0x20; i >= 0; i--) {
        for (int j = 0; j < 16; j+=3) {
            device.setPWMOutput(j, i);
        }
        delay(10);
    }
    for (int i = 0; i < 0x20; i++) {
        for (int j = 2; j < 16; j+=3) {
            device.setPWMOutput(j, i);
        }
        delay(10);
    }
    for (int i = 0x20; i >= 0; i--) {
        for (int j = 1; j < 16; j+=3) {
            device.setPWMOutput(j, i);
        }
        delay(10);
    }
}

void sw_reset() {
  _PROTECTED_WRITE(RSTCTRL.SWRR,1);
}