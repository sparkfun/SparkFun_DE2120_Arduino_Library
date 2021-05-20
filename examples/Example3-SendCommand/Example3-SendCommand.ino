/*
  Send arbitrary commands undocumented in the library
  By: Nick Poole
  SparkFun Electronics
  Date: April 14th 2020
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  This example demonstrates how to use the "sendCommand()" method to send arbitrary serial commands to the barcode reader.
  It also demonstrates the "CIDENA" or "Code ID Enable" function, which includes the barcode type when transmitting the 
  decoded string.

  sendCommand() takes two strings as arguments, concatenates them, adds the command prefix "^_^" and the command suffix "."
  and then transmits the command to the module. For example, to enable Matrix 2 of 5 scanning, which is done using the 
  command '^_^M25ENA1.' you would make the call 'scanner.sendCommand("M25ENA", "1")'

  While it is valid to call 'scanner.sendCommand("M25ENA1")' the former method is preferred in many cases.

  NOTE: You must put the module into TTL mode by scanning the POR232. barcode in the datasheet.
  This will put the module in the correct mode to receive and transmit serial. The baud rate in POR232
  defaults to 115200 which is too fast for software serial. This library will automatically set the baud rate to 9600bps.

  To connect the barcode scanner to an Arduino:

  (Arduino pin) = (Scanner pin)
  2 = TX pin on scanner
  3 = RX pin on scanner
  GND = GND
  3.3V = 3.3V

*/

#include "SoftwareSerial.h"
SoftwareSerial softSerial(2, 3); //RX, TX: Connect Arduino pin 2 to scanner TX pin. Connect Arduino pin 3 to scanner RX pin.

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
  Serial.println("Transmit Code ID with Barcode? (y/n)");
  Serial.println("-------------------------------------");
  Serial.println("Type 'y' or 'n' or scan a barcode: ");

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

      case 'y':
        Serial.println("Code ID will be displayed on scan");
        scanner.sendCommand("CIDENA", "1");
        break;

      case 'n':
        Serial.println("Code ID will NOT be displayed on scan");
        scanner.sendCommand("CIDENA", "0");
        break;

      default:
        Serial.println("Command not recognized");
        break;
    }
}

  void flushRx(){
    while(Serial.available()){
      Serial.read();
      delay(1);
    }
  }
