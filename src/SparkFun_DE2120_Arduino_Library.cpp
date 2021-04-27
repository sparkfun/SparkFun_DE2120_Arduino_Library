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
DE2120::DE2120(void)
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

bool DE2120::available()
{
  return _serial->available();
}

int DE2120::read()
{
  return _serial->read();
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
// complete scan) If a CR is found, we overwrite the
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
    resultBuffer[0] = '\0';

  for (uint8_t idx = strlen(resultBuffer); idx < size; idx++)
  {
    if (_serial->available())
    {
      resultBuffer[idx] = _serial->read();
      if (resultBuffer[idx] == '\r')
      {
        resultBuffer[idx+1] = '\0';
        return true;
      }
    }
    else
      return false;
  }

  return (false);
}

// Change the serial baud rate for the barcode module (default 115200)
bool DE2120::changeBaudRate(uint32_t baud)
{
  char arg[2] = {'0', '\0'};

  switch (baud)
  {
  case 1200:
    arg[0] = '2';
    break;
  case 2400:
    arg[0] = '3';
    break;
  case 4800:
    arg[0] = '4';
    break;
  case 9600:
    arg[0] = '5';
    break;
  case 19200:
    arg[0] = '6';
    break;
  case 38400:
    arg[0] = '7';
    break;
  case 57600:
    arg[0] = '8';
    break;
  case 115200:
    arg[0] = '9';
    break;
  }
  // only change baud rate if a valid value is passed
  if (arg[0] != '0')
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
    char strTone[2] = {0, '\0'};
    strTone[0] = '0' + tone;
    return (sendCommand(PROPERTY_BUZZER_FREQ, strTone));
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
  char arg[2] = {0, '\0'};
  switch (percent)
  {
  case 100:
    arg[0] = '0';
    break;
  case 80:
    arg[0] = '1';
    break;
  case 60:
    arg[0] = '2';
    break;
  case 40:
    arg[0] = '3';
    break;
  case 20:
    arg[0] = '4';
    break;
  }
  // only change if a valid value is passed
  if (arg[0] != 0)
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

    char strRepeatInterval[2] = {0, '\0'};
    strRepeatInterval[0] = '0' + repeatInterval;

    return (sendCommand(PROPERTY_CONTINUOUS_MODE_INTERVAL, strRepeatInterval));
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
    char commandString[4] = {'\0'}; //Max is 100\0

    switch (sensitivity)
    {
    case (15):
      strcat(commandString, "15");
      break;
    case (20):
      strcat(commandString, "20");
      break;
    case (30):
      strcat(commandString, "30");
      break;
    case (50):
      strcat(commandString, "50");
      break;
    case (100):
      strcat(commandString, "100");
      break;
    default:
      break;
    }

    sendCommand(PROPERTY_READING_MODE, "MDH");
    return (sendCommand(PROPERTY_COMM_MODE, commandString));
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
  return (sendCommand(PROPERTY_DISABLE_ALL_2D));
}

// Start or stop reading when in Trigger Mode (DEFAULT)
// Module will automatically stop reading after a few seconds
bool DE2120::startScan()
{
  return (sendCommand(COMMAND_START_SCAN));
}
bool DE2120::stopScan()
{
  return (sendCommand(COMMAND_STOP_SCAN));
}