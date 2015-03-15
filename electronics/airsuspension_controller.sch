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
Sheet 1 6
Title "ECU controller airsuspension"
Date "2015-03-14"
Rev "0.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 4550 3750 2200 2400
U 5504B555
F0 "Outputs" 60
F1 "airsuspension_outputs.sch" 60
F2 "Compressor_ONOFF" I R 6750 3850 60 
F3 "Compressor_DIAG" I R 6750 3950 60 
F4 "Compressor_CurrentSense" I R 6750 4050 60 
F5 "Compressor_OUT" I L 4550 3950 60 
F6 "LeftFill_OnOff" I R 6750 4200 60 
F7 "RightFill_OnOff" I R 6750 4450 60 
F8 "LeftFill_DIAG" I R 6750 4300 60 
F9 "RightFill_Diag" I R 6750 4550 60 
F10 "RightFill_OUT" I L 4550 4500 60 
F11 "LeftFill_Out" I L 4550 4250 60 
F12 "Airdryer_OnOff" I R 6750 5700 60 
F13 "Spare1_OnOff" I R 6750 5950 60 
F14 "Airdryer_DIAG" I R 6750 5800 60 
F15 "Spare1_Diag" I R 6750 6050 60 
F16 "Spare1_OUT" I L 4550 6000 60 
F17 "Airdryer_Out" I L 4550 5750 60 
F18 "LeftSuck_OnOff" I R 6750 5200 60 
F19 "RightSuck_OnOff" I R 6750 5450 60 
F20 "LeftSuck_DIAG" I R 6750 5300 60 
F21 "RightSuck_Diag" I R 6750 5550 60 
F22 "RightSuck_OUT" I L 4550 5500 60 
F23 "LeftSuck_Out" I L 4550 5250 60 
F24 "LeftDump_OnOff" I R 6750 4700 60 
F25 "RightDump_OnOff" I R 6750 4950 60 
F26 "LeftDump_DIAG" I R 6750 4800 60 
F27 "RightDump_Diag" I R 6750 5050 60 
F28 "RightDump_OUT" I L 4550 5000 60 
F29 "LeftDump_Out" I L 4550 4750 60 
$EndSheet
$Sheet
S 4350 600  2550 2750
U 550626C9
F0 "Inputs" 60
F1 "airsupsension_inputs.sch" 60
F2 "CompressorPressure_IN" I L 4350 800 60 
F3 "CompressorPressure_5V" I L 4350 700 60 
F4 "CompressurePressure_3.3V" I R 6900 800 60 
F5 "RightHeightPot_IN" I L 4350 2200 60 
F6 "RightHeightPot_5V" I L 4350 2100 60 
F7 "RightHeightPot_3.3V" I R 6900 2200 60 
F8 "LeftPressure_IN" I L 4350 1150 60 
F9 "LeftPressure_5V" I L 4350 1050 60 
F10 "LeftPressure_3.3V" I R 6900 1150 60 
F11 "RightPressure_IN" I L 4350 1500 60 
F12 "RightPressure_5V" I L 4350 1400 60 
F13 "RightPressure_3.3V" I R 6900 1500 60 
F14 "LeftHeightPot_IN" I L 4350 1850 60 
F15 "LeftHeightPot_5V" I L 4350 1750 60 
F16 "LeftHeightPot_3.3V" I R 6900 1850 60 
F17 "AnalogSpare_IN" I L 4350 2550 60 
F18 "AnalogSpare_5V" I L 4350 2450 60 
F19 "AnalogSpare_3.3V" I R 6900 2550 60 
F20 "CompressorTemp_IN" I L 4350 2900 60 
F21 "CompressorTemp_5V" I L 4350 2800 60 
F22 "CompressureTemp_3.3V" I R 6900 2850 60 
F23 "SpareTemp_IN" I L 4350 3150 60 
F24 "SpareTemp_5V" I L 4350 3050 60 
F25 "SpareTemp_3.3V" I R 6900 3150 60 
F26 "CompressorPressure_0V" I L 4350 900 60 
F27 "LeftPressure_0V" I L 4350 1250 60 
F28 "RightPressure_0V" I L 4350 1600 60 
F29 "LeftHeightPot_0V" I L 4350 1950 60 
F30 "RightHeightPot_0V" I L 4350 2300 60 
F31 "AnaglogSpare_0V" I L 4350 2650 60 
$EndSheet
$Sheet
S 1650 6600 1100 350 
U 55069990
F0 "CAN_bus" 60
F1 "airsuspension_CAN.sch" 60
F2 "CAN_HI" I L 1650 6700 60 
F3 "CAN_LO" I L 1650 6850 60 
F4 "CAN_RX" I R 2750 6700 60 
F5 "CAN_TX" I R 2750 6850 60 
$EndSheet
$Sheet
S 1600 750  800  250 
U 5507DD0F
F0 "5V power" 60
F1 "Airsuspension_5V.sch" 60
F2 "+12V_IGN" I L 1600 850 60 
$EndSheet
Text GLabel 2150 5350 0    60   Input ~ 0
+12V_bat
$Comp
L GNDPWR #PWR01
U 1 1 55081FC6
P 2650 4850
F 0 "#PWR01" H 2650 4650 40  0001 C CNN
F 1 "GNDPWR" H 2650 4720 40  0000 C CNN
F 2 "" H 2650 4800 60  0000 C CNN
F 3 "" H 2650 4800 60  0000 C CNN
	1    2650 4850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5508267F
P 3350 4150
F 0 "#PWR02" H 3350 3900 60  0001 C CNN
F 1 "GND" H 3350 4000 60  0000 C CNN
F 2 "" H 3350 4150 60  0000 C CNN
F 3 "" H 3350 4150 60  0000 C CNN
	1    3350 4150
	1    0    0    -1  
$EndComp
$Comp
L conn_molex_36638-48pin conn1
U 1 1 550928D4
P 2000 3900
F 0 "conn1" H 2000 5250 60  0000 C CNN
F 1 "conn_molex_36638-48pin" H 1950 2600 60  0000 C CNN
F 2 "" H 1950 4750 60  0000 C CNN
F 3 "" H 1950 4750 60  0000 C CNN
	1    2000 3900
	1    0    0    -1  
$EndComp
Text Label 1300 4850 2    60   ~ 0
Compressor_OUT
Text Label 4550 3950 2    60   ~ 0
Compressor_OUT
Text Label 1600 850  2    60   ~ 0
+12V_IGN
Text Label 1300 4150 2    60   ~ 0
+12V_IGN
Text Label 4550 4250 2    60   ~ 0
LeftFill_OUT
Text Label 4550 4500 2    60   ~ 0
RightFill_Out
Text Label 1300 4500 2    60   ~ 0
LeftFill_OUT
Text Label 1300 4600 2    60   ~ 0
RightFill_OUT
Text Label 4550 4750 2    60   ~ 0
LeftDump_OUT
Text Label 4550 5000 2    60   ~ 0
RightDump_OUT
Text Label 2600 4500 0    60   ~ 0
LeftDump_OUT
Text Label 2600 4600 0    60   ~ 0
RightDump_Out
Text Label 4550 5250 2    60   ~ 0
LeftSuck_OUT
Text Label 4550 5500 2    60   ~ 0
RightSuck_OUT
Text Label 4550 5750 2    60   ~ 0
AirDryer_Out
Text Label 4550 6000 2    60   ~ 0
Spare1_OUT
Text Label 1300 4300 2    60   ~ 0
LeftSuck_OUT
Text Label 1300 4400 2    60   ~ 0
RightSuck_OUT
Text Label 2600 4300 0    60   ~ 0
AirDryer_OUT
Text Label 2600 4400 0    60   ~ 0
Spare1_OUT
NoConn ~ 1900 4100
NoConn ~ 2150 4100
Text Label 1650 6700 2    60   ~ 0
CAN_HI
Text Label 1650 6850 2    60   ~ 0
CAN_LO
Text Label 1300 3900 2    60   ~ 0
CAN_HI
Text Label 1300 4000 2    60   ~ 0
CAN_LO
Text Label 4350 700  2    60   ~ 0
CompressorPressure_5V
Text Label 4350 800  2    60   ~ 0
CompressorPressure_IN
Text Label 4350 900  2    60   ~ 0
CompressorPressure_0V
Text Label 4350 1050 2    60   ~ 0
LeftPressure_5v
Text Label 4350 1150 2    60   ~ 0
LeftPressure_IN
Text Label 4350 1250 2    60   ~ 0
LeftPressure_0V
Text Label 4350 1400 2    60   ~ 0
RightPressure_5V
Text Label 4350 1500 2    60   ~ 0
RightPressure_IN
Text Label 4350 1600 2    60   ~ 0
RightPressure_0V
Text Label 1650 2500 1    60   ~ 0
CompressorPressure_5V
Text Label 1900 2500 1    60   ~ 0
CompressorPressure_IN
Text Label 2150 2500 1    60   ~ 0
CompressorPressure_0V
Text Label 1300 2800 2    60   ~ 0
LeftPressure_5V
Text Label 1300 2950 2    60   ~ 0
LeftPressure_IN
Text Label 2550 2800 0    60   ~ 0
LeftPressure_0V
Text Label 1300 3100 2    60   ~ 0
RightPressure_5V
Text Label 1300 3200 2    60   ~ 0
RightPressure_IN
Text Label 2550 3000 0    60   ~ 0
RightPressure_0V
Text Label 4350 1750 2    60   ~ 0
LeftHeightPot_5V
Text Label 4350 1850 2    60   ~ 0
LeftHeightPot_IN
Text Label 4350 1950 2    60   ~ 0
LeftHeightPot_0V
Text Label 4350 2100 2    60   ~ 0
RightHeightPot_5V
Text Label 4350 2300 2    60   ~ 0
RightHeightPot_0V
Text Label 4350 2200 2    60   ~ 0
RightHeightPot_IN
Text Label 1300 3300 2    60   ~ 0
LeftHeightPot_5V
Text Label 1300 3400 2    60   ~ 0
LeftHeightPot_IN
Text Label 1300 3500 2    60   ~ 0
RightHeightPot_5V
Text Label 1300 3600 2    60   ~ 0
RightHeightPot_IN
Text Label 2550 3200 0    60   ~ 0
LeftHeightPot_0V
Text Label 2550 3400 0    60   ~ 0
RightHeightPot_0V
Text Label 4350 2450 2    60   ~ 0
AnalogSpare_5V
Text Label 4350 2550 2    60   ~ 0
AnalogSpare_IN
Text Label 4350 2650 2    60   ~ 0
AnalogSpare_0V
Text Label 2550 3600 0    60   ~ 0
AnalogSpare_0V
Text Label 1300 3700 2    60   ~ 0
AnalogSpare_5V
Text Label 1300 3800 2    60   ~ 0
AnalogSpare_IN
Text Label 4350 3050 2    60   ~ 0
SpareTemp_5V
Text Label 4350 3150 2    60   ~ 0
SpareTemp_IN
Text Label 4350 2800 2    60   ~ 0
CompressorTemp_5V
Text Label 4350 2900 2    60   ~ 0
CompressorTemp_IN
Text Label 2400 2500 1    60   ~ 0
CompressorTemp_5V
Text Label 2600 2500 1    60   ~ 0
CompressorTemp_IN
Text Notes 650  6100 0    60   ~ 0
Pins L and M is 12A pins \nThe rest is 6A pins\nAs compressor might \nconsume more than 24A we feed it via 3pins\nAlso in total bat feed has 4 pins = 48A max
NoConn ~ 2400 3900
NoConn ~ 2150 3900
NoConn ~ 2400 3700
NoConn ~ 2400 3500
$Sheet
S 9650 850  1350 4450
U 550B97CC
F0 "Microcontroller" 60
F1 "airsuspension_MCU.sch" 60
F2 "Compressor_DIAG" I L 9650 1100 60 
F3 "Compressor_ON" I L 9650 950 60 
F4 "Compressor_Current" I L 9650 1250 60 
F5 "Compressor_Pressure_3.3V" I L 9650 1400 60 
F6 "Airdryer_DIAG" I L 9650 1850 60 
F7 "LeftFill_DIAG" I L 9650 2150 60 
F8 "CAN_TX" I L 9650 5000 60 
F9 "CAN_RX" I L 9650 5150 60 
F10 "Airdryer_ON" I L 9650 1700 60 
F11 "LeftDump_ON" I L 9650 2600 60 
F12 "LeftDump_DIAG" I L 9650 2750 60 
F13 "LeftFill_ON" I L 9650 2000 60 
F14 "RightFill_ON" I L 9650 2300 60 
F15 "RightFill_DIAG" I L 9650 2450 60 
F16 "RightDump_DIAG" I L 9650 3050 60 
F17 "Spare1_ON" I L 9650 3800 60 
F18 "LeftHeight_3.3V" I L 9650 4100 60 
F19 "AnalogSpare_3.3V" I L 9650 4850 60 
F20 "RightHeight_3.3V" I L 9650 4250 60 
F21 "LeftPressure_3.3V" I L 9650 4400 60 
F22 "RightPressure_3.3V" I L 9650 4550 60 
F23 "SpareTemp_3.3V" I L 9650 4700 60 
F24 "RightDump_ON" I L 9650 2900 60 
F25 "CompressorTemp_3.3V" I L 9650 1550 60 
F26 "LeftSuck_ON" I L 9650 3200 60 
F27 "RightSuck_ON" I L 9650 3500 60 
F28 "LeftSuck_DIAG" I L 9650 3350 60 
F29 "RightSuck_DIAG" I L 9650 3650 60 
F30 "Spare1_Diag" I L 9650 3950 60 
$EndSheet
Text Label 6900 800  0    60   ~ 0
CompressorPressure_3.3V
Text Label 6900 1150 0    60   ~ 0
LeftPressure_3.3V
Text Label 6900 1500 0    60   ~ 0
RightPressure_3.3V
Text Label 6900 1850 0    60   ~ 0
LeftHeight_3.3V
Text Label 6900 2200 0    60   ~ 0
RightHeight_3.3V
Text Label 6900 2550 0    60   ~ 0
AnalogSpare_3.3V
Text Label 6900 2850 0    60   ~ 0
CompressorTemp_3.3V
Text Label 6900 3150 0    60   ~ 0
SpareTemp_3.3V
Text Label 6750 3850 0    60   ~ 0
Compressor_ON
Text Label 6750 3950 0    60   ~ 0
Compressor_DIAG
Text Label 6750 4050 0    60   ~ 0
CompressorCurrent_3.3V
Text Label 6750 4200 0    60   ~ 0
LeftFill_ON
Text Label 6750 4300 0    60   ~ 0
LeftFill_DIAG
Text Label 6750 4450 0    60   ~ 0
RightFill_ON
Text Label 6750 4550 0    60   ~ 0
RightFill_DIAG
Text Label 6750 4700 0    60   ~ 0
LeftDump_ON
Text Label 6750 4800 0    60   ~ 0
LeftDump_DIAG
Text Label 6750 4950 0    60   ~ 0
RightDump_ON
Text Label 6750 5050 0    60   ~ 0
RightDump_DIAG
Text Label 6750 5200 0    60   ~ 0
LeftSuck_ON
Text Label 6750 5300 0    60   ~ 0
LeftSuck_DIAG
Text Label 6750 5450 0    60   ~ 0
RightSuck_ON
Text Label 6750 5550 0    60   ~ 0
RightSuck_DIAG
Text Label 6750 5700 0    60   ~ 0
Airdryer_ON
Text Label 6750 5800 0    60   ~ 0
Airdryer_DIAG
Text Label 6750 5950 0    60   ~ 0
Spare1_ON
Text Label 6750 6050 0    60   ~ 0
Spare1_DIAG
Text Label 2750 6700 0    60   ~ 0
CAN_RX
Text Label 2750 6850 0    60   ~ 0
CAN_TX
Text Label 9650 950  2    60   ~ 0
Compressor_ON
Text Label 9650 1100 2    60   ~ 0
Compressor_DIAG
Text Label 9650 1250 2    60   ~ 0
CompressorCurrent_3.3V
Text Label 9650 1400 2    60   ~ 0
CompressorPressure_3.3V
Text Label 9650 1550 2    60   ~ 0
CompressorTemp_3.3V
Text Label 9650 1700 2    60   ~ 0
Airdryer_ON
Text Label 9650 1850 2    60   ~ 0
Airdryer_DIAG
Text Label 9650 2000 2    60   ~ 0
LeftFill_ON
Text Label 9650 2150 2    60   ~ 0
LeftFill_DIAG
Text Label 9650 2300 2    60   ~ 0
RightFill_ON
Text Label 9650 2450 2    60   ~ 0
RightFill_DIAG
Text Label 9650 2600 2    60   ~ 0
LeftDump_ON
Text Label 9650 2750 2    60   ~ 0
LeftDump_DIAG
Text Label 9650 2900 2    60   ~ 0
RightDump_ON
Text Label 9650 3050 2    60   ~ 0
RightDump_DIAG
Text Label 9650 3200 2    60   ~ 0
LeftSuck_ON
Text Label 9650 3350 2    60   ~ 0
LeftSuck_DIAG
Text Label 9650 3500 2    60   ~ 0
RightSuck_ON
Text Label 9650 3650 2    60   ~ 0
RightSuck_DIAG
Text Label 9650 3800 2    60   ~ 0
Spare1_ON
Text Label 9650 3950 2    60   ~ 0
Spare1_DIAG
Text Label 9650 4100 2    60   ~ 0
LeftHeight_3.3V
Text Label 9650 4250 2    60   ~ 0
RightHeight_3.3V
Text Label 9650 4400 2    60   ~ 0
LeftPressure_3.3V
Text Label 9650 4550 2    60   ~ 0
RightPressure_3.3V
Text Label 9650 4700 2    60   ~ 0
SpareTemp_3.3V
Text Label 9650 4850 2    60   ~ 0
AnalogSpare_3.3V
Text Label 9650 5000 2    60   ~ 0
CAN_TX
Text Label 9650 5150 2    60   ~ 0
CAN_RX
$Comp
L PWR_FLAG #FLG03
U 1 1 550EDDE0
P 2150 5350
F 0 "#FLG03" H 2150 5445 30  0001 C CNN
F 1 "PWR_FLAG" H 2150 5530 30  0000 C CNN
F 2 "" H 2150 5350 60  0000 C CNN
F 3 "" H 2150 5350 60  0000 C CNN
	1    2150 5350
	0    1    1    0   
$EndComp
Text Label 2550 3100 0    60   ~ 0
SpareTemp_5V
Text Label 2550 3300 0    60   ~ 0
SpareTemp_IN
$Comp
L PWR_FLAG #FLG04
U 1 1 550ECFBB
P 2650 4850
F 0 "#FLG04" H 2650 4945 30  0001 C CNN
F 1 "PWR_FLAG" H 2650 5030 30  0000 C CNN
F 2 "" H 2650 4850 60  0000 C CNN
F 3 "" H 2650 4850 60  0000 C CNN
	1    2650 4850
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 550EB261
P 3350 4150
F 0 "#FLG05" H 3350 4245 30  0001 C CNN
F 1 "PWR_FLAG" H 3350 4330 30  0000 C CNN
F 2 "" H 3350 4150 60  0000 C CNN
F 3 "" H 3350 4150 60  0000 C CNN
	1    3350 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 5000 1650 5050
Wire Wire Line
	1650 5050 2400 5050
Wire Wire Line
	2400 5050 2400 5000
Wire Wire Line
	2150 5000 2150 5350
Connection ~ 2150 5050
Wire Wire Line
	1900 5000 1900 5050
Connection ~ 1900 5050
Wire Wire Line
	1650 4750 1650 4850
Wire Wire Line
	1300 4850 2150 4850
Wire Wire Line
	1900 4850 1900 4750
Wire Wire Line
	2150 4850 2150 4750
Wire Wire Line
	2400 4750 2400 4850
Wire Wire Line
	2400 4850 2650 4850
Connection ~ 1650 4850
Connection ~ 1900 4850
Wire Wire Line
	1650 4500 1300 4500
Wire Wire Line
	1900 4500 1900 4600
Wire Wire Line
	1900 4600 1300 4600
Wire Wire Line
	2400 4500 2600 4500
Wire Wire Line
	2600 4600 2150 4600
Wire Wire Line
	2150 4600 2150 4500
Wire Wire Line
	2600 4300 2400 4300
Wire Wire Line
	2600 4400 2150 4400
Wire Wire Line
	2150 4400 2150 4300
Wire Wire Line
	1650 4300 1300 4300
Wire Wire Line
	1900 4300 1900 4400
Wire Wire Line
	1900 4400 1300 4400
Wire Wire Line
	3350 4150 2400 4150
Wire Wire Line
	2400 4150 2400 4100
Wire Wire Line
	1650 4100 1650 4150
Wire Wire Line
	1650 4150 1300 4150
Wire Wire Line
	1650 3900 1300 3900
Wire Wire Line
	1900 3900 1900 4000
Wire Wire Line
	1900 4000 1300 4000
Wire Wire Line
	2150 2500 2150 2700
Wire Wire Line
	1900 2500 1900 2700
Wire Wire Line
	1650 2500 1650 2700
Wire Wire Line
	1900 2900 1900 2950
Wire Wire Line
	1900 2950 1300 2950
Wire Wire Line
	2150 2900 2150 2800
Wire Wire Line
	2150 2800 2550 2800
Wire Wire Line
	1650 2900 1650 2800
Wire Wire Line
	1650 2800 1300 2800
Wire Wire Line
	1650 3100 1300 3100
Wire Wire Line
	1900 3100 1900 3150
Wire Wire Line
	1900 3150 1300 3150
Wire Wire Line
	1300 3150 1300 3200
Wire Wire Line
	2550 3000 2150 3000
Wire Wire Line
	2150 3000 2150 3100
Wire Wire Line
	1650 3300 1300 3300
Wire Wire Line
	1900 3300 1900 3350
Wire Wire Line
	1900 3350 1300 3350
Wire Wire Line
	1300 3350 1300 3400
Wire Wire Line
	2550 3200 2150 3200
Wire Wire Line
	2150 3200 2150 3300
Wire Wire Line
	2550 3400 2150 3400
Wire Wire Line
	2150 3400 2150 3500
Wire Wire Line
	1650 3500 1300 3500
Wire Wire Line
	1900 3500 1900 3550
Wire Wire Line
	1900 3550 1300 3550
Wire Wire Line
	1300 3550 1300 3600
Wire Wire Line
	1650 3700 1300 3700
Wire Wire Line
	1900 3700 1900 3750
Wire Wire Line
	1900 3750 1300 3750
Wire Wire Line
	1300 3750 1300 3800
Wire Wire Line
	2550 3600 2150 3600
Wire Wire Line
	2150 3600 2150 3700
Wire Wire Line
	2400 2700 2400 2500
Wire Wire Line
	2600 2500 2600 2550
Wire Wire Line
	2600 2550 2450 2550
Wire Wire Line
	2450 2550 2450 2900
Wire Wire Line
	2450 2900 2400 2900
Wire Wire Line
	2550 3100 2400 3100
Wire Wire Line
	2550 3300 2400 3300
$EndSCHEMATC
