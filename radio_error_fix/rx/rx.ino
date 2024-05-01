#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

typedef struct {
   int bpm;
   double calories;
   uint8_t touch :5;
} dtaPct;

dtaPct gdata;
String dataRcv[3] = {"","",""};

void EmptyData(){
      dataRcv[0] = "";
      dataRcv[1] = "";
      dataRcv[2] = "";

      gdata.bpm=0;
      gdata.calories=0;
      gdata.touch=0;
}

void setup()
{
    Serial.begin(9600);	// Debugging only
    Serial.println("Start rx");
    if (!driver.init())
         Serial.println("init failed");
}

void printData(){
  Serial.println(gdata.bpm);
  Serial.println(gdata.calories);
  Serial.println(gdata.touch,BIN);
}

void loop()
{
   collectData();
}

void collectData(){
   uint8_t buf[12];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      EmptyData();
      int i;
      bool ocr1 = 0;
      bool ocr2 = 0;

      for(int j=0; j < strlen(buf); j++){

        if(!ocr1){
          if(buf[j] == ','){
            ocr1= true;
            continue;
          }
          dataRcv[0] += (char) buf[j];
        }

        if(ocr1 && !ocr2){
          if(buf[j] == ','){
            ocr2= true;
            continue;
          }
          dataRcv[1] += (char) buf[j];
        }

        if(ocr2){
          if(buf[j] == ','){
            break;
          }
          dataRcv[2] += (char) buf[j];
        }

      }
      gdata.bpm =  dataRcv[0].toInt();
      gdata.calories = dataRcv[1].toDouble();
      gdata.touch = dataRcv[2].toInt();
      printData();

    }
}