
const int b[5] = {2,3,4,5,7}; // head R front L back
  

void setup() {
  for(int i = 0; i < 5; i++)  {
    pinMode(b[i], INPUT);
  }

  Serial.begin(9600);
  Serial.println("start read");
}

void loop() {
  int rd[5] = {0};

  for(int i = 0; i < 5; i++)  {
      rd[i] = digitalRead(b[i]);
    }

  for(int i = 0; i < 5; i++)  {
      Serial.print(rd[i]);
    }

  Serial.println();
  delay(1000);
}
