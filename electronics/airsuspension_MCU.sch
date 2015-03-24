EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:teensy
LIBS:power_switches
LIBS:miscellaneous
LIBS:airsuspension_controller-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Teensy3.1 U10
U 1 1 550B9955
P 5450 3250
F 0 "U10" H 4900 5300 60  0000 C CNN
F 1 "Teensy3.1" H 6100 5300 60  0000 C CNN
F 2 "" H 5600 3400 60  0001 C CNN
F 3 "https://www.pjrc.com/teensy/pinout.html" H 5600 3400 60  0001 C CNN
F 4 "Teensy3.1" H 5450 3250 60  0001 C CNN "name"
F 5 "2430007" H 5450 3250 60  0001 C CNN "farnell"
	1    5450 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR56
U 1 1 550B9984
P 4000 1300
F 0 "#PWR56" H 4000 1050 60  0001 C CNN
F 1 "GND" H 4000 1150 60  0000 C CNN
F 2 "" H 4000 1300 60  0000 C CNN
F 3 "" H 4000 1300 60  0000 C CNN
	1    4000 1300
	1    0    0    -1  
$EndComp
Text GLabel 6800 1400 2    60   Input ~ 0
+5V
Text GLabel 6800 1700 2    60   Input ~ 0
+3.3V
$Comp
L GND #PWR57
U 1 1 550B99C1
P 7300 1550
F 0 "#PWR57" H 7300 1300 60  0001 C CNN
F 1 "GND" H 7300 1400 60  0000 C CNN
F 2 "" H 7300 1550 60  0000 C CNN
F 3 "" H 7300 1550 60  0000 C CNN
	1    7300 1550
	1    0    0    -1  
$EndComp
Text HLabel 4200 1550 0    60   Input ~ 0
Compressor_DIAG
Text HLabel 4200 2300 0    60   Input ~ 0
Compressor_ON
Text HLabel 6800 2000 2    60   Input ~ 0
Compressor_Current
Text HLabel 6800 2600 2    60   Input ~ 0
Compressor_Pressure_3.3V
Text HLabel 4200 1700 0    60   Input ~ 0
Airdryer_DIAG
Text HLabel 4200 1850 0    60   Input ~ 0
LeftFill_DIAG
Text HLabel 4200 2000 0    60   Input ~ 0
CAN_TX
Text HLabel 4200 2150 0    60   Input ~ 0
CAN_RX
Text HLabel 4200 2450 0    60   Input ~ 0
Airdryer_ON
Text HLabel 4200 2600 0    60   Input ~ 0
LeftDump_ON
Text HLabel 4200 2750 0    60   Input ~ 0
LeftDump_DIAG
Text HLabel 4200 2900 0    60   Input ~ 0
LeftFill_ON
Text HLabel 4200 3050 0    60   Input ~ 0
RightFill_ON
Text HLabel 4200 3200 0    60   Input ~ 0
RightFill_DIAG
Text HLabel 4200 3350 0    60   Input ~ 0
RightDump_DIAG
Text HLabel 6800 1850 2    60   Input ~ 0
Spare1_ON
Text HLabel 6800 2300 2    60   Input ~ 0
LeftHeight_3.3V
Text HLabel 6800 2150 2    60   Input ~ 0
AnalogSpare_3.3V
Text HLabel 6800 2450 2    60   Input ~ 0
RightHeight_3.3V
Text HLabel 6800 2750 2    60   Input ~ 0
LeftPressure_3.3V
Text HLabel 6800 2900 2    60   Input ~ 0
RightPressure_3.3V
Text HLabel 6800 3050 2    60   Input ~ 0
SpareTemp_3.3V
Text HLabel 6800 3200 2    60   Input ~ 0
Spare1_Diag
Text HLabel 6800 3350 2    60   Input ~ 0
RightDump_ON
Text HLabel 4200 4050 0    60   Input ~ 0
CompressorTemp_3.3V
Text Notes 8050 1200 0    60   ~ 0
Please note !\nYou need to cut the jumper that feeds from USB\non your Teensy3.1 board. Else you will risk burning \nUSB port on your computer.
NoConn ~ 6800 3750
NoConn ~ 6800 4050
NoConn ~ 6800 4200
NoConn ~ 6800 4350
NoConn ~ 4200 3550
NoConn ~ 4200 4950
NoConn ~ 5000 5800
NoConn ~ 5450 5800
NoConn ~ 5850 5800
NoConn ~ 6800 5550
NoConn ~ 6800 5400
NoConn ~ 6800 5250
NoConn ~ 6800 5100
NoConn ~ 6800 4950
NoConn ~ 6800 4800
NoConn ~ 6800 4650
NoConn ~ 6800 4500
Text HLabel 4200 4350 0    60   Input ~ 0
LeftSuck_ON
Text HLabel 4200 4500 0    60   Input ~ 0
RightSuck_ON
$Comp
L GND #PWR55
U 1 1 550CFA12
P 3400 4200
F 0 "#PWR55" H 3400 3950 60  0001 C CNN
F 1 "GND" H 3400 4050 60  0000 C CNN
F 2 "" H 3400 4200 60  0000 C CNN
F 3 "" H 3400 4200 60  0000 C CNN
	1    3400 4200
	1    0    0    -1  
$EndComp
Text HLabel 4200 4650 0    60   Input ~ 0
LeftSuck_DIAG
Text HLabel 4200 4800 0    60   Input ~ 0
RightSuck_DIAG
NoConn ~ 6800 3600
NoConn ~ 4200 3700
NoConn ~ 4200 3850
Wire Wire Line
	4200 1400 4200 1300
Wire Wire Line
	4200 1300 4000 1300
Wire Wire Line
	6800 1550 7300 1550
Wire Wire Line
	4200 4200 3400 4200
$EndSCHEMATC
