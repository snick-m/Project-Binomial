#include "PCA_Control.h"

Servo HipL = Servo(0, 55, 90); // Servo(slot, low_limit, high_limit);
Servo KneeL = Servo(1, 55, 90); // Servo(slot, low_limit, high_limit);


// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float x = 0, y = 0, z = 9.8;


void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

//  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("");
  delay(100);
  
  Servo::init_pca();

  Serial.println(KneeL.getIndex());
  Serial.println(HipL.getIndex());
}

void loop() {
  if (Serial.available()) {
    int cmd = Serial.parseInt();
    int idx = cmd / 100;
    int val = cmd % 100;

    Serial.print(idx);
    Serial.print(' ');
    Serial.println(val);

    switch(idx) {
      case 0:
        HipL.write(val);
        break;
      case 1:
        KneeL.write(val);
        break;
    }
    
//    KneeL.write(Serial.parseInt());
  }


  // MPU Code

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;

  float x_sum, y_sum, z_sum;
  
  for (int i = 0; i < 10; i++) {
    mpu.getEvent(&a, &g, &temp);
  
    x = a.acceleration.x * 0.2 + x * 0.8;
    y = a.acceleration.y * 0.2 + y * 0.8;
    z = a.acceleration.z * 0.2 + z * 0.8;

    x_sum += x;
    y_sum += y;
    z_sum += z;

    delay(5);
  }

  x = x_sum / 10;
  y = y_sum / 10;
  z = z_sum / 10;
//
//  Serial.print("X: ");
//  Serial.print(x);
//  Serial.print(", Y: ");
//  Serial.print(y);
//  Serial.print(", Z: ");
//  Serial.print(z);
//
//  Serial.println("");
}
