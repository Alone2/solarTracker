#include <Servo.h>

const int SERVO_BASE_PIN =  10;
const int SERVO_TOP_PIN =  11;

const int TOP_LIGHT = 3;
const int RIGHT_LIGHT = 2;
const int BOTTOM_LIGHT = 1;
const int LEFT_LIGHT = 0;

const int SOLAR = 4;

const float widerstand = 10;

const int  waitTimeSec =  60;

int direction_base = 180;
int direction_top = 90;
const float ok_diff = 0.004;

int shouldWaitTopCount = 0;
int shouldWaitBaseCount = 0;


Servo sBase;
Servo sTop;

void setup() {
  Serial.begin(9600);
  // Servos initalisiert
  sBase.attach(SERVO_BASE_PIN, 1000, 2000);
  sTop.attach(SERVO_TOP_PIN, 1000, 2000);

  delay(2000);
}

void loop() {
  //sTop für links - rechts
  float left = analogRead(LEFT_LIGHT);
  float right = analogRead(RIGHT_LIGHT);
  // kalibrieren
  float left_cal = left / 987.0;
  float right_cal = right / 872.0;
  // get direction of stuff
  if (abs(right_cal - left_cal) > ok_diff) {
    direction_top += right_cal > left_cal ? 1 : -1;
    shouldWaitBaseCount = 0;
  } else {
    shouldWaitBaseCount++;
  }
  
  //sBase für oben - unten
  float top = analogRead(TOP_LIGHT);
  float bottom = analogRead(BOTTOM_LIGHT) ;
  // kalibrieren
  float top_cal = top / 931.0;
  float bottom_cal = bottom / 1004.0;
  // get direction of stuff
  if (abs(top_cal - bottom_cal) > ok_diff) {
    direction_base += top_cal > bottom_cal ? 1 : -1;
    shouldWaitTopCount = 0;
  } else {
    shouldWaitTopCount++;
  }

 //Debug
 printSensors(top, top_cal, bottom, bottom_cal, right, right_cal, left, left_cal);
 Serial.println("------");
 printDirection();
 Serial.println("------");

  // Do not move in 0-180 degrees
  direction_top += direction_top > 180 ? -1 : 0;
  direction_top += direction_top < 0 ? 1 : 0;
  direction_base += direction_base > 180 ? -1 : 0;
  direction_base += direction_base < 0 ? 1 : 0;

  // move Servos
  sTop.write(direction_top);
  sBase.write(direction_base);

  // wait if sun found
  if (shouldWaitBaseCount > 3 && shouldWaitTopCount > 3) {
  //if (true) {
    sTop.detach();
    sBase.detach();
    Serial.println("SUN FOUND");
    
    for (int i = 0; i < waitTimeSec; i++){
      printURI();
      delay(1000);
    }
    shouldWaitBaseCount = 0;
    shouldWaitTopCount = 0;
    sBase.attach(SERVO_BASE_PIN, 1000, 2000);
    sTop.attach(SERVO_TOP_PIN, 1000, 2000);
  }

  //delay(1000);
  printURI();

}


void printURI() {
    // Print solar voltage;
  float messen = analogRead(SOLAR);
  // widerstand
  float R = 10;
  // 5 weil 5V arduino
  float U = 5 * messen / 1023.0;
  float I = U / widerstand;
  float P = U * I;
  Serial.print((String) "U: " + U + " V,  ");
  Serial.print((String) "I: " + I + " A,  ");
  Serial.println((String) "P: " + P*1000 + " mW");
}

void printSensors(float top, float top_cal, float bottom, float bottom_cal, float right, float right_cal, float left, float left_cal) {
  // Print debug stuff
   Serial.print(top_cal, 5);
   Serial.print(" ");
   Serial.print(right_cal, 5);
   Serial.print(" ");
   Serial.print(bottom_cal, 5);
   Serial.print(" ");
   Serial.println(left_cal, 5);

   Serial.print(top, 5); // 16, 947 -> 931
   Serial.print(" ");
   Serial.print(right,  5);  // 18, 890 -> 872
   Serial.print(" ");
   Serial.print(bottom, 5); // 6, 1010 ->  1004
   Serial.print(" ");
   Serial.println(left, 5); // 17, 1004 ->  987
}

void printDirection() {
   Serial.println(direction_top);
   Serial.println(direction_base);

}
