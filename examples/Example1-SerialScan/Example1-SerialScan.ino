#include "SparkFun_DE2120_Arduino_Library.h"
#include "SoftwareSerial.h"

SoftwareSerial softSerial(6,5);
DE2120 scanner;

void setup() {

  Serial.begin(9600);
  
  softSerial.begin(9600);
  Serial.println(scanner.begin(softSerial));

}

void loop() {

  delay(200);

  if(scanner.available()){
    Serial.print("code found: ");
    while(scanner.available()){
      Serial.print(char(scanner.read()));
    }
    Serial.println();
  }

}