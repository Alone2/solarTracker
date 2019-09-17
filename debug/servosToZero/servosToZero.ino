#include <Servo.h>

const int SERVO_BASE_PIN =  10;
const int SERVO_TOP_PIN =  11;

Servo sBase;
Servo sTop;
void setup() {
  Serial.begin(9600);
  // Servos initalisiert
  sBase.attach(SERVO_BASE_PIN);
  sTop.attach(SERVO_TOP_PIN);

  // Servos auf 0etzen
  // 90 weil 90 grad (90 = 0)
  sBase.write(90);
  sTop.write(90);

  // 0.5 sek warten
  delay(500);

  // bye servo!
  sBase.detach();
  sTop.detach();
}

void loop() {
  
}
