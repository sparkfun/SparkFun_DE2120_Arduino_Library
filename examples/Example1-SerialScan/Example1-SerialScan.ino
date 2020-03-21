#include "SparkFun_DE2120_Arduino_Library.h"
#include "SoftwareSerial.h"

#define BUFFER_LEN 40

SoftwareSerial softSerial(3,2);
DE2120 scanner;

char scanBuffer[BUFFER_LEN];

void setup() {

  Serial.begin(115200);
  
  softSerial.begin(9600);
  Serial.println(scanner.begin(softSerial));

}

void loop() {

  delay(200);

  if(scanner.readBarcode(scanBuffer, BUFFER_LEN)){
    Serial.println("code found");
    for(int i = 0; i < strlen(scanBuffer); i++){
      Serial.print(scanBuffer[i]);
    }
    Serial.println();
  }

}
