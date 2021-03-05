#include <UltrasonicSensor.h>

UltrasonicSensor ultrasonic(13, 14);

void setup() {
  Serial.begin(115200);
  ultrasonic.setTemperature(21); // ~70 degrees Fahrenheit
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(15, INPUT);
}

void loop() {
  int distance = ultrasonic.distanceInCentimeters();
  int button1 = digitalRead(18); // Yellow button
  int button2 = digitalRead(19); // Blue button

  int pir = digitalRead(15); // Infrared sensor (jumper set to high for repeatable trigger mode), time set to minimum (3s) for smooth but responsive behavior
  
  Serial.printf("Distance: %d cm\tA: %d\tB:%d\tIR:%d\n", distance, button1, button2, pir);
  delay(50); // Run at high speed to ensure we can detect brief "string-crossing"
}
