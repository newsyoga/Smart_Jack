#include <RH_ASK.h>
#include <SPI.h>

////////////////////// libs MPU /////////////////////
#include "Arduino.h"
#include "MPU6050.h"
#include "Wire.h"
#include "I2Cdev.h"
////////////////////// libs heart /////////////////////
#include "Arduino.h"
#include "pulse-sensor-arduino.h"


RH_ASK driver;

typedef struct {
   int bpm;
   double calories;
   uint8_t touch :5;
} dtaPct;


//////////////////////////////////////////////////////////////////////////////////////////// edit below ////////////////////////////////////////////////////////////////////////////

///// trx ///////

dtaPct test = {0,0.0,0b11111};
unsigned long Dlay = 1000;

///// calories ///////

MPU6050 mpu6050;

double calorie_index = (0.05 * Dlay)/1000;
double mainCal = 0;

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

///// touch ///////
const int b[5] = {2,3,4,5,7}; // head R front L back

///// heart ///////
#define HEARTPULSE_PIN_SIG	A0
PulseSensor heartpulse;
int Delay_in = Dlay;
int s20 = 1;
bool in20 = 0;
int count_hb = 0;

//////////////////////////////////////////////////////////////////////////////////////////// send TRX data ////////////////////////////////////////////////////////////////////////////


void init_tx(){
  Serial.begin(9600);	  // Debugging only
    Serial.println("Tx start");
    if (!driver.init())
         Serial.println("init failed");
}

void send_data_prime(){

    String a = String(test.bpm) + ","+ String(test.calories)+ "," + String(test.touch) + ",";
    char msg[a.length()];

    a.toCharArray(msg, sizeof(msg)+1);
    Serial.println(msg);

    //char *msg = "Hello World!";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    Serial.println();
}



void  collect_data(){
  test.calories = getCal();

  int rd[5] = {0};

  for(int i = 0; i < 5; i++)  {
    rd[i] = digitalRead(b[i]);
  }

  for(int i = 0; i < 5; i++)  {
      //Serial.print(rd[i]);
    }

// Serial.println(rd[0]);
// Serial.println(0b1 | 0b10);
  int touchsnd = rd[0] | (rd[1] << 1) | (rd[2] << 2) | (rd[3] << 3) | (rd[4] << 4);
  Serial.println(touchsnd, BIN);
  test.touch = touchsnd;

  test.bpm = heartpulse.BPM;

}


//////////////////////////////////////////////////////////////////////////////////////////// calories ////////////////////////////////////////////////////////////////////////////

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


void init_mpu(){
  Wire.begin();
  mpu6050.initialize();
}

//////////////////////////////////////////////////////////////////////////////////////////// touch ////////////////////////////////////////////////////////////////////////////


void init_touch(){
  for(int i = 0; i < 5; i++)  {
    pinMode(b[i], INPUT);
  }
}


//////////////////////////////////////////////////////////////////////////////////////////// heart ////////////////////////////////////////////////////////////////////////////
void init_heart(){
  heartpulse.begin(HEARTPULSE_PIN_SIG);
}





void setup()
{
    init_tx();
    init_mpu();
    init_touch();
}

void loop()
{
    //collect_data();
    //send_data_prime();
    //delay(Dlay);


  if(count_hb > (Delay_in / s20)){ // called once in given time
    /*int heartpulseBPM = heartpulse.BPM;
    Serial.println(heartpulseBPM);

    if (heartpulse.QS == true) {
    Serial.println("PULSE");
    heartpulse.QS = false;
    }*/

    collect_data();
    send_data_prime();

    count_hb = 0;
  } 


  heartpulse.every20s();
  count_hb ++;
  delay(s20);
}

