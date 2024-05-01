
#include "Arduino.h"
#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"

MPU6050 mpu6050;

double calorie_index = 0.05;
double mainCal = 0;

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;




double return_positive(double val1, double val2) {
  double tmp = val1 - val2;
  if (tmp < 0) {
    return (tmp * -1.0);
  } else {
    return (tmp);
  }
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
}

double calculate_cal(double diff) {
  return ((diff * calorie_index) / 360);
}

void setup() 
{
    Serial.begin(9600);
    while (!Serial) ; 
    Serial.println("start");
    
    Wire.begin();
    mpu6050.initialize();
    
}

void loop() 
{
    delay(300);
    countCal();
}
