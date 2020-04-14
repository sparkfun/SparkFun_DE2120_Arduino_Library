/*
  This is a library written for the DE2120 2D Barcode Scanner Engine

  SparkFun sells these at its website: www.sparkfun.com

  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/TBD

  Written by Nick Poole @ SparkFun Electronics, January 9th, 2020

  The DE2120 is a camera-based barcode scanner
  https://github.com/sparkfun/SparkFun_DE2120_Arduino_Library
  Development environment specifics:

  Arduino IDE 1.8.7

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SparkFun_DE2120_Arduino_Library.h"
#include "Arduino.h"

#include <SoftwareSerial.h>

//Constructor
DE2120::DE2120()
{
}

//Initializes the device with basic settings
//Returns false if device is not detected
bool DE2120::begin(HardwareSerial &serialPort)
{
  //Trick comes from: https://forum.arduino.cc/index.php?topic=503782.msg3435988#msg3435988
  hwStream = &serialPort;
  swStream = NULL;
  _serial = hwStream;

  if (isConnected() == false)
    return (false); //No device detected

  //Clear any remaining incoming chars. The prevents a mis-read of the first barcode.
  while (_serial->available())
    _serial->read();

  return (true); //We're all setup!
}

bool DE2120::begin(SoftwareSerial &serialPort)
{
  //Serial.println("I am software");
  swStream = &serialPort;
  hwStream = NULL;
  _serial = swStream;

  if (isConnected() == false)
    return (false); //No device detected

  //Clear any remaining incoming chars. The prevents a mis-read of the first barcode.
  while (_serial->available())
    _serial->read();

  return (true); //We're all setup!
}

// Try to retrieve the firmware version number as a
// test to determine whether the module is connected.
// We're only testing to see if we got an ACK response,
// We don't ever check the actual firmware number.
bool DE2120::isConnected()
{
  //Attempt initial comm at 9600
  if (hwStream)
    hwStream->begin(9600);
  else
    swStream->begin(9600);

  if (sendCommand(COMMAND_GET_VERSION, "", 800)) //Takes ~430ms to get firmware version response
    return (true);

  //If we failed, try again at the factory default of 115200bps
  if (hwStream)
    hwStream->begin(115200);
  else
    swStream->begin(115200);

  delay(10);

  sendCommand(PROPERTY_BAUD_RATE, "5", 500); //Goto 9600bps
  //300ms is too quick for module to switch to new setting

  //Return to 9600bps
  if (hwStream)
    hwStream->begin(9600);
  else
    swStream->begin(9600);

  delay(10);

  if (sendCommand(COMMAND_GET_VERSION, "", 800)) //Takes ~430ms to get firmware version response
    return (true);

  return (false);
}

// Revert module to all factory default settings
// THIS WILL DISCONNECT THE MODULE FROM SERIAL
bool DE2120::factoryDefault()
{
  return (sendCommand(COMMAND_SET_DEFAULTS));
}

// Construct a command or parameter and send it to the
// module, then check the serial buffer for a response.
// Return TRUE if response contains ACK character, else
// return FALSE
bool DE2120::sendCommand(const char *cmd, const char *arg, uint32_t maxWaitInms)
{
  char commandString[14] = {'\0'};
  char start[] = "^_^";
  char end[] = ".";

  strcat(commandString, start);
  strcat(commandString, cmd);
  strcat(commandString, arg);
  strcat(commandString, end);

  _serial->print(commandString);

  uint32_t timeout = millis() + maxWaitInms;

  while (millis() < timeout)
  {
    if (_serial->available())
    {
      while (_serial->available())
      {
        byte incoming = _serial->read();
        if (incoming == DE2120_COMMAND_ACK)
          return (true);
        else if (incoming == DE2120_COMMAND_NACK)
          return (false);
      }
    }
    delay(1);
  }

  return (false);
}

// Check the receive buffer for serial data
// from the barcode scanner. If there's data,
// check the result buffer for a CR (marks a
// complete scan) and if it contains none, add
// to it. If a CR is found, we overwrite the
// result buffer until either it's full or we
// reach a CR in the receive buffer.
bool DE2120::readBarcode(char *resultBuffer, uint8_t size)
{
  if (!_serial->available())
    return false;

  bool crFound = false;
  for (uint8_t idx = 0; idx < size; idx++)
  {
    if (resultBuffer[idx] == '\r')
      crFound = true;
  }

  if (crFound)
    resultBuffer[0] = 0;

  for (uint8_t idx = strlen(resultBuffer); idx < size; idx++)
  {
    if (_serial->available())
    {
      resultBuffer[idx] = _serial->read();
      if (resultBuffer[idx] == '\r')
        return true;
    }
    else
      return false;
  }
}

// Change the serial baud rate for the barcode module (default 115200)
bool DE2120::changeBaudRate(uint32_t baud)
{
  uint8_t arg = 10;
  switch (baud)
  {
  case 1200:
    arg = 2;
    break;
  case 2400:
    arg = 3;
    break;
  case 4800:
    arg = 4;
    break;
  case 9600:
    arg = 5;
    break;
  case 19200:
    arg = 6;
    break;
  case 38400:
    arg = 7;
    break;
  case 57600:
    arg = 8;
    break;
  case 115200:
    arg = 9;
    break;
  }
  // only change baud rate if a valid value is passed
  if (arg < 10)
  {
    return (sendCommand(PROPERTY_BAUD_RATE, arg));
  }
  return (false);
}

// Change the beep frequency between low, med, and high
bool DE2120::changeBuzzerTone(uint8_t tone)
{
  // only change if a valid value is passed
  if (tone > 0 && tone < 4)
  {
    return (sendCommand(PROPERTY_BUZZER_FREQ, '0' + tone)); // conv single-digit int to char by adding to '0'
  }
  return (false);
}

// Enable and Disable Beep sound on successful read
bool DE2120::enableDecodeBeep()
{
  return (sendCommand(PROPERTY_DECODE_BEEP, "1"));
}
bool DE2120::disableDecodeBeep()
{
  return (sendCommand(PROPERTY_DECODE_BEEP, "0"));
}

// Enable and Disable Beed sound on startup
bool DE2120::enableBootBeep()
{
  return (sendCommand(PROPERTY_BOOT_BEEP, "1"));
}
bool DE2120::disableBootBeep()
{
  return (sendCommand(PROPERTY_BOOT_BEEP, "0"));
}

// Control the white illumination LED
bool DE2120::lightOn()
{
  return (sendCommand(PROPERTY_FLASH_LIGHT, "1"));
}
bool DE2120::lightOff()
{
  return (sendCommand(PROPERTY_FLASH_LIGHT, "0"));
}

// Control the red scan line
bool DE2120::reticleOn()
{
  return (sendCommand(PROPERTY_AIM_LIGHT, "1"));
}
bool DE2120::reticleOff()
{
  return (sendCommand(PROPERTY_AIM_LIGHT, "0"));
}

// Change the percentage of the frame to scan for barcodes
bool DE2120::changeReadingArea(uint8_t percent)
{
  uint8_t arg = 5;
  switch (percent)
  {
  case 100:
    arg = 0;
    break;
  case 80:
    arg = 1;
    break;
  case 60:
    arg = 2;
    break;
  case 40:
    arg = 3;
    break;
  case 20:
    arg = 4;
    break;
  }
  // only change if a valid value is passed
  if (arg < 5)
  {
    return (sendCommand(PROPERTY_READING_AREA, arg));
  }
  return (false);
}

// Enable and Disable Mirror Image reading
bool DE2120::enableImageFlipping()
{
  return (sendCommand(PROPERTY_MIRROR_FLIP, "1"));
}
bool DE2120::disableImageFlipping()
{
  return (sendCommand(PROPERTY_MIRROR_FLIP, "0"));
}

// Enable USB Communication and set the mode
// THIS WILL MAKE THE MODULE STOP RESPONDING ON TTL
bool DE2120::USBMode(char *mode)
{
  // reject invalid mode strings
  if (mode == "KBD" || mode == "HID" || mode == "VIC")
  {
    return (sendCommand(PROPERTY_COMM_MODE, mode));
  }
  return (false);
}

// Enable and disable continuous read mode
// if enabling, set the repeat interval for same-code reads
bool DE2120::enableContinuousRead(uint8_t repeatInterval)
{
  // reject invalid parameters
  if (repeatInterval < 4)
  {
    sendCommand(PROPERTY_READING_MODE, "CNT");
    return (sendCommand(PROPERTY_CONTINUOUS_MODE_INTERVAL, '0' + repeatInterval)); // conv single-digit int to char by adding to '0'
  }
  return (false);
}
bool DE2120::disableContinuousRead()
{
  return (sendCommand(PROPERTY_READING_MODE, "MAN"));
}

// Enable and disable motion sensitive read mode
// if enabling, set the sensitivity level
bool DE2120::enableMotionSense(uint8_t sensitivity)
{
  // reject invalid sensitivity values
  if (sensitivity == 15 || sensitivity == 20 || sensitivity == 30 || sensitivity == 50 || sensitivity == 100)
  {
    sendCommand(PROPERTY_READING_MODE, "MDH");
    return (sendCommand(PROPERTY_COMM_MODE, '0' + sensitivity)); // conv single-digit int to char by adding to '0'
  }
  return (false);
}
bool DE2120::disableMotionSense()
{
  return (sendCommand(PROPERTY_READING_MODE, "MAN"));
}

// Enable or Disable decoding of all 1D symbologies
bool DE2120::enableAll1D()
{
  return (sendCommand(PROPERTY_ENABLE_ALL_1D));
}
bool DE2120::disableAll1D()
{
  return (sendCommand(PROPERTY_DISABLE_ALL_1D));
}

// Enable or Disable decoding of all 2D symbologies
bool DE2120::enableAll2D()
{
  return (sendCommand(PROPERTY_ENABLE_ALL_2D));
}
bool DE2120::disableAll2D()
{
  return (sendCommand(PROPERTY_ENABLE_ALL_2D));
}

// Start or stop reading when in Trigger Mode (DEFAULT)
// Module will automatically stop reading after a few seconds
bool DE2120::startRead()
{
  return (sendCommand(COMMAND_START_SCAN));
}
bool DE2120::stopRead()
{
  return (sendCommand(COMMAND_STOP_SCAN));
}

/*
  // Enable or Disable particular barcode symbologies
  void DE2120::enableSymbology(char* symbology)
  {
  switch(symbology){
    case "UPCA":
      sendCommand(PROPERTY_ENABLE_UPCA, "1");
      break;
    case "UPCE":
      sendCommand(PROPERTY_ENABLE_UPCE, "1");
      break;
    case "EAN8":
      sendCommand(PROPERTY_ENABLE_EAN8, "1");
      break;
    case "EAN13":
      sendCommand(PROPERTY_ENABLE_EAN13, "1");
      break;
    case "CODE128":
      sendCommand(PROPERTY_ENABLE_CODE128, "1");
      break;
    case "GS1128":
      sendCommand(PROPERTY_ENABLE_GS1128, "1");
      break;
    case "CODE39":
      sendCommand(PROPERTY_ENABLE_CODE39, "1");
      break;
    case "CODE93":
      sendCommand(PROPERTY_ENABLE_CODE93, "1");
      break;
    case "CODE11":
      sendCommand(PROPERTY_ENABLE_CODE11, "1");
      break;
    case "INT2OF5":
      sendCommand(PROPERTY_ENABLE_INT2OF5, "1");
      break;
    case "MATRIX2OF5":
      sendCommand(PROPERTY_ENABLE_MATRIX2OF5, "1");
      break;
    case "GS1DATABAR":
      sendCommand(PROPERTY_ENABLE_GS1DATABAR, "1");
      break;
    case "CHINAPOST":
      sendCommand(PROPERTY_ENABLE_CHINAPOST, "1");
      break;
    case "QRCODE":
      sendCommand(PROPERTY_ENABLE_QRCODE, "1");
      break;
    case "DATAMATRIX":
      sendCommand(PROPERTY_ENABLE_DATAMATRIX, "1");
      break;
    case "PDF417":
      sendCommand(PROPERTY_ENABLE_PDF417, "1");
      break;
    case "MICROPDF417":
      sendCommand(PROPERTY_ENABLE_MICROPDF417, "1");
      break;
    case "AZTEC":
      sendCommand(PROPERTY_ENABLE_AZTEC, "1");
      break;
  }
  }
  void DE2120::disableSymbology(char* symbology)
  {
  switch(symbology){
    case "UPCA":
      sendCommand(PROPERTY_ENABLE_UPCA, "0");
      break;
    case "UPCE":
      sendCommand(PROPERTY_ENABLE_UPCE, "0");
      break;
    case "EAN8":
      sendCommand(PROPERTY_ENABLE_EAN8, "0");
      break;
    case "EAN13":
      sendCommand(PROPERTY_ENABLE_EAN13, "0");
      break;
    case "CODE128":
      sendCommand(PROPERTY_ENABLE_CODE128, "0");
      break;
    case "GS1128":
      sendCommand(PROPERTY_ENABLE_GS1128, "0");
      break;
    case "CODE39":
      sendCommand(PROPERTY_ENABLE_CODE39, "0");
      break;
    case "CODE93":
      sendCommand(PROPERTY_ENABLE_CODE93, "0");
      break;
    case "CODE11":
      sendCommand(PROPERTY_ENABLE_CODE11, "0");
      break;
    case "INT2OF5":
      sendCommand(PROPERTY_ENABLE_INT2OF5, "0");
      break;
    case "MATRIX2OF5":
      sendCommand(PROPERTY_ENABLE_MATRIX2OF5, "0");
      break;
    case "GS1DATABAR":
      sendCommand(PROPERTY_ENABLE_GS1DATABAR, "0");
      break;
    case "CHINAPOST":
      sendCommand(PROPERTY_ENABLE_CHINAPOST, "0");
      break;
    case "QRCODE":
      sendCommand(PROPERTY_ENABLE_QRCODE, "0");
      break;
    case "DATAMATRIX":
      sendCommand(PROPERTY_ENABLE_DATAMATRIX, "0");
      break;
    case "PDF417":
      sendCommand(PROPERTY_ENABLE_PDF417, "0");
      break;
    case "MICROPDF417":
      sendCommand(PROPERTY_ENABLE_MICROPDF417, "0");
      break;
    case "AZTEC":
      sendCommand(PROPERTY_ENABLE_AZTEC, "0");
      break;
  }
  }
*/