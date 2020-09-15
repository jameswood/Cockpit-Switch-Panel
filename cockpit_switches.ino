#include <EasyTransfer.h>

#define BUTTONS 16
#define FAUXBUTTONS 5

const long speedLimit = 10;

unsigned long lastSend = 0;

int buttonPins[BUTTONS] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3 };
int analogButtonPin = A4;

EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t joyx;
  int16_t joyy;
  int8_t joyz;
  int16_t joyxr;
  int16_t joyyr;
  int8_t joyzr;
  uint32_t buttonState;
};

SEND_DATA_STRUCTURE controllerState;

void setup() {
  Serial.begin(2000000);
  ET.begin(details(controllerState), &Serial);
  for (int i = 0; i < BUTTONS; i++) { pinMode(buttonPins[i], INPUT_PULLUP); }
  pinMode(analogButtonPin, INPUT);
}

uint32_t readButtons(){
  uint32_t buttonStateNow = 0;
  for (int i=0; i<BUTTONS; i++) {
    bitWrite(buttonStateNow, i, !digitalRead(buttonPins[i]));
  }
  int fauxButton = analogRead(analogButtonPin) / 205;
  for (int i=0; i<FAUXBUTTONS; i++) {
    bool buttonPressed = false;
    fauxButton == i ? buttonPressed = true : buttonPressed = false;
    bitWrite(buttonStateNow, i + BUTTONS, buttonPressed);
  }
  return buttonStateNow;
}

void loop(){
  controllerState.buttonState = readButtons();
  if ((millis() - lastSend) > speedLimit ) {
    ET.sendData();
    lastSend = millis();
  }
}
