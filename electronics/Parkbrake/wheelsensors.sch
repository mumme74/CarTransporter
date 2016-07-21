EESchema Schematic File Version 2
LIBS:Parkbrake-rescue
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
LIBS:cinch
LIBS:miscellaneous
LIBS:teensy
LIBS:power_switches
LIBS:stm32
LIBS:Parkbrake-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 10 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 2675 2450 2    60   Input ~ 0
R_wheelsensor_out+
Text GLabel 2675 1225 2    60   Input ~ 0
L_wheelsensor_out+
Text GLabel 2675 3025 2    60   Input ~ 0
R_wheelsensor_out-
Text GLabel 2650 1750 2    60   Input ~ 0
L_wheelsensor_out-
Text GLabel 2150 1225 0    60   Input ~ 0
LF_wheelsensor_in+
Text GLabel 2150 1750 0    60   Input ~ 0
LF_wheelsensor_in-
Text GLabel 6525 2600 0    60   Input ~ 0
LR_wheelsensor_in+
Text GLabel 6500 3200 0    60   Input ~ 0
LR_wheelsensor_in-
Text GLabel 2150 2450 0    60   Input ~ 0
RF_wheelsensor_in+
Text GLabel 2150 3025 0    60   Input ~ 0
RF_wheelsensor_in-
Text GLabel 6525 2800 0    60   Input ~ 0
RR_wheelsensor_in+
Text GLabel 6500 3425 0    60   Input ~ 0
RR_wheelsensor_in-
Text Label 2150 1050 2    60   ~ 0
L_sensor_interpolated_out+
Text Label 2150 1600 2    60   ~ 0
L_sensor_interpolated_out-
Text Label 2150 2275 2    60   ~ 0
R_sensor_interpolated_out+
Text Label 2150 2850 2    60   ~ 0
R_sensor_interpolated_out-
Text Notes 1075 900  0    60   ~ 0
Select software calculated speed sensor out or passtrough\nPasstrough is default
$Comp
L NCV1124 U?
U 1 1 57900E18
P 8000 2700
F 0 "U?" H 8250 3200 60  0000 C CNN
F 1 "NCV1124" H 7900 3200 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 8150 3800 60  0001 C CNN
F 3 "" H 8150 3800 60  0000 C CNN
F 4 "NCV1124DR2G" H 8000 2700 60  0001 C CNN "part"
F 5 "2534601" H 8000 2700 60  0001 C CNN "farnell"
	1    8000 2700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57900EFD
P 9900 3425
F 0 "C?" H 9925 3525 50  0000 L CNN
F 1 "100n" H 9925 3325 50  0000 L CNN
F 2 "" H 9938 3275 50  0000 C CNN
F 3 "" H 9900 3425 50  0000 C CNN
	1    9900 3425
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57901042
P 10150 3425
F 0 "C?" H 10175 3525 50  0000 L CNN
F 1 "10n" H 10175 3325 50  0000 L CNN
F 2 "" H 10188 3275 50  0000 C CNN
F 3 "" H 10150 3425 50  0000 C CNN
	1    10150 3425
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579010A2
P 9650 3425
F 0 "C?" H 9675 3525 50  0000 L CNN
F 1 "10u" H 9675 3325 50  0000 L CNN
F 2 "" H 9688 3275 50  0000 C CNN
F 3 "" H 9650 3425 50  0000 C CNN
	1    9650 3425
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 579010CB
P 10150 3575
F 0 "#PWR?" H 10150 3325 50  0001 C CNN
F 1 "GND" H 10150 3425 50  0000 C CNN
F 2 "" H 10150 3575 50  0000 C CNN
F 3 "" H 10150 3575 50  0000 C CNN
	1    10150 3575
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57901107
P 7550 3000
F 0 "#PWR?" H 7550 2750 50  0001 C CNN
F 1 "GND" H 7550 2850 50  0000 C CNN
F 2 "" H 7550 3000 50  0000 C CNN
F 3 "" H 7550 3000 50  0000 C CNN
	1    7550 3000
	1    0    0    -1  
$EndComp
Text GLabel 10150 3275 1    60   Input ~ 0
+5V
Text GLabel 8500 2400 2    60   Input ~ 0
+5V
$Comp
L R R?
U 1 1 5790137C
P 8650 2600
F 0 "R?" V 8730 2600 50  0000 C CNN
F 1 "1k" V 8650 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8580 2600 50  0001 C CNN
F 3 "" H 8650 2600 50  0000 C CNN
F 4 "MC01W080511K" V 8650 2600 60  0001 C CNN "part"
F 5 "9332383" V 8650 2600 60  0001 C CNN "farnell"
	1    8650 2600
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57901405
P 8650 2800
F 0 "R?" V 8730 2800 50  0000 C CNN
F 1 "1k" V 8650 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8580 2800 50  0001 C CNN
F 3 "" H 8650 2800 50  0000 C CNN
F 4 "MC01W080511K" V 8650 2800 60  0001 C CNN "part"
F 5 "9332383" V 8650 2800 60  0001 C CNN "farnell"
	1    8650 2800
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57901432
P 8950 3075
F 0 "R?" V 9030 3075 50  0000 C CNN
F 1 "2k" V 8950 3075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8880 3075 50  0001 C CNN
F 3 "" H 8950 3075 50  0000 C CNN
F 4 "MC01W080512K" V 8950 3075 60  0001 C CNN "part"
F 5 "9332766" V 8950 3075 60  0001 C CNN "farnell"
	1    8950 3075
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57901469
P 9175 3075
F 0 "R?" V 9255 3075 50  0000 C CNN
F 1 "2k" V 9175 3075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9105 3075 50  0001 C CNN
F 3 "" H 9175 3075 50  0000 C CNN
F 4 "MC01W080512K" V 9175 3075 60  0001 C CNN "part"
F 5 "9332766" V 9175 3075 60  0001 C CNN "farnell"
	1    9175 3075
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5790159A
P 9175 3225
F 0 "#PWR?" H 9175 2975 50  0001 C CNN
F 1 "GND" H 9175 3075 50  0000 C CNN
F 2 "" H 9175 3225 50  0000 C CNN
F 3 "" H 9175 3225 50  0000 C CNN
	1    9175 3225
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5790171F
P 7400 2400
F 0 "R?" V 7325 2400 50  0000 C CNN
F 1 "16.9k" V 7400 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7330 2400 50  0001 C CNN
F 3 "" H 7400 2400 50  0001 C CNN
F 4 "MC01W0805116K9" V 7400 2400 60  0001 C CNN "part"
F 5 "2141858" V 7400 2400 60  0001 C CNN "farnell"
	1    7400 2400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 579017A6
P 7250 2400
F 0 "#PWR?" H 7250 2150 50  0001 C CNN
F 1 "GND" H 7250 2250 50  0001 C CNN
F 2 "" H 7250 2400 50  0000 C CNN
F 3 "" H 7250 2400 50  0000 C CNN
	1    7250 2400
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57901906
P 6675 2600
F 0 "R?" V 6755 2600 50  0000 C CNN
F 1 "15k" V 6675 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6605 2600 50  0001 C CNN
F 3 "" H 6675 2600 50  0000 C CNN
F 4 "9332618" V 6675 2600 60  0001 C CNN "farnell"
F 5 "MC01W0805115K" V 6675 2600 60  0001 C CNN "part"
	1    6675 2600
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57901951
P 6675 2800
F 0 "R?" V 6755 2800 50  0000 C CNN
F 1 "15k" V 6675 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6605 2800 50  0001 C CNN
F 3 "" H 6675 2800 50  0000 C CNN
F 4 "9332618" V 6675 2800 60  0001 C CNN "farnell"
F 5 "MC01W0805115K" V 6675 2800 60  0001 C CNN "part"
	1    6675 2800
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 5790199C
P 6925 3050
F 0 "C?" H 6950 3150 50  0000 L CNN
F 1 "47n" H 6950 2950 50  0000 L CNN
F 2 "" H 6963 2900 50  0000 C CNN
F 3 "" H 6925 3050 50  0000 C CNN
	1    6925 3050
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57901A21
P 7200 3050
F 0 "C?" H 7225 3150 50  0000 L CNN
F 1 "47n" H 7225 2950 50  0000 L CNN
F 2 "" H 7238 2900 50  0000 C CNN
F 3 "" H 7200 3050 50  0000 C CNN
	1    7200 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57901C47
P 7200 3200
F 0 "#PWR?" H 7200 2950 50  0001 C CNN
F 1 "GND" H 7200 3050 50  0000 C CNN
F 2 "" H 7200 3200 50  0000 C CNN
F 3 "" H 7200 3200 50  0000 C CNN
	1    7200 3200
	1    0    0    -1  
$EndComp
Text Notes 6325 2475 0    60   ~ 0
1/2W resistors
Text Label 8500 3000 3    60   ~ 0
VR_diag
$Comp
L Q_PMOS_GSD Q?
U 1 1 579020F0
P 10075 4775
F 0 "Q?" V 10300 4600 50  0000 R CNN
F 1 "Q_PMOS_GSD" V 10425 5000 50  0000 R CNN
F 2 "" H 10275 4875 50  0000 C CNN
F 3 "" H 10075 4775 50  0000 C CNN
	1    10075 4775
	0    1    -1   0   
$EndComp
$Comp
L BSS138 Q?
U 1 1 579022F3
P 9975 5325
F 0 "Q?" H 10175 5400 50  0000 L CNN
F 1 "BSS138" H 10175 5325 50  0000 L CNN
F 2 "SOT-23" H 10175 5250 50  0000 L CIN
F 3 "" H 9975 5325 50  0000 L CNN
	1    9975 5325
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57902558
P 9750 4825
F 0 "R?" V 9830 4825 50  0000 C CNN
F 1 "10k" V 9750 4825 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9680 4825 50  0001 C CNN
F 3 "" H 9750 4825 50  0000 C CNN
F 4 "9332391" V 9750 4825 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" V 9750 4825 60  0001 C CNN "part"
	1    9750 4825
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57902641
P 10075 5525
F 0 "#PWR?" H 10075 5275 50  0001 C CNN
F 1 "GND" H 10075 5375 50  0000 C CNN
F 2 "" H 10075 5525 50  0000 C CNN
F 3 "" H 10075 5525 50  0000 C CNN
	1    10075 5525
	1    0    0    -1  
$EndComp
Text Label 10275 4675 0    60   ~ 0
VR_diag
Text GLabel 9650 4675 0    60   Input ~ 0
+5V
Text GLabel 9775 5375 0    60   Input ~ 0
VR_diag_test
Text GLabel 9375 2600 2    60   Input ~ 0
LR_speed_in
Text GLabel 9375 2800 2    60   Input ~ 0
RR_speed_in
$Comp
L NCV1124 U?
U 1 1 579039B4
P 8000 1375
F 0 "U?" H 8250 1875 60  0000 C CNN
F 1 "NCV1124" H 7900 1875 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 8150 2475 60  0001 C CNN
F 3 "" H 8150 2475 60  0000 C CNN
F 4 "NCV1124DR2G" H 8000 1375 60  0001 C CNN "part"
F 5 "2534601" H 8000 1375 60  0001 C CNN "farnell"
	1    8000 1375
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579039BA
P 9850 2025
F 0 "C?" H 9875 2125 50  0000 L CNN
F 1 "100n" H 9875 1925 50  0000 L CNN
F 2 "" H 9888 1875 50  0000 C CNN
F 3 "" H 9850 2025 50  0000 C CNN
	1    9850 2025
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579039C0
P 10100 2025
F 0 "C?" H 10125 2125 50  0000 L CNN
F 1 "10n" H 10125 1925 50  0000 L CNN
F 2 "" H 10138 1875 50  0000 C CNN
F 3 "" H 10100 2025 50  0000 C CNN
	1    10100 2025
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579039C6
P 9600 2025
F 0 "C?" H 9625 2125 50  0000 L CNN
F 1 "10u" H 9625 1925 50  0000 L CNN
F 2 "" H 9638 1875 50  0000 C CNN
F 3 "" H 9600 2025 50  0000 C CNN
	1    9600 2025
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 579039CC
P 10100 2175
F 0 "#PWR?" H 10100 1925 50  0001 C CNN
F 1 "GND" H 10100 2025 50  0000 C CNN
F 2 "" H 10100 2175 50  0000 C CNN
F 3 "" H 10100 2175 50  0000 C CNN
	1    10100 2175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 579039D2
P 7550 1675
F 0 "#PWR?" H 7550 1425 50  0001 C CNN
F 1 "GND" H 7550 1525 50  0000 C CNN
F 2 "" H 7550 1675 50  0000 C CNN
F 3 "" H 7550 1675 50  0000 C CNN
	1    7550 1675
	1    0    0    -1  
$EndComp
Text GLabel 10100 1875 1    60   Input ~ 0
+5V
Text GLabel 8500 1075 2    60   Input ~ 0
+5V
$Comp
L R R?
U 1 1 579039DE
P 8650 1275
F 0 "R?" V 8730 1275 50  0000 C CNN
F 1 "1k" V 8650 1275 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8580 1275 50  0001 C CNN
F 3 "" H 8650 1275 50  0000 C CNN
F 4 "MC01W080511K" V 8650 1275 60  0001 C CNN "part"
F 5 "9332383" V 8650 1275 60  0001 C CNN "farnell"
	1    8650 1275
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 579039E4
P 8650 1475
F 0 "R?" V 8730 1475 50  0000 C CNN
F 1 "1k" V 8650 1475 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8580 1475 50  0001 C CNN
F 3 "" H 8650 1475 50  0000 C CNN
F 4 "MC01W080511K" V 8650 1475 60  0001 C CNN "part"
F 5 "9332383" V 8650 1475 60  0001 C CNN "farnell"
	1    8650 1475
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 579039EA
P 8950 1750
F 0 "R?" V 9030 1750 50  0000 C CNN
F 1 "2k" V 8950 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8880 1750 50  0001 C CNN
F 3 "" H 8950 1750 50  0000 C CNN
F 4 "MC01W080512K" V 8950 1750 60  0001 C CNN "part"
F 5 "9332766" V 8950 1750 60  0001 C CNN "farnell"
	1    8950 1750
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 579039F0
P 9175 1750
F 0 "R?" V 9255 1750 50  0000 C CNN
F 1 "2k" V 9175 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9105 1750 50  0001 C CNN
F 3 "" H 9175 1750 50  0000 C CNN
F 4 "MC01W080512K" V 9175 1750 60  0001 C CNN "part"
F 5 "9332766" V 9175 1750 60  0001 C CNN "farnell"
	1    9175 1750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 579039FA
P 9175 1900
F 0 "#PWR?" H 9175 1650 50  0001 C CNN
F 1 "GND" H 9175 1750 50  0000 C CNN
F 2 "" H 9175 1900 50  0000 C CNN
F 3 "" H 9175 1900 50  0000 C CNN
	1    9175 1900
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57903A03
P 7400 1075
F 0 "R?" V 7325 1075 50  0000 C CNN
F 1 "16.9k" V 7400 1075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7330 1075 50  0001 C CNN
F 3 "" H 7400 1075 50  0000 C CNN
F 4 "MC01W0805116K9" V 7400 1075 60  0001 C CNN "part"
F 5 "2141858" V 7400 1075 60  0001 C CNN "farnell"
	1    7400 1075
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 57903A09
P 7250 1075
F 0 "#PWR?" H 7250 825 50  0001 C CNN
F 1 "GND" H 7250 925 50  0001 C CNN
F 2 "" H 7250 1075 50  0000 C CNN
F 3 "" H 7250 1075 50  0000 C CNN
	1    7250 1075
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57903A0F
P 6675 1275
F 0 "R?" V 6755 1275 50  0000 C CNN
F 1 "15k" V 6675 1275 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6605 1275 50  0001 C CNN
F 3 "" H 6675 1275 50  0000 C CNN
F 4 "MC01W0805115K" V 6675 1275 60  0001 C CNN "part"
F 5 "9332618" V 6675 1275 60  0001 C CNN "farnell"
	1    6675 1275
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57903A15
P 6675 1475
F 0 "R?" V 6755 1475 50  0000 C CNN
F 1 "15k" V 6675 1475 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6605 1475 50  0001 C CNN
F 3 "" H 6675 1475 50  0000 C CNN
F 4 "9332618" V 6675 1475 60  0001 C CNN "farnell"
F 5 "MC01W0805115K" V 6675 1475 60  0001 C CNN "part"
	1    6675 1475
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57903A1B
P 6925 1725
F 0 "C?" H 6950 1825 50  0000 L CNN
F 1 "47n" H 6950 1625 50  0000 L CNN
F 2 "" H 6963 1575 50  0000 C CNN
F 3 "" H 6925 1725 50  0000 C CNN
	1    6925 1725
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57903A21
P 7200 1725
F 0 "C?" H 7225 1825 50  0000 L CNN
F 1 "47n" H 7225 1625 50  0000 L CNN
F 2 "" H 7238 1575 50  0000 C CNN
F 3 "" H 7200 1725 50  0000 C CNN
	1    7200 1725
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57903A27
P 7200 1875
F 0 "#PWR?" H 7200 1625 50  0001 C CNN
F 1 "GND" H 7200 1725 50  0000 C CNN
F 2 "" H 7200 1875 50  0000 C CNN
F 3 "" H 7200 1875 50  0000 C CNN
	1    7200 1875
	1    0    0    -1  
$EndComp
Text Notes 6325 1150 0    60   ~ 0
1/2W resistors
Text Label 8500 1675 3    60   ~ 0
VR_diag
Text GLabel 9375 1275 2    60   Input ~ 0
LF_speed_in
Text GLabel 9375 1475 2    60   Input ~ 0
RF_speed_in
$Comp
L Jumper_NC_Small JP?
U 1 1 57903C51
P 2425 1225
F 0 "JP?" H 2425 1305 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2435 1165 50  0001 C CNN
F 2 "" H 2425 1225 50  0000 C CNN
F 3 "" H 2425 1225 50  0000 C CNN
	1    2425 1225
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP?
U 1 1 57903D98
P 2425 1400
F 0 "JP?" H 2425 1480 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 1340 50  0001 C CNN
F 2 "" H 2425 1400 50  0000 C CNN
F 3 "" H 2425 1400 50  0000 C CNN
	1    2425 1400
	1    0    0    -1  
$EndComp
Text Label 2675 1400 0    60   ~ 0
LF_wheelsensor+
$Comp
L Jumper_NO_Small JP?
U 1 1 57904332
P 2425 1050
F 0 "JP?" H 2425 1130 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 990 50  0001 C CNN
F 2 "" H 2425 1050 50  0000 C CNN
F 3 "" H 2425 1050 50  0000 C CNN
	1    2425 1050
	1    0    0    -1  
$EndComp
Text Label 2650 1925 0    60   ~ 0
LF_wheelsensor-
$Comp
L Jumper_NO_Small JP?
U 1 1 579049E5
P 2425 1600
F 0 "JP?" H 2425 1680 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 1540 50  0001 C CNN
F 2 "" H 2425 1600 50  0000 C CNN
F 3 "" H 2425 1600 50  0000 C CNN
	1    2425 1600
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP?
U 1 1 57904A62
P 2425 1925
F 0 "JP?" H 2425 2005 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 1865 50  0001 C CNN
F 2 "" H 2425 1925 50  0000 C CNN
F 3 "" H 2425 1925 50  0000 C CNN
	1    2425 1925
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NC_Small JP?
U 1 1 57904AC7
P 2425 1750
F 0 "JP?" H 2425 1830 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2435 1690 50  0001 C CNN
F 2 "" H 2425 1750 50  0000 C CNN
F 3 "" H 2425 1750 50  0000 C CNN
	1    2425 1750
	1    0    0    -1  
$EndComp
Text Label 2675 2625 0    60   ~ 0
RF_wheelsensor+
$Comp
L Jumper_NO_Small JP?
U 1 1 579051C2
P 2425 2275
F 0 "JP?" H 2425 2355 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 2215 50  0001 C CNN
F 2 "" H 2425 2275 50  0000 C CNN
F 3 "" H 2425 2275 50  0000 C CNN
	1    2425 2275
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP?
U 1 1 57905290
P 2425 2625
F 0 "JP?" H 2425 2705 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 2565 50  0001 C CNN
F 2 "" H 2425 2625 50  0000 C CNN
F 3 "" H 2425 2625 50  0000 C CNN
	1    2425 2625
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NC_Small JP?
U 1 1 579052F9
P 2425 2450
F 0 "JP?" H 2425 2530 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2435 2390 50  0001 C CNN
F 2 "" H 2425 2450 50  0000 C CNN
F 3 "" H 2425 2450 50  0000 C CNN
	1    2425 2450
	1    0    0    -1  
$EndComp
Text Label 2675 3200 0    60   ~ 0
RF_wheelsensor-
$Comp
L Jumper_NC_Small JP?
U 1 1 57905A59
P 2425 3025
F 0 "JP?" H 2425 3105 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2435 2965 50  0001 C CNN
F 2 "" H 2425 3025 50  0000 C CNN
F 3 "" H 2425 3025 50  0000 C CNN
	1    2425 3025
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP?
U 1 1 57905B4F
P 2425 2850
F 0 "JP?" H 2425 2930 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 2790 50  0001 C CNN
F 2 "" H 2425 2850 50  0000 C CNN
F 3 "" H 2425 2850 50  0000 C CNN
	1    2425 2850
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP?
U 1 1 57905C2E
P 2425 3200
F 0 "JP?" H 2425 3280 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2435 3140 50  0001 C CNN
F 2 "" H 2425 3200 50  0000 C CNN
F 3 "" H 2425 3200 50  0000 C CNN
	1    2425 3200
	1    0    0    -1  
$EndComp
Text Label 6525 1275 2    60   ~ 0
LF_wheelsensor+
Text Label 6525 1475 2    60   ~ 0
RF_wheelsensor+
Text Label 6500 1875 2    60   ~ 0
LF_wheelsensor-
Text Label 6500 2100 2    60   ~ 0
RF_wheelsensor-
Text Label 2850 6725 2    60   ~ 0
L_sensor_interpolated_out-
Text Label 2850 6575 2    60   ~ 0
L_sensor_interpolated_out+
$Comp
L R R?
U 1 1 57907257
P 3125 6875
F 0 "R?" V 3205 6875 50  0000 C CNN
F 1 "1k" V 3125 6875 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3055 6875 50  0001 C CNN
F 3 "" H 3125 6875 50  0000 C CNN
F 4 "MC01W080511K" V 3125 6875 60  0001 C CNN "part"
F 5 "9332383" V 3125 6875 60  0001 C CNN "farnell"
	1    3125 6875
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 57907490
P 3350 6875
F 0 "R?" V 3430 6875 50  0000 C CNN
F 1 "1k" V 3350 6875 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3280 6875 50  0001 C CNN
F 3 "" H 3350 6875 50  0000 C CNN
F 4 "MC01W080511K" V 3350 6875 60  0001 C CNN "part"
F 5 "9332383" V 3350 6875 60  0001 C CNN "farnell"
	1    3350 6875
	-1   0    0    1   
$EndComp
$Comp
L MAX232 U?
U 1 1 579078A3
P 2325 5650
F 0 "U?" H 1875 6500 50  0000 L CNN
F 1 "MAX232" H 2525 6500 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-16_4.4x5mm_Pitch0.65mm" H 2325 5650 50  0001 C CNN
F 3 "" H 2325 5650 50  0000 C CNN
F 4 "2335478" H 2325 5650 60  0001 C CNN "farnell"
F 5 "MAX202IPW" H 2325 5650 60  0001 C CNN "part"
	1    2325 5650
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57907954
P 3225 5100
F 0 "C?" H 3250 5200 50  0000 L CNN
F 1 "1u" H 3250 5000 50  0000 L CNN
F 2 "" H 3263 4950 50  0000 C CNN
F 3 "" H 3225 5100 50  0000 C CNN
	1    3225 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57907ABF
P 3075 5550
F 0 "#PWR?" H 3075 5300 50  0001 C CNN
F 1 "GND" H 3075 5400 50  0001 C CNN
F 2 "" H 3075 5550 50  0000 C CNN
F 3 "" H 3075 5550 50  0000 C CNN
	1    3075 5550
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57907B2D
P 3075 5350
F 0 "C?" H 3100 5450 50  0000 L CNN
F 1 "1u" H 3100 5250 50  0000 L CNN
F 2 "" H 3113 5200 50  0000 C CNN
F 3 "" H 3075 5350 50  0000 C CNN
	1    3075 5350
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57907BDA
P 3075 5850
F 0 "C?" H 3100 5950 50  0000 L CNN
F 1 "1u" H 3100 5750 50  0000 L CNN
F 2 "" H 3113 5700 50  0000 C CNN
F 3 "" H 3075 5850 50  0000 C CNN
	1    3075 5850
	0    1    1    0   
$EndComp
Wire Wire Line
	9650 3575 10150 3575
Connection ~ 9900 3575
Wire Wire Line
	9650 3275 10150 3275
Connection ~ 9900 3275
Wire Wire Line
	8950 2925 8950 2800
Wire Wire Line
	8800 2800 9375 2800
Wire Wire Line
	8800 2600 9375 2600
Wire Wire Line
	9175 2600 9175 2925
Wire Wire Line
	8950 3225 9175 3225
Connection ~ 8950 2800
Connection ~ 9175 2600
Wire Wire Line
	6500 3200 7200 3200
Wire Wire Line
	6825 2600 7550 2600
Wire Wire Line
	6825 2800 7550 2800
Wire Wire Line
	7200 2900 7200 2600
Connection ~ 7200 2600
Wire Wire Line
	6925 2900 6925 2800
Connection ~ 6925 2800
Connection ~ 6925 3200
Wire Wire Line
	6500 3425 6500 3200
Wire Wire Line
	9650 4675 9875 4675
Connection ~ 9750 4675
Wire Wire Line
	9750 4975 10075 4975
Wire Wire Line
	10075 4975 10075 5125
Wire Wire Line
	9600 2175 10100 2175
Connection ~ 9850 2175
Wire Wire Line
	9600 1875 10100 1875
Connection ~ 9850 1875
Wire Wire Line
	8950 1600 8950 1475
Wire Wire Line
	8800 1475 9375 1475
Wire Wire Line
	8800 1275 9375 1275
Wire Wire Line
	9175 1275 9175 1600
Wire Wire Line
	8950 1900 9175 1900
Connection ~ 8950 1475
Connection ~ 9175 1275
Wire Wire Line
	6500 1875 7200 1875
Wire Wire Line
	6825 1275 7550 1275
Wire Wire Line
	6825 1475 7550 1475
Wire Wire Line
	7200 1575 7200 1275
Connection ~ 7200 1275
Wire Wire Line
	6925 1575 6925 1475
Connection ~ 6925 1475
Connection ~ 6925 1875
Wire Wire Line
	6500 2100 6500 1875
Wire Wire Line
	2525 1050 2525 1225
Wire Wire Line
	2525 1225 2675 1225
Wire Wire Line
	2325 1050 2150 1050
Wire Wire Line
	2325 1225 2150 1225
Wire Wire Line
	2325 1225 2325 1400
Wire Wire Line
	2525 1400 2675 1400
Wire Wire Line
	2325 1600 2150 1600
Wire Wire Line
	2525 1600 2525 1750
Wire Wire Line
	2525 1750 2650 1750
Wire Wire Line
	2325 1750 2150 1750
Wire Wire Line
	2325 1750 2325 1925
Wire Wire Line
	2525 1925 2650 1925
Wire Wire Line
	2325 2275 2150 2275
Wire Wire Line
	2525 2275 2525 2450
Wire Wire Line
	2525 2450 2675 2450
Wire Wire Line
	2675 2625 2525 2625
Wire Wire Line
	2325 2450 2325 2625
Wire Wire Line
	2325 2450 2150 2450
Wire Wire Line
	2675 3025 2525 3025
Wire Wire Line
	2325 3025 2150 3025
Wire Wire Line
	2325 3200 2325 3025
Wire Wire Line
	2525 3200 2675 3200
Wire Wire Line
	2525 3025 2525 2850
Wire Wire Line
	2325 2850 2150 2850
Wire Wire Line
	3225 5550 2925 5550
Wire Wire Line
	3225 5250 3225 5850
Connection ~ 3075 5550
Connection ~ 3225 5550
Wire Wire Line
	3225 4950 2925 4950
Connection ~ 3225 5350
$Comp
L C C?
U 1 1 57908438
P 1575 5150
F 0 "C?" H 1600 5250 50  0000 L CNN
F 1 "1u" H 1600 5050 50  0000 L CNN
F 2 "" H 1613 5000 50  0000 C CNN
F 3 "" H 1575 5150 50  0000 C CNN
	1    1575 5150
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579084C7
P 1575 5650
F 0 "C?" H 1600 5750 50  0000 L CNN
F 1 "1u" H 1600 5550 50  0000 L CNN
F 2 "" H 1613 5500 50  0000 C CNN
F 3 "" H 1575 5650 50  0000 C CNN
	1    1575 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1725 4950 1575 4950
Wire Wire Line
	1575 4950 1575 5000
Wire Wire Line
	1725 5350 1575 5350
Wire Wire Line
	1575 5350 1575 5300
Wire Wire Line
	1725 5450 1575 5450
Wire Wire Line
	1575 5450 1575 5500
Wire Wire Line
	1725 5850 1575 5850
Wire Wire Line
	1575 5850 1575 5800
Text GLabel 3225 4950 1    60   Input ~ 0
+5V
$Comp
L C C?
U 1 1 57909216
P 3125 6300
F 0 "C?" H 3150 6400 50  0000 L CNN
F 1 "1u" H 3150 6200 50  0000 L CNN
F 2 "" H 3163 6150 50  0000 C CNN
F 3 "" H 3125 6300 50  0000 C CNN
	1    3125 6300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579093B9
P 3350 6300
F 0 "C?" H 3375 6400 50  0000 L CNN
F 1 "1u" H 3375 6200 50  0000 L CNN
F 2 "" H 3388 6150 50  0000 C CNN
F 3 "" H 3350 6300 50  0000 C CNN
	1    3350 6300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5790981F
P 3125 7025
F 0 "#PWR?" H 3125 6775 50  0001 C CNN
F 1 "GND" H 3125 6875 50  0000 C CNN
F 2 "" H 3125 7025 50  0000 C CNN
F 3 "" H 3125 7025 50  0000 C CNN
	1    3125 7025
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5790996D
P 2925 6350
F 0 "#PWR?" H 2925 6100 50  0001 C CNN
F 1 "GND" H 2925 6200 50  0001 C CNN
F 2 "" H 2925 6350 50  0000 C CNN
F 3 "" H 2925 6350 50  0000 C CNN
	1    2925 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2925 6350 2925 6250
Wire Wire Line
	2925 6150 3125 6150
Wire Wire Line
	2925 6050 3350 6050
Wire Wire Line
	3350 6050 3350 6150
Wire Wire Line
	3350 7025 3125 7025
Wire Wire Line
	3350 6450 3350 6725
NoConn ~ 1725 6350
NoConn ~ 1725 6250
Wire Wire Line
	3125 6575 2850 6575
Wire Wire Line
	3125 6450 3125 6725
Connection ~ 3125 6575
Wire Wire Line
	3350 6725 2850 6725
Text GLabel 1725 6150 0    60   Input ~ 0
L_speed_out+
Text Label 5900 6725 2    60   ~ 0
R_sensor_interpolated_out-
Text Label 5900 6575 2    60   ~ 0
R_sensor_interpolated_out+
$Comp
L R R?
U 1 1 5790C71F
P 6175 6875
F 0 "R?" V 6255 6875 50  0000 C CNN
F 1 "1k" V 6175 6875 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6105 6875 50  0001 C CNN
F 3 "" H 6175 6875 50  0000 C CNN
F 4 "MC01W080511K" V 6175 6875 60  0001 C CNN "part"
F 5 "9332383" V 6175 6875 60  0001 C CNN "farnell"
	1    6175 6875
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 5790C725
P 6400 6875
F 0 "R?" V 6480 6875 50  0000 C CNN
F 1 "1k" V 6400 6875 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6330 6875 50  0001 C CNN
F 3 "" H 6400 6875 50  0000 C CNN
F 4 "MC01W080511K" V 6400 6875 60  0001 C CNN "part"
F 5 "9332383" V 6400 6875 60  0001 C CNN "farnell"
	1    6400 6875
	-1   0    0    1   
$EndComp
$Comp
L MAX232 U?
U 1 1 5790C72B
P 5375 5650
F 0 "U?" H 4925 6500 50  0000 L CNN
F 1 "MAX232" H 5575 6500 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-16_4.4x5mm_Pitch0.65mm" H 5375 5650 50  0001 C CNN
F 3 "" H 5375 5650 50  0000 C CNN
F 4 "MAX202IPW" H 5375 5650 60  0001 C CNN "part"
F 5 "2335478" H 5375 5650 60  0001 C CNN "farnell"
	1    5375 5650
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5790C731
P 6275 5100
F 0 "C?" H 6300 5200 50  0000 L CNN
F 1 "1u" H 6300 5000 50  0000 L CNN
F 2 "" H 6313 4950 50  0000 C CNN
F 3 "" H 6275 5100 50  0000 C CNN
	1    6275 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5790C737
P 6125 5550
F 0 "#PWR?" H 6125 5300 50  0001 C CNN
F 1 "GND" H 6125 5400 50  0001 C CNN
F 2 "" H 6125 5550 50  0000 C CNN
F 3 "" H 6125 5550 50  0000 C CNN
	1    6125 5550
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5790C73D
P 6125 5350
F 0 "C?" H 6150 5450 50  0000 L CNN
F 1 "1u" H 6150 5250 50  0000 L CNN
F 2 "" H 6163 5200 50  0000 C CNN
F 3 "" H 6125 5350 50  0000 C CNN
	1    6125 5350
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 5790C743
P 6125 5850
F 0 "C?" H 6150 5950 50  0000 L CNN
F 1 "1u" H 6150 5750 50  0000 L CNN
F 2 "" H 6163 5700 50  0000 C CNN
F 3 "" H 6125 5850 50  0000 C CNN
	1    6125 5850
	0    1    1    0   
$EndComp
Wire Wire Line
	6275 5550 5975 5550
Wire Wire Line
	6275 5250 6275 5850
Connection ~ 6125 5550
Connection ~ 6275 5550
Wire Wire Line
	6275 4950 5975 4950
Connection ~ 6275 5350
$Comp
L C C?
U 1 1 5790C74F
P 4625 5150
F 0 "C?" H 4650 5250 50  0000 L CNN
F 1 "1u" H 4650 5050 50  0000 L CNN
F 2 "" H 4663 5000 50  0000 C CNN
F 3 "" H 4625 5150 50  0000 C CNN
	1    4625 5150
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5790C755
P 4625 5650
F 0 "C?" H 4650 5750 50  0000 L CNN
F 1 "1u" H 4650 5550 50  0000 L CNN
F 2 "" H 4663 5500 50  0000 C CNN
F 3 "" H 4625 5650 50  0000 C CNN
	1    4625 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4775 4950 4625 4950
Wire Wire Line
	4625 4950 4625 5000
Wire Wire Line
	4775 5350 4625 5350
Wire Wire Line
	4625 5350 4625 5300
Wire Wire Line
	4775 5450 4625 5450
Wire Wire Line
	4625 5450 4625 5500
Wire Wire Line
	4775 5850 4625 5850
Wire Wire Line
	4625 5850 4625 5800
Text GLabel 6275 4950 1    60   Input ~ 0
+5V
$Comp
L C C?
U 1 1 5790C764
P 6175 6300
F 0 "C?" H 6200 6400 50  0000 L CNN
F 1 "1u" H 6200 6200 50  0000 L CNN
F 2 "" H 6213 6150 50  0000 C CNN
F 3 "" H 6175 6300 50  0000 C CNN
	1    6175 6300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5790C76A
P 6400 6300
F 0 "C?" H 6425 6400 50  0000 L CNN
F 1 "1u" H 6425 6200 50  0000 L CNN
F 2 "" H 6438 6150 50  0000 C CNN
F 3 "" H 6400 6300 50  0000 C CNN
	1    6400 6300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5790C770
P 6175 7025
F 0 "#PWR?" H 6175 6775 50  0001 C CNN
F 1 "GND" H 6175 6875 50  0000 C CNN
F 2 "" H 6175 7025 50  0000 C CNN
F 3 "" H 6175 7025 50  0000 C CNN
	1    6175 7025
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5790C776
P 5975 6350
F 0 "#PWR?" H 5975 6100 50  0001 C CNN
F 1 "GND" H 5975 6200 50  0001 C CNN
F 2 "" H 5975 6350 50  0000 C CNN
F 3 "" H 5975 6350 50  0000 C CNN
	1    5975 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5975 6350 5975 6250
Wire Wire Line
	5975 6150 6175 6150
Wire Wire Line
	5975 6050 6400 6050
Wire Wire Line
	6400 6050 6400 6150
Wire Wire Line
	6400 7025 6175 7025
Wire Wire Line
	6400 6450 6400 6725
NoConn ~ 4775 6350
NoConn ~ 4775 6250
Wire Wire Line
	6175 6575 5900 6575
Wire Wire Line
	6175 6450 6175 6725
Connection ~ 6175 6575
Wire Wire Line
	6400 6725 5900 6725
Wire Notes Line
	675  4225 6600 4225
Wire Notes Line
	6600 4225 6600 7350
Wire Notes Line
	6600 7350 675  7350
Wire Notes Line
	675  7350 675  4225
Text Notes 825  4450 0    60   ~ 0
output drivers, interfaces with car ABS rear wheel sensor circuit\noutputs a AC squarewave (+ - 16V pk to pk)
Wire Notes Line
	10875 3800 5425 3800
Wire Notes Line
	5425 3800 5425 625 
Wire Notes Line
	5425 625  10875 625 
Wire Notes Line
	10875 625  10875 3800
Text Notes 5500 825  0    60   ~ 0
Converts VR sensor analog to digital
Wire Notes Line
	8150 4250 10850 4250
Wire Notes Line
	10850 4250 10850 5875
Wire Notes Line
	10850 5875 8150 5875
Wire Notes Line
	8150 5875 8150 4250
Text Notes 8200 4450 0    60   ~ 0
switch to diag mode (check wheelsensors for continuity)\nif output toggles sensor is broken
Text GLabel 4775 6150 0    60   Input ~ 0
R_speed_out+
Text GLabel 1725 6050 0    60   Input ~ 0
L_speed_out-
Text GLabel 4775 6050 0    60   Input ~ 0
R_speed_out-
$EndSCHEMATC
