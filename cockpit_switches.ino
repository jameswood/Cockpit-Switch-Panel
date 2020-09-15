#include <Wire.h>
char controllerState[8];
#define I2C_SLAVE_ADDR  0x07

const long throttle = 100;
long lastSend=0;

void setup(){
  for (int pinNumber = 0; pinNumber < 14; pinNumber++) { pinMode(pinNumber, INPUT_PULLUP); }
//  for (int pinNumber = 14; pinNumber <= 21; pinNumber++) { pinMode(pinNumber, INPUT); }
  pinMode(0+14, INPUT);
  Wire.begin();
//  Serial.begin(115200);
}

void loop(){
  controllerState[0] = map(analogRead(14), 0, 1023, 0, 255);
  controllerState[1] = 128; //map(analogRead(15), 0, 1023, 0, 255);
  controllerState[2] = 128; //map(analogRead(16), 0, 1023, 0, 255);
  controllerState[3] = 128; //map(analogRead(17), 0, 1023, 0, 255);
  controllerState[4] = 128; //map(analogRead(20), 0, 1023, 0, 255);
  controllerState[5] = 128; //map(analogRead(21), 0, 1023, 0, 255);
  controllerState[6] = readButtons(8, 0);
  controllerState[7] = readButtons(6, 8);
  if ((millis() - lastSend) > throttle ) {
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    for (int i=0; i<sizeof(controllerState); i++){
      Wire.write(controllerState[i]);
    }
    Wire.endTransmission();
    lastSend = millis();
  }
}

byte readButtons(int numberOfButtonsToRead, int inputNumberOffset){
  byte tempByte = 0;
  for (int i=0; i<8; i++){
    if (i < numberOfButtonsToRead) {
      tempByte += (digitalRead(i + inputNumberOffset) << i);
    } else {
      tempByte += (1 << i);
    }
  }
  return ~tempByte;
}
