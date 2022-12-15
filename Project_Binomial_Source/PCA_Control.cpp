#include "Arduino.h"
#include "PCA_Control.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

void Servo::init_pca() {
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
}

Servo::Servo(int index, int low, int high, int initial) {
  _index = index;
  _low = low;
  _high = high;
  _initial = initial;
  //  pwm.setPWM(_index, 0, pulseWidth(_low));
}

void Servo::write(int angle) {
  //  if (_position == -1) {
  //    for (int i = 1; i <= 50; i++) {
  //      int val = (float)angle * -(cos(PI * (float)i / 50) - 1) / 2;
  //      pwm.setPWM(_index, 0, pulseWidth(val));
  //      delay(20);
  //    }
  //
  //    _position = angle;
  //    _target = angle;
  //    return;
  //  }

  _target = constrain(angle, _low, _high); // Set Target for Update loop.
}

void Servo::update() { // Update servo position on main loop.
  if (_position == -1) {
    _target = _initial;
    _position = 0;
  }
  if (_position != _target) {
    _position = (int)round(_target * 0.07 + _position * 0.93); // 3.5% of Target, 96.5% of Current

#ifdef SERVO_D
    char buff[30];
    sprintf(buff, "Servo: %d\tTarget: %2f\tPosition: %2f\tDelta: %2f", _index, _target, _position, _target * 0.1 + _position * 0.9);
    Serial.println(buff);
#endif

    pwm.setPWM(_index, 0, pulseWidth(_position));
    delay(10); // TODO: Decide on Loop Frequency handling in library vs main loop.
  }
}
