#include <UltrasonicSensor.h>

UltrasonicSensor ultrasonic(13, 14);

void setup() {
  Serial.begin(115200);
  ultrasonic.setTemperature(21); // ~70 degrees Fahrenheit
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
}

void loop() {
  int distance = ultrasonic.distanceInCentimeters();
  int button1 = digitalRead(18);
  int button2 = digitalRead(19);
  Serial.printf("Distance: %d cm\tA: %d\tB:%d\n", distance, button1, button2);
  delay(50); // Run at high speed to ensure we can detect brief "string-crossing"
}
