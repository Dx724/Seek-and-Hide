#include <UltrasonicSensor.h>

UltrasonicSensor ultrasonic(13, 14);

void setup() {
  Serial.begin(115200);
  ultrasonic.setTemperature(21); // ~70 degrees Fahrenheit
}

void loop() {
  int distance = ultrasonic.distanceInCentimeters();
  delay(50); // Run at high speed to ensure we can detect brief "string-crossing"
}
