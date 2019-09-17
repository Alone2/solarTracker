#include <Servo.h>

const int SERVO_BASE_PIN =  10;
const int SERVO_TOP_PIN =  11;

const int TOP_LIGHT = 3;
const int RIGHT_LIGHT = 2;
const int BOTTOM_LIGHT = 1;
const int LEFT_LIGHT = 0;

int direction_base = 90;
int direction_top = 90;
const float ok_diff = 0.01;

Servo sBase;
Servo sTop;

void setup() {
  Serial.begin(9600);
  // Servos initalisiert
  sBase.attach(SERVO_BASE_PIN, 1000, 2000);
  sTop.attach(SERVO_TOP_PIN, 1000, 2000);

  delay(2000);

  // debug
  float left = analogRead(LEFT_LIGHT);
  float right = analogRead(RIGHT_LIGHT);
  float top = analogRead(TOP_LIGHT);
  float bottom = analogRead(BOTTOM_LIGHT) ;

  Serial.print(top);
  Serial.print(" ");
  Serial.print(right);
   Serial.print(" ");
   Serial.print(bottom);
   Serial.print(" ");
   Serial.println(left);
  
}

void loop() {
  //sTop für links - rechts
  float left = analogRead(LEFT_LIGHT);
  float right = analogRead(RIGHT_LIGHT);
  // kalibrieren
  float left_cal = left / 996;
  float right_cal = right / 994;
  // get direction of stuff
  if (abs(right_cal - left_cal) > ok_diff) {
    direction_top += right_cal > left_cal ? 1 : -1;
  }
  
  //sBase für oben - unten
  float top = analogRead(TOP_LIGHT);
  float bottom = analogRead(BOTTOM_LIGHT) ;
  // kalibrieren
  float top_cal = top / 986;
  float bottom_cal = bottom / 1001;
  // get direction of stuff
  if (abs(top_cal - bottom_cal) > ok_diff) {
    direction_base += top_cal > bottom_cal ? 1 : -1;
  }

   // Print debug stuff
   Serial.print(top_cal);
   Serial.print(" ");
   Serial.print(right_cal);
   Serial.print(" ");
   Serial.print(bottom_cal);
   Serial.print(" ");
   Serial.println(left_cal);

   Serial.print(top);
   Serial.print(" ");
   Serial.print(right);
   Serial.print(" ");
   Serial.print(bottom);
   Serial.print(" ");
   Serial.println(left);

   Serial.println("-------");

   Serial.println(direction_top);
   Serial.println(direction_base);

   Serial.println("-------");
  
  // Do not move in 0-180 degrees
  if (direction_top > 180) {
    direction_top--;
  } else if (direction_top < 0) {
    direction_top++;
  }
  
  // move Servos
  sTop.write(direction_top);
  sBase.write(direction_base);

   //delay(1000);
  
}
