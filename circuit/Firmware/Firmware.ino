double calorie_index = 0.05;
double mainCal = 0;

// Include Libraries
#include "Arduino.h"
#include "pulse-sensor-arduino.h"
#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "Button.h"

#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"


// Pin Definitions
#define HEARTPULSE_PIN_SIG A0
#define NRF24L01_PIN_CE 2
#define NRF24L01_PIN_CS 10
#define PUSHBUTTON_1_PIN_2 3
#define PUSHBUTTON_2_PIN_2 4
#define PUSHBUTTON_3_PIN_2 5
#define PUSHBUTTON_4_PIN_2 6
#define PUSHBUTTON_5_PIN_2 7
#define PUSHBUTTON_6_PIN_2 8
#define PUSHBUTTON_7_PIN_2 9
#define PUSHBUTTON_8_PIN_2	A1


// Global variables and defines
int16_t mpu6050Ax, mpu6050Ay, mpu6050Az;
int16_t mpu6050Gx, mpu6050Gy, mpu6050Gz;
// object initialization
PulseSensor heartpulse;
MPU6050 mpu6050;
Button pushButton_1(PUSHBUTTON_1_PIN_2);
Button pushButton_2(PUSHBUTTON_2_PIN_2);
Button pushButton_3(PUSHBUTTON_3_PIN_2);
Button pushButton_4(PUSHBUTTON_4_PIN_2);
Button pushButton_5(PUSHBUTTON_5_PIN_2);
Button pushButton_6(PUSHBUTTON_6_PIN_2);
Button pushButton_7(PUSHBUTTON_7_PIN_2);
Button pushButton_8(PUSHBUTTON_8_PIN_2);


// define vars for testing menu
const int timeout = 10000;  //define timeout of 10 sec
char menuOption = 0;
long time0;

RF24 radio(NRF24L01_PIN_CE, NRF24L01_PIN_CS);  // CE, CSN
const byte address[6] = "00001";



void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("start");

  heartpulse.begin(HEARTPULSE_PIN_SIG);
  Wire.begin();
  mpu6050.initialize();

  pushButton_1.init();
  pushButton_2.init();
  pushButton_3.init();
  pushButton_4.init();
  pushButton_5.init();
  pushButton_6.init();
  pushButton_7.init();
  pushButton_8.init();

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  //menuOption = menu();
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() {
  sendData();
  delay(50);
}

char menu() {

  Serial.println(F("\nWhich component would you like to test?"));
  Serial.println(F("(0) calories"));
  Serial.println(F("(1) Heart Rate Pulse Sensor"));
  Serial.println(F("(2) SparkFun MPU-6050 - Accelerometer and Gyro"));
  Serial.println(F("(3) NRF24L01 - 2.4G Wireless Transceiver Module"));
  Serial.println(F("(4) Mini Pushbutton Switch #1"));
  Serial.println(F("(5) Mini Pushbutton Switch #2"));
  Serial.println(F("(6) Mini Pushbutton Switch #3"));
  Serial.println(F("(7) Mini Pushbutton Switch #4"));
  Serial.println(F("(8) Mini Pushbutton Switch #5"));
  Serial.println(F("(9) Mini Pushbutton Switch #6"));
  Serial.println(F("(10) Mini Pushbutton Switch #7"));
  Serial.println(F("(menu) send anything else or press on board reset button\n"));
  while (!Serial.available())
    ;

  // Read data from serial monitor if received
  while (Serial.available()) {
    char c = Serial.read();
    if (isAlphaNumeric(c)) {

      if (c == '0')
        Serial.println(F("Now counting calories"));
      else if (c == '1')
        Serial.println(F("Now Testing Heart Rate Pulse Sensor"));
      else if (c == '2')
        Serial.println(F("Now Testing SparkFun MPU-6050 - Accelerometer and Gyro"));
      else if (c == '3')
        Serial.println(F("Now Testing NRF24L01 - 2.4G Wireless Transceiver Module - note that this component doesn't have a test code"));
      else if (c == '4')
        Serial.println(F("Now Testing Mini Pushbutton Switch #1"));
      else if (c == '5')
        Serial.println(F("Now Testing Mini Pushbutton Switch #2"));
      else if (c == '6')
        Serial.println(F("Now Testing Mini Pushbutton Switch #3"));
      else if (c == '7')
        Serial.println(F("Now Testing Mini Pushbutton Switch #4"));
      else if (c == '8')
        Serial.println(F("Now Testing Mini Pushbutton Switch #5"));
      else if (c == '9')
        Serial.println(F("Now Testing Mini Pushbutton Switch #6"));
      else if (c == '10')
        Serial.println(F("Now Testing Mini Pushbutton Switch #7"));
      else {
        Serial.println(F("illegal input!"));
        return 0;
      }
      time0 = millis();
      return c;
    }
  }
}



int heartBeats() {
  return heartpulse.BPM;
}

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

void readVelo() {
  mpu6050.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);  //read accelerometer and gyroscope raw data in three axes

  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  Serial.print("AngleX= ");
  Serial.println(x);

  Serial.print("AngleY= ");
  Serial.println(y);

  Serial.print("AngleZ= ");
  Serial.println(z);
  Serial.println("----------");
}


double return_positive(double val1, double val2) {
  double tmp = val1 - val2;
  if (tmp < 0) {
    return (tmp * -1.0);
  } else {
    return (tmp);
  }
}

double calculate_cal(double diff) {
  return ((diff * calorie_index) / 360);
}

double getCal() {
  mpu6050.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);

  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  double xin = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  double yin = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  double zin = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  // calculations
  double diffX = return_positive(xin, x);
  double diffY = return_positive(yin, y);
  double diffZ = return_positive(zin, z);

  double calo = calculate_cal(diffX) + calculate_cal(diffY) + calculate_cal(diffZ);

  // assign
  x = xin;
  y = yin;
  z = zin;

  return calo;
}

void countCal(){
  mainCal += getCal();
  Serial.println(mainCal);
  delay(300);
}

int countBPM(){
  int heartpulseBPM = heartpulse.BPM;
  if (heartpulse.QS == true) {
    Serial.println("PULSE");
    heartpulse.QS = false;
  }
  return heartpulseBPM;
}

byte getTouch(){
  bool pushButton_1Val = pushButton_1.read();
  bool pushButton_2Val = pushButton_2.read();
  bool pushButton_3Val = pushButton_3.read();
  bool pushButton_4Val = pushButton_4.read();

  bool pushButton_5Val = pushButton_5.read();
  bool pushButton_6Val = pushButton_6.read();
  bool pushButton_7Val = pushButton_7.read();
  bool pushButton_8Val = pushButton_8.read();


  byte vals = (pushButton_1Val | vals);
  vals = (pushButton_2Val << 1 | vals);
  vals = (pushButton_3Val << 2 | vals);
  vals = (pushButton_4Val << 3 | vals);

  vals = (pushButton_5Val << 4 | vals);
  vals = (pushButton_6Val << 5 | vals);
  vals = (pushButton_7Val << 6 | vals);
  vals = (pushButton_8Val << 7 | vals);

  // Serial.println(String(vals));

  return vals;
  
}

char* ConvertStringToCharArray(String S)
{
  int   ArrayLength  =S.length()+1;    //The +1 is for the 0x00h Terminator
  char  CharArray[ArrayLength];
  S.toCharArray(CharArray,ArrayLength);

  return(CharArray);
}


void sendData(){
  char dataMed[25];

  String touchDataStr = String(getTouch()) + "," + String(heartpulse.BPM) + "," + String(getCal());
  touchDataStr.toCharArray(dataMed,touchDataStr.length()+1);
  //Serial.println(sizeof(dataMed));

  radio.write(&dataMed, sizeof(dataMed));

}
