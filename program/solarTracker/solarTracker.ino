#include <Servo.h>

const int SERVO_BASE_PIN =  10;
const int SERVO_TOP_PIN =  11;

const int TOP_LIGHT = 0;
const int RIGHT_LIGHT = 1;
const int BOTTOM_LIGHT = 2;
const int LEFT_LIGHT = 3;

int direction_base = 90;
int direction_top = 90;

Servo sBase;
Servo sTop;

void setup() {
  Serial.begin(9600);
  // Servos initalisiert
  sBase.attach(SERVO_BASE_PIN);
  sTop.attach(SERVO_TOP_PIN);

  // potential Error: Lichtsensoren sind anders?
  
}

void loop() {
  //sTop für links - rechts
  int left = analogRead(LEFT_LIGHT);
  int right = analogRead(RIGHT_LIGHT);
  Serial.println(left, right);
  if (left > right) {
    direction_top++;
    Serial.println("Top Servo -> Left");
  } else if  (left < right) {
    direction_top--;
    Serial.println("Top Servo -> Right");
  }
  
  //sBase für oben - unten
  int top = analogRead(TOP_LIGHT);
  int bottom = analogRead(TOP_LIGHT) ;
  Serial.println(top, bottom);
  if (top > bottom) {
    direction_base++;
    Serial.println("Base Servo -> Right");
  } else if (top < bottom) {
    direction_base--;
    Serial.println("Base Servo -> Right");
  }
  
  // move Servos
  sTop.write(direction_top);
  sBase.write(direction_base);
  
}
