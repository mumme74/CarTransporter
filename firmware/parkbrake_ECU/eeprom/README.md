EEPROM Driver For ChibiOS
=========================

Abstraction layer for EEPROM ICs like 25AA/25LC (SPI) and 24AA/24LC (I2C).

DESCRIPTION
-----------

This library designed to work on top of ChibiOS/RT. It abstracts access to
EEPROM like a regular file(s).

LIMITATIONS
-----------

* The maximum capacity of EEPROM is 65536 bytes for 24AA and 24LC (a driver
  limitation).

USAGE
-----

* Include `eeprom/eeprom.h` file to your project.
* Customize `eeprom/drvconf.h`.
* Define a file object of type `SPIEepromFileStream` or `I2CEepromFileStream` and
  config structure for it.
* Retrieve low level eeprom driver descriptor by 'EepromFindDevice()'.
* Open it with `EepromFileOpen()` function.

Now you can access to EEPROM array like to a regular file with functions:

* `chFileStreamRead()`
* `chFileStreamWrite()`
* `chFileStreamGetPosition()`
* `chFileStreamSeek()`
* etc

Examples of usage and checking the returning parameter you can found in
`eeprom/eeprom_testsuit.c`.

Based on [24aa project] of ChibiOS/RT from Uladzimir Pylinski (aka [barthess]):

[24aa project]: https://github.com/barthess/24aa
[barthess]: https://github.com/barthess
