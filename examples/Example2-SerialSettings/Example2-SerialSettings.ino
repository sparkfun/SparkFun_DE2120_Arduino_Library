#include "SparkFun_DE2120_Arduino_Library.h"
#include "SoftwareSerial.h"

SoftwareSerial softSerial(6,5);
DE2120 scanner;

void setup() 
{
  Serial.begin(9600);

  Serial.println("Connecting to Module...");
  
  softSerial.begin(9600);
  if(scanner.begin(softSerial)){
    Serial.println("Module Found!");  
  }else{
    Serial.println("Module Not Responding.");
    while(1){};
  }
}

void loop() 
{
  Serial.println("...");
  Serial.println("...");
  Serial.println("...");

  Serial.println("SparkX DE2120 Barcode Scanner Library");
  Serial.println("-------------------------------------");
  Serial.println("1\) Start Scan");
  Serial.println("2\) Stop Scan");
  Serial.println("3\) Enable Flashlight");
  Serial.println("4\) Disable Flashlight");
  Serial.println("5\) Set Reading Area");
  Serial.println("6\) Set Reading Mode");
  Serial.println("7\) Enable Disable Symbologies");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:");
  
  while(!Serial.available()){};

  char response = Serial.read();

  delay(10);

  while(Serial.available()){Serial.read(); delay(10);};

  switch(response){

    case '1':
      scanner.sendCommand("SCAN","");
      break;

    case '2':
      scanner.sendCommand("SLEEP","");
      break;

    case '3':
      scanner.sendCommand("LAMENA","1");
      //scanner.lightOn();
      break;

    case '4':
      scanner.sendCommand("LAMENA","0");
      //scanner.lightOff();
      break;

    case '5':
      //readingArea();
      break;

    case '6':
      //readingMode();
      break;

    case '7':
      //symbologies();
      break;

    default:
      Serial.println("Command not recognized");
      break;
  }
}

void reticle()
{
  Serial.println("...");
  Serial.println("...");
  Serial.println("...");
  Serial.println("-------------------------------------");
  Serial.println("1\) Enable Reticle");
  Serial.println("2\) Disable Reticle");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:"); 

 while(!Serial.available()){};

  char response = Serial.read();

  delay(10);

  while(Serial.available()){Serial.read(); delay(10);};

  switch(response){

    case '1':
      scanner.reticleOn();
      break;

    case '2':
      scanner.reticleOff();
      break;

    default:
      Serial.println("Command not recognized");
      break;
  }   
}

/*
void readingArea()
{
  Serial.println("...");
  Serial.println("...");
  Serial.println("...");
  Serial.println("-------------------------------------");
  Serial.println("1\) Full Width (Default)");
  Serial.println("2\) Center 80%");
  Serial.println("3\) Center 60%");
  Serial.println("4\) Center 40%");
  Serial.println("5\) Center 20%");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:"); 

 while(!Serial.available()){};

  char response = Serial.read();

  delay(10);

  while(Serial.available()){Serial.read(); delay(10);};

  switch(response){

    case '1':
      scanner.changeReadingArea(100);
      return;

    case '2':
      scanner.changeReadingArea(80);
      return;

    case '3':
      scanner.changeReadingArea(60);
      return;

    case '4':
      scanner.changeReadingArea(40);
      return;

    case '5':
      scanner.changeReadingArea(20);
      return;

    default:
      Serial.println("Command not recognized");
      return;
  }   
}

void readingMode()
{
  Serial.println("...");
  Serial.println("...");
  Serial.println("...");
  Serial.println("-------------------------------------");
  Serial.println("1\) Manual Read Mode (Default)");
  Serial.println("2\) Continuous Read Mode");
  Serial.println("3\) Motion Sensor Mode");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:"); 

 while(!Serial.available()){};

   char response = Serial.read();

   delay(10);

  while(Serial.available()){Serial.read(); delay(10);};
  
  switch(response){

    case '1':
      scanner.disableMotionSense();
      return;

    case '2':
      scanner.enableContinuousRead();
      return;

    case '3':
      scanner.enableMotionSense();
      return;

    default:
      Serial.println("Command not recognized");
      return;
  }   
}

void symbologies()
{

}

*/