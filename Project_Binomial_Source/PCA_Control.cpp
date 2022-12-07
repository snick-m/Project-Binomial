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
//  pwm.setPWM(_index, 0, pulseWidth(_low));
}

void Servo::write(int angle) {
  if (_position == -1) {
    for (int i = 1; i <= 50; i++) {
      int val = (float)angle * -(cos(PI * (float)i / 50) - 1) / 2;
      pwm.setPWM(_index, 0, pulseWidth(val));
      delay(1);
    }
    
    _position = angle;
    _target = angle;
    return;
  }

  _target = angle; // Set Target for Update loop.
}

void Servo::update() { // Update servo position on main loop.
  if (_position != _target) {
    _position = (int)round(_target * 0.035 + _position * 0.965); // 3.5% of Target, 96.5% of Current
//    char buff[30];
//    sprintf(buff, "Target: %2f\tPosition: %2f\tDelta: %2f", _target, _position, _target * 0.1 + _position * 0.9);
//    Serial.println(buff);
    pwm.setPWM(_index, 0, pulseWidth(_position));
    delay(10); // TODO: Decide on Loop Frequency handling in library vs main loop.
  }
}
