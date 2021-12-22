#include "PCA9622.h"

#define OUTPUT_ENABLE_PIN 2

// Initialize the class with a specified I2C address and Output Enable pin
PCA9622 device(0xA2, OUTPUT_ENABLE_PIN, GRB);

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(400000UL);

  // Device configuration before calling begin
  device.setI2CAddress(0xA2);
  device.setLEDConfiguration(RGB);
  device.setOutputEnablePin(OUTPUT_ENABLE_PIN);

  device.softwareReset();

  delay(100);

  device.begin();

  device.sleep();
  uint8_t buffer = device.readRegister(PCA9622_MODE1);
  Serial.println("Mode1: 0x" + String(buffer, HEX));

  device.wakeUp();
  delay(10);
  buffer = device.readRegister(PCA9622_MODE1);
  Serial.println("Mode1: 0x" + String(buffer, HEX));

  device.configure(ALL_CALL_ON | SUB_3_ON | SUB_2_ON | SUB_1_ON);
  buffer = device.readRegister(PCA9622_MODE1);
  Serial.println("Mode1: 0x" + String(buffer, HEX));

  buffer = device.readRegister(PCA9622_SUB_ADR1);
  Serial.println("Sub1: 0x" + String(buffer, HEX));
  device.setSubAddress1(0xD2);
  buffer = device.readRegister(PCA9622_SUB_ADR1);
  Serial.println("Sub1: 0x" + String(buffer, HEX));

  buffer = device.readRegister(PCA9622_SUB_ADR2);
  Serial.println("Sub2: 0x" + String(buffer, HEX));
  device.setSubAddress2(0xD4);
  buffer = device.readRegister(PCA9622_SUB_ADR2);
  Serial.println("Sub2: 0x" + String(buffer, HEX));

  buffer = device.readRegister(PCA9622_SUB_ADR3);
  Serial.println("Sub3: 0x" + String(buffer, HEX));
  device.setSubAddress3(0xD8);
  buffer = device.readRegister(PCA9622_SUB_ADR3);
  Serial.println("Sub3: 0x" + String(buffer, HEX));

  buffer = device.readRegister(PCA9622_ALL_CALL);
  Serial.println("Allcall: 0x" + String(buffer, HEX));
  device.setAllCallAddress(0xD0);
  buffer = device.readRegister(PCA9622_ALL_CALL);
  Serial.println("Allcall: 0x" + String(buffer, HEX));

  uint8_t bufferArray[4];
  device.readMultiRegister(PCA9622_LED_OUT0 | PCA9622_AI_ALL, bufferArray, 4);
  Serial.println("LED_OUT: 0x" + String(((uint32_t)bufferArray[0] & 0xFF) | (((uint32_t)bufferArray[1] << 8) & 0xFF00) | (((uint32_t)bufferArray[2] << 16) & 0xFF0000) | (((uint32_t)bufferArray[3] << 24) & 0xFF000000), HEX));
  
  device.setLEDOutputState(0, OFF);
  device.setLEDOutputState(1, ON);
  device.setLEDOutputState(2, PWM_AND_GROUP_CONTROL);
  device.setOutputState(3, ON);
  device.setPWMOutputState(15, PWM_CONTROL);
  device.readMultiRegister(PCA9622_LED_OUT0 | PCA9622_AI_ALL, bufferArray, 4);
  Serial.println("LED_OUT: 0x" + String(((uint32_t)bufferArray[0] & 0xFF) | (((uint32_t)bufferArray[1] << 8) & 0xFF00) | (((uint32_t)bufferArray[2] << 16) & 0xFF0000) | (((uint32_t)bufferArray[3] << 24) & 0xFF000000), HEX));

  device.enableOutputs();

  device.setPWMOutput(15, 0xFF);
 
  device.setAllLEDColor(0x3F,0x00,0x00);
  delay(500);
  device.setAllLEDColor(0x00,0x3F,0x00);
  delay(500);
  device.setAllLEDColor(0x00,0x00,0x3F);
  delay(500);
  for (int i = 0; i < 255; i++) {
    device.setGroupPWM(i);
    delay(5);
  }

  device.enableGroupBlinking();
  device.setGroupFrequency(100);
  device.setGroupPWM(128);
  delay(500);
  device.enableGroupDimming();
}

void loop() {
  Rainbow();
}

void Rainbow() {
  for (int i = 0; i < 256; i++) {
    device.setPWMOutput(15, i);
    byte wheel = 255 - i;
    if(wheel < 85) {
      device.setAllLEDColor(255 - wheel * 3, 0, wheel * 3);
    } else if(wheel < 170) {
      wheel -= 85;
      device.setAllLEDColor(0, wheel * 3, 255 - wheel * 3);
    } else {
      wheel -= 170;
      device.setAllLEDColor(wheel * 3, 255 - wheel * 3, 0);
    }
    delay(20);
  }
}