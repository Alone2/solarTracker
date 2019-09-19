#include <Servo.h>

const int SERVO_BASE_PIN =  10;
const int SERVO_TOP_PIN =  11;

const int TOP_LIGHT = 3;
const int RIGHT_LIGHT = 2;
const int BOTTOM_LIGHT = 1;
const int LEFT_LIGHT = 0;

Servo sBase;
Servo sTop;
void setup() {
  Serial.begin(9600);
  // Servos initalisiert
  sBase.attach(SERVO_BASE_PIN);
  sTop.attach(SERVO_TOP_PIN);

  // Servos auf 0etzen
  // 90 weil 90 grad (90 = 0)
  sBase.write(180);
  sTop.write(90);

  // 0.5 sek warten
  delay(500);

  // bye servo!
  sBase.detach();
  sTop.detach();
}

void loop() {
  float top = analogRead(TOP_LIGHT);
  float bottom = analogRead(BOTTOM_LIGHT) ;
    float left = analogRead(LEFT_LIGHT);
  float right = analogRead(RIGHT_LIGHT);

    // kalibrieren
  float top_cal = top / 931.0;
  float bottom_cal = bottom / 1004.0;
    float left_cal = left / 987.0;
  float right_cal = right / 872.0;
  
   // Print debug stuff
   Serial.print(top_cal, 5);
   Serial.print(" ");
   Serial.print(right_cal, 5);
   Serial.print(" ");
   Serial.print(bottom_cal, 5);
   Serial.print(" ");
   Serial.println(left_cal, 5);

   Serial.print(top, 5); 
   Serial.print(" ");
   Serial.print(right,  5);
   Serial.print(" ");
   Serial.print(bottom, 5); 
   Serial.print(" ");
   Serial.println(left, 5);

   delay(1000);
  
}
