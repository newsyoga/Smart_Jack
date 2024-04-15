
// Include Libraries
#include "Arduino.h"
#include "Buzzer.h"
#include "LED.h"

#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"


// Pin Definitions
#define BUZZER_PIN_SIG	2
#define LEDB_1_PIN_VIN	3
#define LEDB_2_PIN_VIN	5
#define LEDB_3_PIN_VIN	6
#define LEDB_4_PIN_VIN	9
#define LEDB_5_PIN_VIN	10
#define LEDB_6_PIN_VIN	A0
#define LEDB_7_PIN_VIN	A1
#define LEDB_8_PIN_VIN	A5
#define NRF24L01_PIN_CE	4
#define NRF24L01_PIN_CS	7



// Global variables and defines

// object initialization
Buzzer buzzer(BUZZER_PIN_SIG);
LED ledB_1(LEDB_1_PIN_VIN);
LED ledB_2(LEDB_2_PIN_VIN);
LED ledB_3(LEDB_3_PIN_VIN);
LED ledB_4(LEDB_4_PIN_VIN);
LED ledB_5(LEDB_5_PIN_VIN);

LED ledB_6(LEDB_6_PIN_VIN);
LED ledB_7(LEDB_7_PIN_VIN);
LED ledB_8(LEDB_8_PIN_VIN);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;


RF24 radio(NRF24L01_PIN_CE, NRF24L01_PIN_CS); // CE, CSN
const byte address[6] = "00001";



// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //menuOption = menu();
    
}

String recvData(){
  if (radio.available()) {
    char recivedStr[25];
    radio.read(&recivedStr, sizeof(recivedStr));     
    Serial.println(recivedStr);
    String retDat = recivedStr;
    return(retDat);
  }
}

void processData(){
  //String rcv = recvData();
  String rcv = "255,2930,34.59";
  String touchs;
  
  for(int i=0; i < rcv.length(); i++) {
    if(rcv.charAt(i) == ','){
      break;
    }
    touchs += rcv[i];
  }
  byte touch = touchs.toInt(); // invert input (~)
  // touch when low

  // plot LED
  // ON when low
  ledB_8.setState((touch >> 0) & 0b1);
  ledB_7.setState((touch >> 1) & 0b1);
  ledB_6.setState((touch >> 2) & 0b1);
  ledB_5.setState((touch >> 3) & 0b1);

  ledB_4.setState((touch >> 4) & 0b1);
  ledB_3.setState((touch >> 5) & 0b1);
  ledB_2.setState((touch >> 6) & 0b1);
  ledB_1.setState((touch >> 7) & 0b1);

}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() {
    processData();
    delay(50);
    
//     if(menuOption == '1') {
//     // Buzzer - Test Code
//     // The buzzer will turn on and off for 500ms (0.5 sec)
//     buzzer.on();       // 1. turns on
//     delay(500);             // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//     buzzer.off();      // 3. turns off.
//     delay(500);             // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//     }
//     else if(menuOption == '2') {
//     // LED - Basic Blue 5mm #1 - Test Code
//     // The LED will turn on and fade till it is off
//     for(int i=255 ; i> 0 ; i -= 5)
//     {
         //ledB_1;                      // 1. Dim Led 
//         delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//     }                                          
//     ledB_1.off();                        // 3. turns off
//     }
//     else if(menuOption == '3') {
//     // LED - Basic Blue 5mm #2 - Test Code
//     // The LED will turn on and fade till it is off
//     for(int i=255 ; i> 0 ; i -= 5)
//     {
//         ledB_2.dim(i);                      // 1. Dim Led 
//         delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//     }                                          
//     ledB_2.off();                        // 3. turns off
//     }
//     else if(menuOption == '4') {
//     // LED - Basic Blue 5mm #3 - Test Code
//     // The LED will turn on and fade till it is off
//     for(int i=255 ; i> 0 ; i -= 5)
//     {
//         ledB_3.dim(i);                      // 1. Dim Led 
//         delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//     }                                          
//     ledB_3.off();                        // 3. turns off
//     }
//     else if(menuOption == '5') {
//     // LED - Basic Blue 5mm #4 - Test Code
//     // The LED will turn on and fade till it is off
//     for(int i=255 ; i> 0 ; i -= 5)
//     {
//         ledB_4.dim(i);                      // 1. Dim Led 
//         delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//     }                                          
//     ledB_4.off();                        // 3. turns off
//     }
//     else if(menuOption == '6') {
//     // LED - Basic Blue 5mm #5 - Test Code
//     // The LED will turn on and fade till it is off
//     for(int i=255 ; i> 0 ; i -= 5)
//     {
//         ledB_5.dim(i);                      // 1. Dim Led 
//         delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//     }                                          
//     ledB_5.off();                        // 3. turns off
//     }
//     else if(menuOption == '7')
//     {
//     // Disclaimer: The NRF24L01 - 2.4G Wireless Transceiver Module is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
//     if (radio.available()) {
//     char text[32] = "";
//     radio.read(&text, sizeof(text));
//     Serial.println(text);
//     }
//     }
    
//     if (millis() - time0 > timeout)
//     {
//         menuOption = menu();
//     }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) Buzzer"));
    Serial.println(F("(2) LED - Basic Blue 5mm #1"));
    Serial.println(F("(3) LED - Basic Blue 5mm #2"));
    Serial.println(F("(4) LED - Basic Blue 5mm #3"));
    Serial.println(F("(5) LED - Basic Blue 5mm #4"));
    Serial.println(F("(6) LED - Basic Blue 5mm #5"));
    Serial.println(F("(7) NRF24L01 - 2.4G Wireless Transceiver Module"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing Buzzer"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing LED - Basic Blue 5mm #1"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing LED - Basic Blue 5mm #2"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing LED - Basic Blue 5mm #3"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing LED - Basic Blue 5mm #4"));
    		else if(c == '6') 
    			Serial.println(F("Now Testing LED - Basic Blue 5mm #5"));
    		else if(c == '7') 
    			Serial.println(F("Now Testing NRF24L01 - 2.4G Wireless Transceiver Module - note that this component doesn't have a test code"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}