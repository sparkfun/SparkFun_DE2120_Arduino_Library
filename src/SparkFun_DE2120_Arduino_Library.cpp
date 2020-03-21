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

//Constructor
DE2120::DE2120()
{

}

//Initializes the device with basic settings
//Returns false if device is not detected
bool DE2120::begin(Stream &serialPort)
{
  _serial = &serialPort;
  if (isConnected() == false) {
    return (false); //No device detected
  } else {
    return (true); //We're all setup!
  }
}

// Try to retrieve the firmware version number as a
// test to determine whether the module is connected.
// We're only testing to see if we got an ACK response,
// We don't ever check the actual firmware number.
bool DE2120::isConnected()
{
  if (sendCommand(COMMAND_GET_VERSION))
  {
    return (true);
  } else { //
    return (false);
  }
}

// Revert module to all factory default settings
// THIS WILL DISCONNECT THE MODULE FROM SERIAL
void DE2120::factoryDefault()
{
  sendCommand(COMMAND_SET_DEFAULTS);
}

// Construct a command or parameter and send it to the
// module, then check the serial buffer for a response.
// Return TRUE if response contains ACK character, else
// return FALSE
bool DE2120::sendCommand(char* cmd, char* arg)
{
  char start[] = "^_^";
  char end[] = ".";
  strcpy(_commandString, "");
  strcat(_commandString, start);
  strcat(_commandString, cmd);
  strcat(_commandString, arg);
  strcat(_commandString, end);
  _serial->print(_commandString);

  uint8_t timeout = millis() + 3000;

    while(millis() < timeout)
  {
    if(_serial->available())
    {
      bool ACK = false;
      while(_serial->available())
      {
        if(_serial->read() == 0x06)
        {
          ACK = true;
        }
      }
      if(ACK)
      {
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    }
  }
}

// Check the receive buffer for serial data
// from the barcode scanner. If there's data,
// check the result buffer for a CR (marks a
// complete scan) and if it contains none, add
// to it. If a CR is found, we overwrite the
// result buffer until either it's full or we
// reach a CR in the receive buffer.
bool DE2120::readBarcode(char* resultBuffer, uint8_t size)
{

  if (!_serial->available()) {
    return false;
  } else {
    bool crFound = false;
    for (uint8_t idx = 0; idx < size; idx++) {
      if (resultBuffer[idx] == 13) {
        crFound = true;
      }
    }
    if (crFound) {
      resultBuffer[0] = 0;
    }
    for (uint8_t idx = strlen(resultBuffer); idx < size; idx++) {
      if (_serial->available()) {
        resultBuffer[idx] = _serial->read();
        if (resultBuffer[idx] == 13) {
          return true;
        }
      } else {
        return false;
      }
    }
  }

}

// Change the serial baud rate for the barcode module (default 115200)
void DE2120::changeBaudRate(uint16_t baud)
{
  uint8_t arg = 10;
  switch (baud) {
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
  if (arg < 10) {
    sendCommand(PROPERTY_BAUD_RATE, arg);
  }
}

// Change the beep frequency between low, med, and high
void DE2120::changeBuzzerTone(uint8_t tone)
{
  // only change if a valid value is passed
  if (tone > 0 && tone < 4) {
    sendCommand(PROPERTY_BUZZER_FREQ, '0' + tone); // conv single-digit int to char by adding to '0'
  }
}

// Enable and Disable Beep sound on successful read
void DE2120::enableDecodeBeep()
{
  sendCommand(PROPERTY_DECODE_BEEP, '1');
}
void DE2120::disableDecodeBeep()
{
  sendCommand(PROPERTY_DECODE_BEEP, '0');
}

// Enable and Disable Beed sound on startup
void DE2120::enableBootBeep()
{
  sendCommand(PROPERTY_BOOT_BEEP, '1');
}
void DE2120::disableBootBeep()
{
  sendCommand(PROPERTY_BOOT_BEEP, '0');
}

// Control the white illumination LED
void DE2120::lightOn()
{
  sendCommand(PROPERTY_FLASH_LIGHT, '1');
}
void DE2120::lightOff()
{
  sendCommand(PROPERTY_FLASH_LIGHT, '0');
}

// Control the red scan line
void DE2120::reticleOn()
{
  sendCommand(PROPERTY_AIM_LIGHT, '1');
}
void DE2120::reticleOff()
{
  sendCommand(PROPERTY_AIM_LIGHT, '0');
}

// Change the percentage of the frame to scan for barcodes
void DE2120::changeReadingArea(uint8_t percent)
{
  uint8_t arg = 5;
  switch (percent) {
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
  if (arg < 5) {
    sendCommand(PROPERTY_READING_AREA, arg);
  }
}

// Enable and Disable Mirror Image reading
void DE2120::enableImageFlipping()
{
  sendCommand(PROPERTY_MIRROR_FLIP, '1');
}
void DE2120::disableImageFlipping()
{
  sendCommand(PROPERTY_MIRROR_FLIP, '0');
}

// Enable USB Communication and set the mode
// THIS WILL MAKE THE MODULE STOP RESPONDING ON TTL
void DE2120::USBMode(char* mode)
{
  // reject invalid mode strings
  if (mode == 'KBD' || mode == 'HID' || mode == 'VIC') {
    sendCommand(PROPERTY_COMM_MODE, mode);
  }
}

// Enable and disable continuous read mode
// if enabling, set the repeat interval for same-code reads
void DE2120::enableContinuousRead(uint8_t repeatInterval)
{
  sendCommand(PROPERTY_READING_MODE, 'CNT');
  // reject invalid parameters
  if (repeatInterval < 4) {
    sendCommand(PROPERTY_CONTINUOUS_MODE_INTERVAL, '0' + repeatInterval); // conv single-digit int to char by adding to '0'
  }
}
void DE2120::disableContinuousRead()
{
  sendCommand(PROPERTY_READING_MODE, 'MAN');
}

// Enable and disable motion sensitive read mode
// if enabling, set the sensitivity level
void DE2120::enableMotionSense(uint8_t sensitivity)
{
  sendCommand(PROPERTY_READING_MODE, 'MDH');
  // reject invalid sensitivity values
  if (sensitivity == 15 || sensitivity == 20 || sensitivity == 30 || sensitivity == 50 || sensitivity == 100) {
    sendCommand(PROPERTY_COMM_MODE, '0' + sensitivity); // conv single-digit int to char by adding to '0'
  }
}
void DE2120::disableMotionSense()
{
  sendCommand(PROPERTY_READING_MODE, 'MAN');
}

// Enable or Disable decoding of all 1D symbologies
void DE2120::enableAll1D()
{
  sendCommand(PROPERTY_ENABLE_ALL_1D);
}
void DE2120::disableAll1D()
{
  sendCommand(PROPERTY_DISABLE_ALL_1D);
}

// Enable or Disable decoding of all 2D symbologies
void DE2120::enableAll2D()
{
  sendCommand(PROPERTY_ENABLE_ALL_2D);
}
void DE2120::disableAll2D()
{
  sendCommand(PROPERTY_ENABLE_ALL_2D);
}

// Start or stop reading when in Trigger Mode (DEFAULT)
// Module will automatically stop reading after a few seconds
void DE2120::startRead()
{
  sendCommand(COMMAND_START_SCAN);
}
void DE2120::stopRead()
{
  sendCommand(COMMAND_STOP_SCAN);
}

/*
  // Enable or Disable particular barcode symbologies
  void DE2120::enableSymbology(char* symbology)
  {
  switch(symbology){
    case 'UPCA':
      sendCommand(PROPERTY_ENABLE_UPCA, '1');
      break;
    case 'UPCE':
      sendCommand(PROPERTY_ENABLE_UPCE, '1');
      break;
    case 'EAN8':
      sendCommand(PROPERTY_ENABLE_EAN8, '1');
      break;
    case 'EAN13':
      sendCommand(PROPERTY_ENABLE_EAN13, '1');
      break;
    case 'CODE128':
      sendCommand(PROPERTY_ENABLE_CODE128, '1');
      break;
    case 'GS1128':
      sendCommand(PROPERTY_ENABLE_GS1128, '1');
      break;
    case 'CODE39':
      sendCommand(PROPERTY_ENABLE_CODE39, '1');
      break;
    case 'CODE93':
      sendCommand(PROPERTY_ENABLE_CODE93, '1');
      break;
    case 'CODE11':
      sendCommand(PROPERTY_ENABLE_CODE11, '1');
      break;
    case 'INT2OF5':
      sendCommand(PROPERTY_ENABLE_INT2OF5, '1');
      break;
    case 'MATRIX2OF5':
      sendCommand(PROPERTY_ENABLE_MATRIX2OF5, '1');
      break;
    case 'GS1DATABAR':
      sendCommand(PROPERTY_ENABLE_GS1DATABAR, '1');
      break;
    case 'CHINAPOST':
      sendCommand(PROPERTY_ENABLE_CHINAPOST, '1');
      break;
    case 'QRCODE':
      sendCommand(PROPERTY_ENABLE_QRCODE, '1');
      break;
    case 'DATAMATRIX':
      sendCommand(PROPERTY_ENABLE_DATAMATRIX, '1');
      break;
    case 'PDF417':
      sendCommand(PROPERTY_ENABLE_PDF417, '1');
      break;
    case 'MICROPDF417':
      sendCommand(PROPERTY_ENABLE_MICROPDF417, '1');
      break;
    case 'AZTEC':
      sendCommand(PROPERTY_ENABLE_AZTEC, '1');
      break;
  }
  }
  void DE2120::disableSymbology(char* symbology)
  {
  switch(symbology){
    case 'UPCA':
      sendCommand(PROPERTY_ENABLE_UPCA, '0');
      break;
    case 'UPCE':
      sendCommand(PROPERTY_ENABLE_UPCE, '0');
      break;
    case 'EAN8':
      sendCommand(PROPERTY_ENABLE_EAN8, '0');
      break;
    case 'EAN13':
      sendCommand(PROPERTY_ENABLE_EAN13, '0');
      break;
    case 'CODE128':
      sendCommand(PROPERTY_ENABLE_CODE128, '0');
      break;
    case 'GS1128':
      sendCommand(PROPERTY_ENABLE_GS1128, '0');
      break;
    case 'CODE39':
      sendCommand(PROPERTY_ENABLE_CODE39, '0');
      break;
    case 'CODE93':
      sendCommand(PROPERTY_ENABLE_CODE93, '0');
      break;
    case 'CODE11':
      sendCommand(PROPERTY_ENABLE_CODE11, '0');
      break;
    case 'INT2OF5':
      sendCommand(PROPERTY_ENABLE_INT2OF5, '0');
      break;
    case 'MATRIX2OF5':
      sendCommand(PROPERTY_ENABLE_MATRIX2OF5, '0');
      break;
    case 'GS1DATABAR':
      sendCommand(PROPERTY_ENABLE_GS1DATABAR, '0');
      break;
    case 'CHINAPOST':
      sendCommand(PROPERTY_ENABLE_CHINAPOST, '0');
      break;
    case 'QRCODE':
      sendCommand(PROPERTY_ENABLE_QRCODE, '0');
      break;
    case 'DATAMATRIX':
      sendCommand(PROPERTY_ENABLE_DATAMATRIX, '0');
      break;
    case 'PDF417':
      sendCommand(PROPERTY_ENABLE_PDF417, '0');
      break;
    case 'MICROPDF417':
      sendCommand(PROPERTY_ENABLE_MICROPDF417, '0');
      break;
    case 'AZTEC':
      sendCommand(PROPERTY_ENABLE_AZTEC, '0');
      break;
  }
  }
*/