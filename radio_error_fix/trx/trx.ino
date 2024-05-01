#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

typedef struct {
   int bpm;
   double calories;
   uint8_t touch :5;
} dtaPct;


//////////////////////////////////////////////////////////////////////////////////////////// edit below ////////////////////////////////////////////////////////////////////////////



dtaPct test = {9,9.9,0b11111};
unsigned long Dlay = 1000;



//////////////////////////////////////////////////////////////////////////////////////////// edit below ////////////////////////////////////////////////////////////////////////////


void init_tx(){
  Serial.begin(9600);	  // Debugging only
    Serial.println("Tx start");
    if (!driver.init())
         Serial.println("init failed");
}


void setup()
{
    init_tx();
}

void loop()
{
    send_data_prime();
    delay(Dlay);
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