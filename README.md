# Serial_2D_Barcode_Scanner_Library
Arduino library for controlling the DE2120 2D Barcode Scanner over serial

![2D Barcode Scanner Breakout](https://cdn.sparkfun.com/assets/parts/1/7/4/0/7/18088-SparkFun_2D_Barcode_Scanner-06.jpg)

[*2D Barcode Scanner Breakout (BOB-18088)*](https://www.sparkfun.com/products/18088)

This library allows the user to easily change common settings on the barcode reader and poll the device for scanned codes.

# Known Weirdness

The barcode scanner should be aimed at a nearby surface during testing. For unknown reasons, the module may hang during "scan" operations in low light conditions when nothing is within illumination range for the built-in LED. This doesn't seem to be an issue in well lit conditions. 

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/barcodes** - Utility barcodes for setting the device to TTL mode, factory reset, etc.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Documentation
--------------

* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.
* **[2D Barcode Scanner Breakout Hookup Guide](https://learn.sparkfun.com/tutorials/2d-barcode-scanner-breakout-hookup-guide/all)** - Basic Hookup Guide for the 2D Barcode Scanner Breakout.

Products that use this Library 
---------------------------------

* [*BOB-18088*](https://www.sparkfun.com/products/18088)
* [*SPX-16441*](https://www.sparkfun.com/products/16441)
* [*SPX-16410*](https://www.sparkfun.com/products/16410)

Version History
---------------

* [V_0.1.0](https://github.com/sparkfun/SparkFun_DE2120_Arduino_Library/tree/v0.1.0) - Pre-release
* [V_1.1.0](https://github.com/sparkfun/SparkFun_DE2120_Arduino_Library/tree/v1.1.0) - Public Release
* [V_1.2.0](https://github.com/sparkfun/SparkFun_DE2120_Arduino_Library/tree/v1.2.0) - Cleaned Up Unused Methods

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact techsupport@sparkfun.com.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
