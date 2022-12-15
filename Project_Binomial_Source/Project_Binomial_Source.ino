// Debug modes

const static bool SERVO_D = true;
//const static bool MPU_D = true;

#include <Wire.h>

// Custom Imports
#include "PCA_Control.h"
//#include "Control_Server.h"

// MPU Imports
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Joint Definitions : Servo(slot_index, low_limit, high_limit)

Servo HipR = Servo(0, 0, 170, 120); // 0-170
Servo KneeR = Servo(1, 0, 180, 100); // 0-180
Servo HipL = Servo(2, 0, 170, 0); // 0-170
Servo KneeL = Servo(3, 0, 180, 0); // 0-180

Adafruit_MPU6050 mpu;

float x = 0, y = 0, z = 9.8, x_off = 0, y_off = 0, z_off = 0; // accelerometer value stores and thresholds

#define LOOP_FREQ 100
#define ZERO_PIN 0

void setup() {
  Serial.begin(115200);
  Serial.println("BEGIN");
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  //  Serial.println("Adafruit MPU6050 test!");

//  controlSetup();

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

  pinMode(ZERO_PIN, INPUT_PULLUP);
  attachInterrupt(ZERO_PIN, setAccOffset, RISING);
  
  Servo::init_pca();

  delay(500); // Skip through initial Serial burst from Nodemcu
  Serial.println("\nCLEAR\n\n");
}

void loop() {
  if (Serial.available()) {
    int idx = Serial.read() - 48;
    int val = 0;

    while (Serial.available()) {
      val *= 10;
      val += Serial.read() - 48;
    }
    
    switch (idx) {
      case 2:
        Serial.print("HL");

        HipL.write(val);
        break;
      case 3:
        Serial.print("KL");

        KneeL.write(val);
        break;
      case 0:
        Serial.print("HR");

        HipR.write(val);
        break;
      case 1:
        Serial.print("KR");

        KneeR.write(val);
        break;
    }

    Serial.print(' ');
    Serial.println(val);
    //
    //    KneeL.write(Serial.parseInt());
  }

  HipL.update();
  KneeL.update();
  HipR.update();
  KneeR.update();


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

  float correction = x * -1 * 25;

//  controlLoop();

#ifdef MPU_D

  Serial.print("X: ");

  Serial.print(x);

  Serial.print(", Correction: ");
  Serial.println(correction);

#endif


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
