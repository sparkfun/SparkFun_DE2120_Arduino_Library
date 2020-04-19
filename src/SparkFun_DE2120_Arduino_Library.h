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

#ifndef _SPARKFUN_DE2120_ARDUINO_LIBRARY_H
#define _SPARKFUN_DE2120_ARDUINO_LIBRARY_H
#include "Arduino.h"

#include <SoftwareSerial.h>

#define DE2120_COMMAND_ACK 0x06
#define DE2120_COMMAND_NACK 0x15

//These are the commands we can send (Prepend "^_^" and append ".")
#define COMMAND_START_SCAN "SCAN"
#define COMMAND_STOP_SCAN "SLEEP"
#define COMMAND_SET_DEFAULTS "DEFALT"
#define COMMAND_GET_VERSION "DSPYFW"

#define PROPERTY_BUZZER_FREQ "BEPPWM"
//BEPPWM0 - Active Drive
//BEPPWM1 - Passive Low Freq
//BEPPWM2 - Passive Med Freq (default)
//BEPPWM3 - Passive Hi Freq

#define PROPERTY_DECODE_BEEP "BEPSUC"
//BEPSUC1 - ON (default)
//BEPSUC0 - OFF

#define PROPERTY_BOOT_BEEP "BEPPWR"
//BEPPWR1 - ON (default)
//BEPPWR0 - OFF

#define PROPERTY_FLASH_LIGHT "LAMENA"
//LAMENA1 - ON (default)
//LAMENA0 - OFF

#define PROPERTY_AIM_LIGHT "AIMENA"
//AIMENA1 - ON (default)
//AIMENA0 - OFF

#define PROPERTY_READING_AREA "IMGREG"
//IMGREG0 - Full Width (default)
//IMGREG1 - Center 80%
//IMGREG2 - Center 60%
//IMGREG3 - Center 40%
//IMGREG4 - Center 20%

#define PROPERTY_MIRROR_FLIP "MIRLRE"
//MIRLRE1 - ON
//MIRLRE0 - OFF (default)

#define PROPERTY_USB_DATA_FORMAT "UTFEAN"
//UTFEAN0 - GBK (default)
//UTFEAN1 - UTF-8

#define PROPERTY_SERIAL_DATA_FORMAT "232UTF"
//232UTF0 - GBK (default)
//232UTF1 - UTF-8
//232UTF2 - Unicode BIG
//232UTF3 - Unicode little

#define PROPERTY_INVOICE_MODE "SPCINV"
//SPCINV1 - ON
//SPCINV0 - OFF (default)

#define PROPERTY_VIRTUAL_KEYBOARD "KBDVIR"
//KBDVIR1 - ON (default)
//KBDVIR0 - OFF

#define PROPERTY_COMM_MODE "POR"
//PORKBD - USB-KBW Mode
//PORHID - USB-HID Mode
//PORVIC - USB-COM Mode
//POR232 - TTL/RS232

#define PROPERTY_BAUD_RATE "232BAD"
//232BAD2 - 1200 bps
//232BAD3 - 2400 bps
//232BAD4 - 4800 bps
//232BAD5 - 9600 bps
//232BAD6 - 19200 bps
//232BAD7 - 38400 bps
//232BAD8 - 57600 bps
//232BAD9 - 115200 bps (default)

#define PROPERTY_READING_MODE "SCM"
//SCMMAN - Manual (default)
//SCMCNT - Continuous
//SCMMDH - Motion Mode

#define PROPERTY_CONTINUOUS_MODE_INTERVAL "CNTALW"
//CNTALW0 - Output Once
//CNTALW1 - Output Continuous No Interval
//CNTALW2 - Output Continuous 0.5s Interval
//CNTALW3 - Output Continuous 1s Interval

#define PROPERTY_MOTION_SENSITIVITY "MDTTHR"
//MDTTHR15 - Extremely High Sensitivity
//MDTTHR20 - High Sensitivity (default)
//MDTTHR30 - Highish Sensitivity
//MDTTHR50 - Mid Sensitivity
//MDTTHR100 - Low Sensitivity

#define PROPERTY_TRANSFER_CODE_ID "CIDENA"
//CIDENA1 - Transfer Code ID
//CIDENA0 - Do Not Transfer Code ID (default)

#define PROPERTY_KBD_CASE_CONVERSION "KBDCNV"
//KBDCNV0 - No conversion (default)
//KBDCNV1 - ALL CAPS
//KBDCNV2 - all lowercase
//KBDCNV3 - case-to-case

// Barcode Style Enable/Disable
#define PROPERTY_ENABLE_ALL_1D "ODCENA"
#define PROPERTY_DISABLE_ALL_1D "ODCDIS"
#define PROPERTY_ENABLE_ALL_2D "AQRENA"
#define PROPERTY_DISABLE_ALL_2D "AQRDIS"

class DE2120
{
public:
  DE2120();

  bool begin(HardwareSerial &serialPort);
  bool begin(SoftwareSerial &serialPort);

  bool isConnected();   //Returns true if device's ID is what it should be
  uint8_t getVersion(); //Queries device for its Version #
  bool factoryDefault();
  bool sendCommand(const char *cmd, const char *arg = "", uint32_t maxWaitInms = 3000);
  bool readBarcode(char *resultBuffer, uint8_t size);
  bool available();
  int read();
  bool changeBaudRate(uint32_t baud);
  bool changeBuzzerTone(uint8_t tone);
  bool enableDecodeBeep();
  bool disableDecodeBeep();
  bool enableBootBeep();
  bool disableBootBeep();
  bool lightOn();
  bool lightOff();
  bool reticleOn();
  bool reticleOff();
  bool changeReadingArea(uint8_t percent);
  bool enableImageFlipping();
  bool disableImageFlipping();
  bool USBMode(char *mode);
  bool enableContinuousRead(uint8_t repeatInterval = 2);
  bool disableContinuousRead();
  bool enableMotionSense(uint8_t sensitivity = 50);
  bool disableMotionSense();
  bool enableAll1D();
  bool disableAll1D();
  bool enableAll2D();
  bool disableAll2D();
  bool startScan();
  bool stopScan();

private:
  //char _responseBuffer[]

  HardwareSerial *hwStream;
  SoftwareSerial *swStream;
  Stream *_serial;
};

#endif