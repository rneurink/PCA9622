/**
 * This example contains a simple application to rainbow cycle the RGB colors of LED 0 (Output 0..2 for RGB and 0..3 for RGBA like LEDs)
 */

// Include the library
#include "PCA9622.h"

#define PCA9622_I2C_ADDRESS 0xA2 // NOTE: Make sure to use the correct I2C address as the PCA9622 can have 128 different addresses
#define OUTPUT_ENABLE_PIN 2 // The ~OE (Output Enable) pin of the device.

// NOTE: Pass RGB, GRB or RGBA GRBA (or any of the combinations) to the constructor of the device according to the LED you use.
// For RGB like LEDs the PCA9622 can handle 5 LEDs, for RGBA like LEDs the PCA9622 can handle 4 LEDs
PCA9622 device(PCA9622_I2C_ADDRESS, OUTPUT_ENABLE_PIN, RGB); 

// If you don't have an enable pin use this device initializer instead
// PCA9622 device(PCA9622_I2C_ADDRESS, RGB);

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
  Rainbow();
}

/**
 * @brief Rainbow on LED 0
 */
void Rainbow() {
  for (int i = 0; i < 256; i++) {
    byte wheel = 255 - i;
    if(wheel < 85) {
      device.setLEDColor(0, 255 - wheel * 3, 0, wheel * 3);
    } else if(wheel < 170) {
      wheel -= 85;
      device.setLEDColor(0, 0, wheel * 3, 255 - wheel * 3);
    } else {
      wheel -= 170;
      device.setLEDColor(0, wheel * 3, 255 - wheel * 3, 0);
    }
    delay(20);
  }
}