#include "SparkFun_DE2120_Arduino_Library.h"
#include "SoftwareSerial.h"

#define BUFFER_LEN 40

SoftwareSerial softSerial(2, 3); //RX, TX: Connect Arduino pin 2 to scanner TX pin. Connect Arduino pin 3 to scanner RX pin.
DE2120 scanner;

//NOTE: You must put the module into TTL mode by scanning the POR232. barcode in the datasheet. This will put the module in
//the correct mode to receive and transmit serial. The baud rate in POR232 defaults to 115200. This library will automatically attempt
//set the baud rate to 9600bps.

char scanBuffer[BUFFER_LEN];

void setup()
{

  Serial.begin(115200);
  Serial.println("DE2120 Scanner Example");

  softSerial.begin(9600);

  scanner.begin(softSerial);
  //  if(scanner.begin(softSerial) == false)
  //  {
  //    Serial.println("Scanner did not respond. Please check wiring. Did you scan the POR232 barcode? Freezing...");
  //    while(1);
  //  }
  Serial.println();

  while (1)
    ;
}

void loop()
{

  delay(200);

  if (scanner.readBarcode(scanBuffer, BUFFER_LEN))
  {
    Serial.println("code found");
    for (int i = 0; i < strlen(scanBuffer); i++)
    {
      Serial.print(scanBuffer[i]);
    }
    Serial.println();
  }
}