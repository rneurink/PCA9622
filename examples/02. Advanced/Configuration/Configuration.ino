/**
 * This example contains an application to configure the PCA9622. 
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

  // Device configuration before calling begin
  device.setI2CAddress(PCA9622_I2C_ADDRESS);
  device.setLEDConfiguration(RGB);
  device.setOutputEnablePin(OUTPUT_ENABLE_PIN);

  // Initialize the device
  device.begin();

  // Reset the device
  device.softwareReset();
  // After calling a software reset make sure to call Wakeup as the device will be in sleep mode

  // Sleep functions
  device.sleep();
  delay(10);
  device.wakeUp();
  delay(10);

  // Device configuration after calling begin
  // Set the sub addresses of the device
  device.setSubAddress1(0xD2);
  device.setSubAddress2(0xD4);
  device.setSubAddress3(0xD8);

  // Set the all call address of the device
  device.setAllCallAddress(0xD0);

  // Set specific configurations using the | sign. 
  device.configure(ALL_CALL_ON | SUB_1_ON | SUB_2_ON | SUB_3_ON);

  // Set output states
  device.setPWMOutputState(0, OFF); // Output is off
  device.setPWMOutputState(1, ON); // Output is (fully) on
  device.setPWMOutputState(2, PWM_CONTROL); // Output can be controlled using the pwm registers
  device.setPWMOutputState(3, PWM_AND_GROUP_CONTROL); // Output can be controlled using the pwm and group registers
  
  // Set the output state of output 4..7 (this function sets the output states according to the LEDOUT registers)
  device.setOutputState(1, PWM_AND_GROUP_CONTROL);

  // Set LED output states
  device.setLEDOutputState(3, ON); // LED 3. Outputs 9,10,11 if using RGB like configuration. With RGBA 8,9,10,11
  device.setLEDOutputState(4, PWM_CONTROL); // LED 4. Outputs 12,13,14 if using RGB like configuration. With RGBA 12,13,14,15

  device.setAllPWMOutputs(0x10); 

  // Enable group blinking
  device.enableGroupBlinking(); // Enables group blinking. Default is group dimming
  device.setGroupFrequency(100); // Blinking interval of 100ms
  device.setGroupPWM(128); // Blink dutycycle 50%

  // Enable the outputs (only used if an output enable pin has been specified)
  device.enableOutputs();
}

void loop() {
  //put your main code here, to run repeatedly:
  for (int i = 0; i <= 255; i++) {
    device.setAllPWMOutputs(i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    device.setAllPWMOutputs(i);
    delay(10);
  }
}