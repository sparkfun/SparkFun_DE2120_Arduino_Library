#include "SparkFun_DE2120_Arduino_Library.h"
#include "SoftwareSerial.h"

#define BUFFER_LEN 40

SoftwareSerial softSerial(3,2);
DE2120 scanner;

char scanBuffer[BUFFER_LEN];

void setup() {

  Serial.begin(115200);

  Serial.println("Connecting to Module...");
  
  softSerial.begin(9600);
  if(scanner.begin(softSerial)){
    Serial.println("Module Found!");  
    Serial.println("...");
    Serial.println("...");
    Serial.println("...");
  }else{
    Serial.println("Module Not Responding.");
    while(1){};
  }

}

void loop() {

  Serial.println("SparkX DE2120 Barcode Scanner Library");
  Serial.println("-------------------------------------");
  Serial.println("1\) Start Scan");
  Serial.println("2\) Stop Scan");
  Serial.println("3\) Print Firmware Ver");
  Serial.println("4\) ");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

}