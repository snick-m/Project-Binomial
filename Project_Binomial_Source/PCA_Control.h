#ifndef SERVO_H
#define SERVO_H

#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"

#define MIN_PULSE_WIDTH 500
#define MAX_PULSE_WIDTH 1950
#define FREQUENCY 50

class Servo {
  private:
    int _index, _low, _high;
    float _position = -1, _target;

    int pulseWidth(int angle) {
      int pulse_wide, analog_value;
      pulse_wide = map(constrain(angle, _low, _high), 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
      return analog_value;
    }

  public:
    Servo(int index, int low, int high);

    void write(int angle);
    void writeDelta(int dAngle) {
      _target = _position + dAngle;
    };
    void update();

    static void init_pca();
    int getIndex() {
      return _index;
    }
};

#endif
