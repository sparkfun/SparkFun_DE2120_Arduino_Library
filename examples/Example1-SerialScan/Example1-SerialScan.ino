/*
  Begin scanning for bar codes
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
  3 = RX pin on scanner
  2 = TX pin on scanner
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
  if (scanner.readBarcode(scanBuffer, BUFFER_LEN))
  {
    Serial.print("Code found: ");
    for (int i = 0; i < strlen(scanBuffer); i++)
      Serial.print(scanBuffer[i]);
    Serial.println();
  }

  delay(200);
}