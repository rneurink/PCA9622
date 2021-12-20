/**
 * This example contains an application to pulse output 0 of the PCA9622 using broadcasts
 * This example is only interesting if you have multiple PCA9622 devices
 */

// Include the library
#include "PCA9622.h"

#define PCA9622_I2C_ADDRESS_1 0xA2 // NOTE: Make sure to use the correct I2C address as the PCA9622 can have 128 different addresses
#define PCA9622_I2C_ADDRESS_2 0xA4 // NOTE: Make sure to use the correct I2C address as the PCA9622 can have 128 different addresses
#define OUTPUT_ENABLE_PIN_1 2 // The ~OE (Output Enable) pin of the device.
#define OUTPUT_ENABLE_PIN_2 3 // The ~OE (Output Enable) pin of the device.

PCA9622 device1(PCA9622_I2C_ADDRESS_1, OUTPUT_ENABLE_PIN_1); // Create a device object with the specified I2C_address and output enable pin
PCA9622 device2(PCA9622_I2C_ADDRESS_2, OUTPUT_ENABLE_PIN_2); // Create a second device object with the specified I2C_address and output enable pin

// If you don't have an enable pin use this device initializer instead
// PCA9622 device(PCA9622_I2C_ADDRESS_1);
// PCA9622 device(PCA9622_I2C_ADDRESS_2);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  // Support for 400kHz is available. Comment this to use the default 100kHz
  //Wire.setClock(400000UL);

  // Initialize the device
  device1.begin();
  device2.begin();

  // Enable the outputs (only used if an output enable pin has been specified)
  device1.enableOutputs();
  device2.enableOutputs();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i <= 255; i++) {
    // Possible options are 'AllCall', 'SubCall1', 'SubCall2', and 'SubCall3' 
    // AllCall is enabled by default. To use the SubCall check out example Configuration.ino
    device1.setPWMOutput(0, i, AllCall); 
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    device1.setPWMOutput(0, i, AllCall);
    delay(10);
  }
}