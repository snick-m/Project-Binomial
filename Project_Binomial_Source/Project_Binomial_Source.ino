#include "PCA_Control.h"

Servo HipL = Servo(0, 0, 180); // Servo(slot, low_limit, high_limit);
Servo KneeL = Servo(1, 55, 90); // Servo(slot, low_limit, high_limit);


// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float x = 0, y = 0, z = 9.8, x_off = 0, y_off = 0, z_off = 0;

#define LOOP_FREQ 100
#define ZERO_PIN D6

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
 
  Servo::init_pca();

  pinMode(ZERO_PIN, INPUT_PULLUP);
  attachInterrupt(ZERO_PIN, setAccOffset, RISING);
}

void loop() {
  if (Serial.available()) {
    int cmd = Serial.parseInt();
    int idx = cmd / 1000;
    int val = cmd % 1000;

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
//    
//    KneeL.write(Serial.parseInt());
  }

  HipL.update();
  KneeL.update();


  // MPU Code

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /*
  
  // Used when Averaging n readings; Need to make it concurrent.
  
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
  
  */

  x = (a.acceleration.x + x_off) * 0.1 + x * 0.9;
  y = (a.acceleration.y + y_off) * 0.1 + y * 0.9;
  z = (a.acceleration.z + z_off) * 0.1 + z * 0.9;

  Serial.print("X: ");
  
//  if (abs(x) < 0.1) x = 0;

  Serial.print(x);

  float correction = x * -1 * 25;
  
  Serial.print(", Correction: ");
  Serial.println(correction);


//  Serial.println((int)(1000.0 / LOOP_FREQ));
//  delay((int)(1000.0 / LOOP_FREQ)); // Run Loop at roughly LOOP_FREQ


//  Serial.print("X: ");
//  Serial.print(x);
//  Serial.print(", Y: ");
//  Serial.print(y);
//  Serial.print(", Z: ");
//  Serial.print(z);
//
//  Serial.println("");
}

ICACHE_RAM_ATTR void setAccOffset() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.println("SET OFFSET");
  x_off = 0 - a.acceleration.x;
  y_off = 0 - a.acceleration.y;
  z_off = 9.8 - a.acceleration.z;
  x = 0;
  y = 0;
  z = 9.8;
  Serial.print(x_off);
  Serial.print("\t");
  Serial.print(y_off);
  Serial.print("\t");
  Serial.println(z_off);
}
