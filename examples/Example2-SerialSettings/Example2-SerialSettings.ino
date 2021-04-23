/*
  Menu control of scanner features
  By: Nick Poole
  SparkFun Electronics
  Date: April 14th 2020
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  This example demonstrates how to get the scanner connected and will output any barcode it sees.

  NOTE: You must put the module into TTL mode by scanning the POR232. barcode in the datasheet.
  This will put the module in the correct mode to receive and transmit serial. The baud rate in POR232
  defaults to 115200 which is too fast for software serial. This library will automatically set the baud rate to 9600bps.

  To connect the barcode scanner to an Arduino:

  (Arduino pin) = (Scanner pin)
  2 = RX pin on scanner
  3 = TX pin on scanner
  GND = GND
  3.3V = 3.3V

*/

#include "SoftwareSerial.h"
SoftwareSerial softSerial(2, 3); //RX, TX: Connect Arduino pin 3 to scanner TX pin. Connect Arduino pin 2 to scanner RX pin.

#include "SparkFun_DE2120_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_DE2120
DE2120 scanner;

#define BUFFER_LEN 40
char scanBuffer[BUFFER_LEN];

void setup()
{
  Serial.begin(115200);
  Serial.println("DE2120 Scanner Example");

  if (scanner.begin(softSerial) == false)
  {
    Serial.println("Scanner did not respond. Please check wiring. Did you scan the POR232 barcode? Freezing...");
    while (1)
      ;
  }
  Serial.println("Scanner online!");
}

void loop()
{
  flushRx(); // Clear the serial rx buffer to avoid line endings
  
  Serial.println();
  Serial.println("SparkFun DE2120 Barcode Scanner Library");
  Serial.println("-------------------------------------");
  Serial.println("1) Start Scan");
  Serial.println("2) Stop Scan");
  Serial.println("3) Enable/Disable Flashlight");
  Serial.println("4) Enable/Disable Aiming Reticle");
  Serial.println("5) Set Reading Area");
  Serial.println("6) Set Reading Mode");
  Serial.println("7) Enable Disable Symbologies");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number: ");

  // Wait for the user to reply but look for barcodes in the meantime
  while (Serial.available() == false)
  {
      if (scanner.readBarcode(scanBuffer, BUFFER_LEN))
    {
      Serial.println("...");
      Serial.print("Code found: ");
      for (int i = 0; i < strlen(scanBuffer); i++)
        Serial.print(scanBuffer[i]);
      Serial.println();
    }
  
    delay(200);
  }

    switch (Serial.read())
    {

      case '1':
        scanner.startScan();
        break;

      case '2':
        scanner.stopScan();
        break;

      case '3':
        flashlight();
        break;

      case '4':
        reticle();
        break;

      case '5':
        readingArea();
        break;

      case '6':
        readingMode();
        break;

      case '7':
        symbologies();
        break;

      default:
        Serial.println("Command not recognized");
        break;
    }
}

void flashlight()
{
  flushRx(); // Clear the serial rx buffer to avoid line endings
  
  Serial.println();
  Serial.println("-------------------------------------");
  Serial.println("1) Enable Flashlight");
  Serial.println("2) Disable Flashlight");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:");

  while (Serial.available() == false)
  {
    //Wait for user to send char
  }

    switch (Serial.read())
    {
      case '1':
        Serial.println("White scan light on");
        scanner.lightOn();
        break;

      case '2':
        Serial.println("White scan light off");
        scanner.lightOff();
        break;

      default:
        Serial.println("Command not recognized");
        break;
    }
}


void reticle()
{
  flushRx(); // Clear the serial rx buffer to avoid line endings
  
  Serial.println();
  Serial.println("-------------------------------------");
  Serial.println("1) Enable Reticle");
  Serial.println("2) Disable Reticle");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:");

  while (Serial.available() == false)
  {
    //Wait for user to send char
  }

    switch (Serial.read())
    {
      case '1':
        Serial.println("Red scan reticle on");
        scanner.reticleOn();
        break;

      case '2':
      Serial.println("Red scan reticle off");
        scanner.reticleOff();
        break;

      default:
        Serial.println("Command not recognized");
        break;
    }
  
}

  void readingArea()
  {

  flushRx(); // Clear the serial rx buffer to avoid line endings
    
  Serial.println("...");
  Serial.println("...");
  Serial.println("...");
  Serial.println("-------------------------------------");
  Serial.println("1) Full Width (Default)");
  Serial.println("2) Center 80%");
  Serial.println("3) Center 60%");
  Serial.println("4) Center 40%");
  Serial.println("5) Center 20%");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:");

  while (Serial.available() == false)
  {
    //Wait for user to send char
  }
  
  switch(Serial.read()){

    case '1':
      Serial.println("Scanning 100% of frame");
      scanner.changeReadingArea(100);
      return;

    case '2':
      Serial.println("Scanning center 80% of frame");
      scanner.changeReadingArea(80);
      return;

    case '3':
      Serial.println("Scanning center 60% of frame");
      scanner.changeReadingArea(60);
      return;

    case '4':
      Serial.println("Scanning center 40% of frame");
      scanner.changeReadingArea(40);
      return;

    case '5':
      Serial.println("Scanning center 20% of frame");
      scanner.changeReadingArea(20);
      return;

    default:
      Serial.println("Command not recognized");
      return;
  }
  }

  void readingMode()
  {
  flushRx(); // Clear the serial rx buffer to avoid line endings
    
  Serial.println("...");
  Serial.println("...");
  Serial.println("...");
  Serial.println("-------------------------------------");
  Serial.println("1) Manual Read Mode (Default)");
  Serial.println("2) Continuous Read Mode");
  Serial.println("3) Motion Sensor Mode");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:");

  while (Serial.available() == false)
  {
    //Wait for user to send char
  }

  switch(Serial.read()){

    case '1':
      Serial.println("Manual Trigger Mode enabled");
      scanner.disableMotionSense();
      return;

    case '2':
      Serial.println("Continuous Read Mode enabled");
      scanner.enableContinuousRead();
      return;

    case '3':
      Serial.println("Motion Trigger Mode enabled");
      scanner.enableMotionSense();
      return;

    default:
      Serial.println("Command not recognized");
      return;
  }
  }

  void symbologies()
  {
  flushRx(); // Clear the serial rx buffer to avoid line endings
    
  Serial.println("...");
  Serial.println("...");
  Serial.println("...");
  Serial.println("-------------------------------------");
  Serial.println("1) Enable All 1D Symbologies");
  Serial.println("2) Disable All 1D Symbologies");
  Serial.println("3) Enable All 2D Symbologies");
  Serial.println("4) Disable All 2D Symbologies");
  Serial.println("-------------------------------------");
  Serial.println("Select an option number:");

  while (Serial.available() == false)
  {
    //Wait for user to send char
  }

  switch(Serial.read()){

    case '1':
      Serial.println("1D Symbologies enabled");
      scanner.enableAll1D();
      return;

    case '2':
      Serial.println("1D Symbologies disabled");
      scanner.disableAll1D();
      return;

    case '3':
      Serial.println("2D Symbologies enabled");
      scanner.enableAll2D();
      return;

    case '4':
      Serial.println("2D Symbologies disabled");
      scanner.disableAll2D();
      return;

    default:
      Serial.println("Command not recognized");
      return;
  }
  }

  void flushRx(){
    while(Serial.available()){
      Serial.read();
      delay(1);
    }
  }
