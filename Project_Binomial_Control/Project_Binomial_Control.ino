// Custom Imports
#include "Control_Server.h"

void setup() {
  Serial.begin(115200);
  Serial.println("BEGIN");
  
  controlSetup();
  
  Serial.println("\nCLEAR\n\n");
}

void loop() {

  controlLoop();
}
