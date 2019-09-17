#include <Servo.h>

const int SERVO_BASE_PIN =  10;
const int SERVO_TOP_PIN =  11;

const int TOP_LIGHT = 0;
const int BOTTOM_LIGHT = 1;
const int LEFT_LIGHT = 2;
const int RIGHT_LIGHT = 3;

int direction_base = 90;
int direction_top = 90;

Servo sBase;
Servo sTop;

void setup() {
  Serial.begin(9600);
  // Servos initalisiert
  sBase.attach(SERVO_BASE_PIN);
  sTop.attach(SERVO_TOP_PIN);
}

void loop() {
  //sTop für links - rechts
  if (analogRead(LEFT_LIGHT) > analogRead(RIGHT_LIGHT)) {
    direction_top++;
    Serial.println("Top Servo -> Left");
  } else {
    direction_top--;
    Serial.println("Top Servo -> Right");
  }
  
  //sBase für oben - unten
  if (analogRead(TOP_LIGHT) > analogRead(BOTTOM_LIGHT)) {
    direction_base++;
    Serial.println("Base Servo -> Right");
  } else {
    direction_base--;
    Serial.println("Base Servo -> Right");
  }
  
  // move Servos
  sTop.move(direction_top);
  sBase.move(direction_base);
  
}
