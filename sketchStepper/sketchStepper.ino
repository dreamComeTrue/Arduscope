/*
Created 2014 September 27
by Pavel Yakimenko
*/

#include <Stepper.h>

Stepper stepper(64, 8, 10, 9, 11);

const int thumbX = A0;
const int thumbY = A1;
const int thumbSwitch = 2;

const int ledX = 5;
const int ledY = 6;
const int ledSwitch = 7;

int axisMin = 0;
int axisMax = 512;
int axisNeutralGap = 10;

int axisXCenter = 512;
int axisYCenter = 512;

// the setup functionruns once when you press reset or power the board
void setup() {
  Serial.begin(9600);
    
  pinMode(thumbX, INPUT);
  pinMode(thumbY, INPUT);
  pinMode(thumbSwitch, INPUT);
  
  pinMode(ledX, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledSwitch, OUTPUT);
  
  stepper.setSpeed(100);

  calibrate();
}

void calibrate() {
  axisXCenter = analogRead(thumbX);
  axisYCenter = analogRead(thumbY);
}

int axisValue(const int axisPin, const int ledPin, const int neutral) {
  int sensorValue = neutral - analogRead(axisPin);
  if (sensorValue < axisNeutralGap &&
      sensorValue > -axisNeutralGap) {
    sensorValue = 0;
  }
  
  int ledBrightness = sensorValue;
  if(ledBrightness < 0) {
    ledBrightness *= -1;
  }
  ledBrightness = map(ledBrightness, axisMin, axisMax, 0x0, 0xFF);
  ledBrightness = constrain(ledBrightness, 0x0, 0xFF);
  Serial.println(ledBrightness);
  analogWrite(ledPin, ledBrightness);
  return sensorValue;
}

void displaySwitchState() {
  int switchState = digitalRead(thumbSwitch);
  digitalWrite(ledSwitch, switchState ? HIGH : LOW);
}

void loop() {
  int xValue = axisValue(thumbX, ledX, axisXCenter);
  int yValue = axisValue(thumbY, ledY, axisYCenter);

  displaySwitchState();

  Serial.println(xValue);
  stepper.step(xValue/100);
}
