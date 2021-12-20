/**
 * This example contains an application which shows the use of group blinking on output 0 of the PCA9622
 * While blinking the example will pulse the output
 */

// Include the library
#include "PCA9622.h"

#define PCA9622_I2C_ADDRESS 0xA2 // NOTE: Make sure to use the correct I2C address as the PCA9622 can have 128 different addresses
#define OUTPUT_ENABLE_PIN 2 // The ~OE (Output Enable) pin of the device.

PCA9622 device(PCA9622_I2C_ADDRESS, OUTPUT_ENABLE_PIN); // Create a device object with the specified I2C_address and output enable pin

// If you don't have an enable pin use this device initializer instead
// PCA9622 device(PCA9622_I2C_ADDRESS);

/** 
 * ----------------------------------- WARNING ----------------------------------
 * if you or someone around you have problems with flickering lights due to 
 * photosensitive epilepsie please do not use this sketch !!!!!
 * ----------------------------------- WARNING ----------------------------------
 */ 

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  // Support for 400kHz is available. Comment this to use the default 100kHz
  Wire.setClock(400000UL);

  // Initialize the device
  device.begin();

  // Enable the outputs (only used if an output enable pin has been specified)
  device.enableOutputs();

  // Enable group blinking. This will blink all outputs which are controlled by the PWM_AND_GROUP_CONTROL. See example PartialGlobalControl for more information
  device.enableGroupBlinking();

  // Sets the blinking frequency in ms (42..10666ms is allowed)
  device.setGroupFrequency(100);
  
  // Sets the blinking duty cycle as a percentage. 
  // If the frequency is set to 100ms a value of 128 (half of 256) sets the output 50ms high and 50ms low
  // If set to 64 (a quarter of 256) the output will be 25ms high and 75ms low
  device.setGroupPWM(128);
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