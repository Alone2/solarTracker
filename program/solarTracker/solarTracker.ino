#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> 
#include <Servo.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

static String NAME = "solarTracker V1.4";

static int SERVO_BASE_PIN =  10;
static int SERVO_TOP_PIN =  11;

static int TOP_LIGHT = 3;
static int RIGHT_LIGHT = 2;
static int BOTTOM_LIGHT = 1;
static int LEFT_LIGHT = 0;

static int PEEP = 8;

// OLED display TWI address
#define OLED_ADDR   0x3C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
// BMP sensor
#define BMP280_ADDR 0x76

const float widerstand = 10;

const int  waitTimeSec =  60;

int direction_base = 180;
int direction_top = 90;
const float ok_diff = 0.003;

int shouldWaitTopCount = 0;
int shouldWaitBaseCount = 0;

Servo sBase;
Servo sTop;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_INA219 ina219;
Adafruit_BMP280 bmp;

int goUp = 0;

int loopI = 0;

void setup() {
  Serial.begin(9600);
  // Servos initalisiert
  sBase.attach(SERVO_BASE_PIN, 1000, 2000);
  sTop.attach(SERVO_TOP_PIN, 1000, 2000);
  //Display initialisiert
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(NAME);
  display.display();
  // Measure device
   ina219.begin();
   ina219.setCalibration_16V_400mA();
   // temp. sensor
   bmp.begin(BMP280_ADDR);
   bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, 
                  Adafruit_BMP280::SAMPLING_X2,  
                  Adafruit_BMP280::SAMPLING_X16, 
                  Adafruit_BMP280::FILTER_X16,    
                  Adafruit_BMP280::STANDBY_MS_500); 
                  
  //wait
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
     //disable motors
    sTop.detach();
    sBase.detach();
    // play tone
    tone(PEEP, 200);
    Serial.println("SUN FOUND");
    // display on display
    bigDisplay("SUN FOUND!");
    delay(500);
    //stop playing tone
    noTone(8); 
    bigDisplay("SUN FOUND");
    delay(2900);
  
    for (int i = 0; i < waitTimeSec/4; i++){
      disStart();
      delay(8000);
    }
    shouldWaitBaseCount = 0;
    shouldWaitTopCount = 0;
    sBase.attach(SERVO_BASE_PIN, 1000, 2000);
    sTop.attach(SERVO_TOP_PIN, 1000, 2000);
  }

  //delay(1000);
 
  //display stuff
  loopI++;
  if (loopI > 40) {
    loopI = 0;
    disStart();
  }
  
}

// print display stuff
void disStart() {
  goUp++;
  if (goUp > 5) {
    goUp = 0;
  }
  printURI(goUp);
}

void bigDisplay(String str) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(NAME);
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.print(str);
    display.setTextSize(1);
    display.display();
}

float printURI(int howMuchToPrint) {
  // widerstand
  float R = 10;
  // 5 weil 5V arduino
  float U =  ina219.getBusVoltage_V() ;
  float I = U / widerstand;
  float P = U * I;
  Serial.print((String) "U: " + U + " V,  ");
  Serial.print((String) "I: " + I + " A,  ");
  Serial.println((String) "P: " + P*1000 + " mW");
  
  // Display display
  display.clearDisplay();
  display.setCursor(0, 0 - howMuchToPrint*10);
  display.print(NAME);
  // voltage etc
  display.setCursor(0, 10 - howMuchToPrint*10);
  display.print((String) "Voltage: " + U + "V");
  display.setCursor(0, 20 - howMuchToPrint*10);
  display.print((String) "Power: " + P*1000 + " mW" );
  //temperature
  display.setCursor(0, 30 - howMuchToPrint*10);
  display.print((String) "Temp: " + bmp.readTemperature() + " *C" );
  // motor stuff
  display.setCursor(0, 40 - howMuchToPrint*10);
  display.print((String) "Angle Base: " + direction_base );
  display.setCursor(0, 50 - howMuchToPrint*10);
  display.print((String) "Angle Top: " + direction_top );
  // like top
  display.setCursor(0, 60 - howMuchToPrint*10);
  display.print(" ");
  display.setCursor(0, 70 - howMuchToPrint*10);
  display.print(NAME);
  display.display();
  display.setCursor(0, 80 - howMuchToPrint*10);
  display.print((String) "Voltage: " + U );
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
