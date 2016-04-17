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
LIBS:Parkbrake-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
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
L Conn48pin P1
U 1 1 56FEAA1E
P 2150 2225
F 0 "P1" H 2075 3850 60  0000 L CNN
F 1 "Conn48pin" H 1975 3750 60  0000 L CNN
F 2 "" H 2300 2525 60  0000 C CNN
F 3 "" H 2300 2525 60  0000 C CNN
	1    2150 2225
	1    0    0    -1  
$EndComp
$Sheet
S 8875 2600 1525 1450
U 570483C1
F0 "H-Bridge_logic" 60
F1 "H-Bridge_logic.sch" 60
$EndSheet
$Sheet
S 8875 1000 1525 850 
U 57088F10
F0 "PowerRegulators" 60
F1 "PowerRegulators.sch" 60
$EndSheet
$Comp
L Q_PMOS_GDS Q?
U 1 1 5708DD42
P 2100 6200
F 0 "Q?" V 2431 6200 50  0000 C CNN
F 1 "P-ch -40V 0.0035ohm" V 2340 6200 50  0000 C CNN
F 2 "" H 2300 6300 50  0000 C CNN
F 3 "" H 2100 6200 50  0000 C CNN
F 4 "2480840" V 2100 6200 60  0001 C CNN "farnell"
F 5 "2480840" V 2100 6200 60  0001 C CNN "part"
	1    2100 6200
	0    -1   -1   0   
$EndComp
$Comp
L ZENER D?
U 1 1 5708DE01
P 2400 6300
F 0 "D?" V 2354 6379 50  0000 L CNN
F 1 "10v" V 2445 6379 50  0000 L CNN
F 2 "" H 2400 6300 50  0000 C CNN
F 3 "" H 2400 6300 50  0000 C CNN
	1    2400 6300
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 5708DE58
P 2100 6650
F 0 "R?" H 2170 6696 50  0000 L CNN
F 1 "10k" H 2170 6605 50  0000 L CNN
F 2 "" V 2030 6650 50  0000 C CNN
F 3 "" H 2100 6650 50  0000 C CNN
	1    2100 6650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5708DE8A
P 2100 6800
F 0 "#PWR?" H 2100 6550 50  0001 C CNN
F 1 "GND" H 2108 6627 50  0000 C CNN
F 2 "" H 2100 6800 50  0000 C CNN
F 3 "" H 2100 6800 50  0000 C CNN
	1    2100 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 6500 2100 6500
Wire Wire Line
	2100 6500 2100 6400
Wire Wire Line
	2300 6100 2775 6100
Connection ~ 2400 6100
Text Label 1900 6100 2    60   ~ 0
+12V_IGN
$Comp
L Q_PMOS_GDS Q?
U 1 1 5709079E
P 5100 6175
F 0 "Q?" V 5300 6175 50  0000 C CNN
F 1 "P-ch -40V 0.0035ohm" V 5300 5625 50  0000 C CNN
F 2 "" H 5300 6275 50  0000 C CNN
F 3 "" H 5100 6175 50  0000 C CNN
F 4 "PD90P04P4-05" V 5100 6175 60  0001 C CNN "part"
F 5 "2480840" V 5100 6175 60  0001 C CNN "farnell"
	1    5100 6175
	0    -1   -1   0   
$EndComp
$Comp
L ZENER D?
U 1 1 5709088A
P 4450 6275
F 0 "D?" V 4404 6354 50  0000 L CNN
F 1 "10v" V 4495 6354 50  0000 L CNN
F 2 "" H 4450 6275 50  0000 C CNN
F 3 "" H 4450 6275 50  0000 C CNN
	1    4450 6275
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 5709092F
P 5100 6625
F 0 "R?" H 5170 6671 50  0000 L CNN
F 1 "1k" H 5170 6580 50  0000 L CNN
F 2 "" V 5030 6625 50  0000 C CNN
F 3 "" H 5100 6625 50  0000 C CNN
	1    5100 6625
	1    0    0    -1  
$EndComp
$Comp
L Q_NMOS_GSD Q?
U 1 1 5709097D
P 5200 6975
F 0 "Q?" H 5394 7021 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 5394 6930 50  0000 L CNN
F 2 "" H 5400 7075 50  0000 C CNN
F 3 "" H 5200 6975 50  0000 C CNN
	1    5200 6975
	-1   0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57090A02
P 5100 7475
F 0 "#PWR?" H 5100 7275 50  0001 C CNN
F 1 "GNDPWR" H 5107 7549 50  0001 C CNN
F 2 "" H 5100 7425 50  0000 C CNN
F 3 "" H 5100 7425 50  0000 C CNN
	1    5100 7475
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57090848
P 4750 6225
F 0 "R?" H 4820 6271 50  0000 L CNN
F 1 "10k" H 4820 6180 50  0000 L CNN
F 2 "" V 4680 6225 50  0000 C CNN
F 3 "" H 4750 6225 50  0000 C CNN
	1    4750 6225
	1    0    0    -1  
$EndComp
Wire Wire Line
	4325 6075 4900 6075
Connection ~ 4750 6075
Wire Wire Line
	5100 6475 5100 6375
Connection ~ 4450 6075
Wire Wire Line
	4450 6475 5100 6475
Wire Wire Line
	4750 6375 4750 6475
Connection ~ 4750 6475
$Comp
L D D?
U 1 1 570913D2
P 5100 7325
F 0 "D?" V 5146 7247 50  0000 R CNN
F 1 "1n4148w" V 5055 7247 50  0000 R CNN
F 2 "" H 5100 7325 50  0000 C CNN
F 3 "" H 5100 7325 50  0000 C CNN
	1    5100 7325
	0    -1   -1   0   
$EndComp
Text Label 4325 6075 2    60   ~ 0
+12V_bat
Text GLabel 5925 6075 2    60   Input ~ 0
+12V_power
Text GLabel 5400 6975 2    60   Input ~ 0
Enable_12V_power
Text GLabel 2775 6100 2    60   Input ~ 0
+12V_IGN_Safe
$Comp
L Q_PMOS_GDS Q?
U 1 1 57091D35
P 5100 5625
F 0 "Q?" V 5300 5625 50  0000 C CNN
F 1 "P-ch -40V 0.0035ohm" V 5150 5025 50  0000 C CNN
F 2 "" H 5300 5725 50  0000 C CNN
F 3 "" H 5100 5625 50  0000 C CNN
F 4 "PD90P04P4-05" V 5100 5625 60  0001 C CNN "part"
F 5 "2480840" V 5100 5625 60  0001 C CNN "farnell"
	1    5100 5625
	0    -1   1    0   
$EndComp
Wire Wire Line
	4900 5725 4750 5725
Wire Wire Line
	4750 5725 4750 6075
Wire Wire Line
	5300 5725 5300 6075
Wire Wire Line
	5100 5425 4875 5425
Wire Wire Line
	4875 5425 4875 6475
Connection ~ 4875 6475
$Comp
L VR VR?
U 1 1 5701F12B
P 5550 6325
F 0 "VR?" H 5638 6371 50  0000 L CNN
F 1 "VR" H 5638 6280 50  0000 L CNN
F 2 "" H 5550 6325 50  0000 C CNN
F 3 "" H 5550 6325 50  0000 C CNN
F 4 "VC120618A400DP" H 5550 6325 60  0001 C CNN "part"
F 5 "1301938" H 5550 6325 60  0001 C CNN "farnell"
	1    5550 6325
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5701F17D
P 5550 6575
F 0 "#PWR?" H 5550 6375 50  0001 C CNN
F 1 "GNDPWR" H 5557 6649 50  0001 C CNN
F 2 "" H 5550 6525 50  0000 C CNN
F 3 "" H 5550 6525 50  0000 C CNN
	1    5550 6575
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 6075 5925 6075
Connection ~ 5550 6075
Wire Notes Line
	3875 5250 6675 5250
Wire Notes Line
	6675 5250 6675 7625
Wire Notes Line
	6675 7625 3875 7625
Wire Notes Line
	3875 7625 3875 5250
Wire Notes Line
	3625 5250 3625 7625
Wire Notes Line
	3625 7625 1275 7625
Wire Notes Line
	1275 7625 1275 5250
Wire Notes Line
	1275 5250 3625 5250
Text Notes 4025 5375 0    60   ~ 0
Batt switch off and polarity protection
Text Notes 1525 5350 0    60   ~ 0
Polarity protection ignition
$Comp
L VR VR?
U 1 1 5701FFFF
P 2725 6350
F 0 "VR?" H 2813 6396 50  0000 L CNN
F 1 "VR" H 2813 6305 50  0000 L CNN
F 2 "" H 2725 6350 50  0000 C CNN
F 3 "" H 2725 6350 50  0000 C CNN
F 4 "VC120618A400DP" H 2725 6350 60  0001 C CNN "part"
F 5 "1301938" H 2725 6350 60  0001 C CNN "farnell"
	1    2725 6350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 570200C5
P 2725 6600
F 0 "#PWR?" H 2725 6350 50  0001 C CNN
F 1 "GND" H 2733 6427 50  0000 C CNN
F 2 "" H 2725 6600 50  0000 C CNN
F 3 "" H 2725 6600 50  0000 C CNN
	1    2725 6600
	1    0    0    -1  
$EndComp
Connection ~ 2725 6100
Connection ~ 2050 1025
Wire Wire Line
	1800 1025 1800 1325
Wire Wire Line
	2050 1025 2050 1175
Connection ~ 1800 1175
Connection ~ 1800 1025
Wire Wire Line
	1575 1025 2300 1025
$Comp
L GNDPWR #PWR?
U 1 1 570235BF
P 1575 1025
F 0 "#PWR?" H 1575 825 50  0001 C CNN
F 1 "GNDPWR" H 1582 1099 50  0000 C CNN
F 2 "" H 1575 975 50  0000 C CNN
F 3 "" H 1575 975 50  0000 C CNN
	1    1575 1025
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1475 1800 1475
Wire Wire Line
	2050 1475 2050 1325
Wire Wire Line
	2300 1175 2300 1475
Connection ~ 2050 1475
Connection ~ 2300 1325
Wire Wire Line
	2300 1175 2650 1175
Text Label 2650 1175 0    60   ~ 0
+12V_Bat
Wire Wire Line
	1800 1625 2675 1625
Connection ~ 2050 1625
Connection ~ 2300 1625
Wire Wire Line
	1625 1775 2300 1775
Connection ~ 2050 1775
Connection ~ 1800 1775
Wire Wire Line
	1800 1925 2675 1925
Connection ~ 2050 1925
Connection ~ 2300 1925
Wire Wire Line
	1625 2075 2300 2075
Connection ~ 2050 2075
Connection ~ 1800 2075
Wire Wire Line
	1800 2225 2675 2225
Connection ~ 2050 2225
Connection ~ 2300 2225
Wire Wire Line
	1625 2375 2300 2375
Connection ~ 2050 2375
Connection ~ 1800 2375
Wire Wire Line
	1800 3025 2675 3025
Connection ~ 2050 3025
Connection ~ 2300 3025
Wire Wire Line
	1625 3175 2300 3175
Connection ~ 2050 3175
Connection ~ 1800 3175
Text GLabel 2675 1625 2    60   Input ~ 0
LeftFrontMotor+
Text GLabel 1625 1775 0    60   Input ~ 0
LeftFrontMotor-
Text GLabel 2675 1925 2    60   Input ~ 0
RightFrontMotor+
Text GLabel 1625 2075 0    60   Input ~ 0
RightFrontMotor-
Text GLabel 2675 2225 2    60   Input ~ 0
LeftRearMotor+
Text GLabel 1625 2375 0    60   Input ~ 0
LeftRearMotor-
Text GLabel 2675 3025 2    60   Input ~ 0
RightRearMotor+
Text GLabel 1625 3175 0    60   Input ~ 0
RightRearMotor-
Wire Wire Line
	2300 3325 2675 3325
Text Label 2675 3325 0    60   ~ 0
+12V_IGN
Wire Wire Line
	2050 3325 2050 3250
Wire Wire Line
	2050 3250 2625 3250
Wire Wire Line
	2625 3250 2625 3150
Wire Wire Line
	2625 3150 3225 3150
$Comp
L GND #PWR?
U 1 1 570245AD
P 3225 3150
F 0 "#PWR?" H 3225 2900 50  0001 C CNN
F 1 "GND" H 3233 2977 50  0001 C CNN
F 2 "" H 3225 3150 50  0000 C CNN
F 3 "" H 3225 3150 50  0000 C CNN
	1    3225 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 3325 1625 3325
Wire Wire Line
	1800 3475 1625 3475
Text GLabel 1625 3325 0    60   Input ~ 0
CAN_HI
Text GLabel 1625 3475 0    60   Input ~ 0
CAN_LO
$Sheet
S 5675 2450 1250 1375
U 570A3FC3
F0 "microcontroller" 60
F1 "microcontroller.sch" 60
$EndSheet
$EndSCHEMATC
