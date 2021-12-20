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
  device.begin(); // Also sets all outputs to PWM_AND_GROUP_CONTROL

  // Set the specific output control states
  device.setPWMOutputState(0, OFF); // Output 0 is off
  device.setPWMOutputState(1, ON); // Output 1 is fully on (no further control)
  device.setPWMOutputState(2, PWM_CONTROL); // Output 2 can be controlled using PWM but not using the group control
  device.setPWMOutputState(3, PWM_AND_GROUP_CONTROL); // Output 3 can be controlled using PWM and group control

  // Sets the output state per register of the device.
  device.setOutputState(1, OFF); // 0 sets 0,1,2,3; 1 sets 4,5,6,7 etc

  // Sets the output state per defined LED configuration
  device.setLEDOutputState(3, PWM_AND_GROUP_CONTROL); // If the configuration is RGB alike, 3 sets 9,10,11. If the configuration is RGBA alike, 3 sets 12,13,14,15.

  // Set the initial PWM outputs
  device.setAllPWMOutputs(0x80);

  // Enable the outputs (only used if an output enable pin has been specified)
  device.enableOutputs();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i <= 255; i++) {
    device.setGroupPWM(i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    device.setGroupPWM(i);
    delay(10);
  }
}