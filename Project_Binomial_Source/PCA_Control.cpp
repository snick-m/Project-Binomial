#include "Arduino.h"
#include "PCA_Control.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void Servo::init_pca() {
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
}

Servo::Servo(int index, int low, int high) {
  _index = index;
  _low = low;
  _high = high;
}

void Servo::write(int angle) {
  pwm.setPWM(_index, 0, pulseWidth(angle));
}
