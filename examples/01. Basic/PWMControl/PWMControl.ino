/**
 * This example contains a simple application to pulse output 0 of the PCA9622
 */

// Include the library
#include "PCA9622.h"

#define PCA9622_I2C_ADDRESS 0xA2 // NOTE: Make sure to use the correct I2C address as the PCA9622 can have 128 different addresses
#define OUTPUT_ENABLE_PIN 2 // The ~OE (Output Enable) pin of the device.

PCA9622 device(PCA9622_I2C_ADDRESS, OUTPUT_ENABLE_PIN); // Create a device object with the specified I2C_address and output enable pin

// If you don't have an enable pin use this device initializer instead
// PCA9622 device(PCA9622_I2C_ADDRESS);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  // Support for 400kHz is available. Comment this to use the default 100kHz
  Wire.setClock(400000UL);

  // Initialize the device
  device.begin();

  // Enable the outputs (only used if an output enable pin has been specified)
  device.enableOutputs();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i <= 255; i++) {
    device.setPWMOutput(0, i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    device.setPWMOutput(0, i);
    delay(10);
  }
}