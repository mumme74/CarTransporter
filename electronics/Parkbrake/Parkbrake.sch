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
Sheet 1 10
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
L ZENER D?
U 1 1 5708DE01
P 2625 6700
F 0 "D?" V 2725 6525 50  0000 L CNN
F 1 "10v" V 2650 6500 50  0000 L CNN
F 2 "" H 2625 6700 50  0000 C CNN
F 3 "" H 2625 6700 50  0000 C CNN
	1    2625 6700
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 5708DE58
P 2325 7050
F 0 "R?" H 2395 7096 50  0000 L CNN
F 1 "10k" H 2395 7005 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 2255 7050 50  0001 C CNN
F 3 "" H 2325 7050 50  0000 C CNN
F 4 "9332391" H 2325 7050 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" H 2325 7050 60  0001 C CNN "part"
	1    2325 7050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5708DE8A
P 2325 7200
F 0 "#PWR?" H 2325 6950 50  0001 C CNN
F 1 "GND" H 2333 7027 50  0000 C CNN
F 2 "" H 2325 7200 50  0000 C CNN
F 3 "" H 2325 7200 50  0000 C CNN
	1    2325 7200
	1    0    0    -1  
$EndComp
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
F 2 "Resistors_SMD:R_0805" V 5030 6625 50  0001 C CNN
F 3 "" H 5100 6625 50  0000 C CNN
F 4 "MC01W080511K" H 5100 6625 60  0001 C CNN "part"
F 5 "9332383" H 5100 6625 60  0001 C CNN "farnell"
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
F 2 "Resistors_SMD:R_0805" V 4680 6225 50  0001 C CNN
F 3 "" H 4750 6225 50  0000 C CNN
F 4 "9332391" H 4750 6225 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" H 4750 6225 60  0001 C CNN "part"
	1    4750 6225
	1    0    0    -1  
$EndComp
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
Text GLabel 5475 6975 2    60   Input ~ 0
Enable_Bridge_PWR
Text GLabel 3000 6500 2    60   Input ~ 0
+12V_Feed
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
Text Notes 4025 5375 0    60   ~ 0
Batt switch off and polarity protection
Text Notes 1075 5350 0    60   ~ 0
Polarity protection ignition and force activate
$Comp
L VR VR?
U 1 1 5701FFFF
P 2950 6750
F 0 "VR?" H 3038 6796 50  0000 L CNN
F 1 "VR" H 3038 6705 50  0000 L CNN
F 2 "" H 2950 6750 50  0000 C CNN
F 3 "" H 2950 6750 50  0000 C CNN
F 4 "VC120618A400DP" H 2950 6750 60  0001 C CNN "part"
F 5 "1301938" H 2950 6750 60  0001 C CNN "farnell"
	1    2950 6750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 570200C5
P 2950 7000
F 0 "#PWR?" H 2950 6750 50  0001 C CNN
F 1 "GND" H 2958 6827 50  0000 C CNN
F 2 "" H 2950 7000 50  0000 C CNN
F 3 "" H 2950 7000 50  0000 C CNN
	1    2950 7000
	1    0    0    -1  
$EndComp
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
Text Label 1575 1325 2    60   ~ 0
+12V_Bat
Text GLabel 2675 1625 2    60   Input ~ 0
LeftFrontMotor+
Text GLabel 2675 1775 2    60   Input ~ 0
LeftFrontMotor-
Text GLabel 1625 1925 0    60   Input ~ 0
RightFrontMotor+
Text GLabel 1625 1625 0    60   Input ~ 0
RightFrontMotor-
Text GLabel 2675 1925 2    60   Input ~ 0
LeftRearMotor+
Text GLabel 2675 2075 2    60   Input ~ 0
LeftRearMotor-
Text GLabel 2675 2225 2    60   Input ~ 0
RightRearMotor+
Text GLabel 2675 2375 2    60   Input ~ 0
RightRearMotor-
Text GLabel 1625 2225 0    60   Input ~ 0
CAN_HI
Text GLabel 1625 2375 0    60   Input ~ 0
CAN_LO
$Sheet
S 6625 950  1250 1375
U 570A3FC3
F0 "microcontroller" 60
F1 "microcontroller.sch" 60
$EndSheet
$Comp
L GND #PWR?
U 1 1 578E7EF2
P 2650 1175
F 0 "#PWR?" H 2650 925 50  0001 C CNN
F 1 "GND" H 2650 1025 50  0000 C CNN
F 2 "" H 2650 1175 50  0000 C CNN
F 3 "" H 2650 1175 50  0000 C CNN
	1    2650 1175
	1    0    0    -1  
$EndComp
Text GLabel 1650 3025 0    60   Input ~ 0
LF_wheelsensor_in+
Text GLabel 1650 2875 0    60   Input ~ 0
LF_wheelsensor_in-
Text GLabel 1650 3175 0    60   Input ~ 0
LR_wheelsensor_in-
Text GLabel 1650 3325 0    60   Input ~ 0
LR_wheelsensor_in+
Text GLabel 2675 2850 2    60   Input ~ 0
RF_wheelsensor_in-
Text GLabel 2675 3000 2    60   Input ~ 0
RF_wheelsensor_in+
Text GLabel 2675 3175 2    60   Input ~ 0
RR_wheelsensor_in-
Text GLabel 2675 3300 2    60   Input ~ 0
RR_wheelsensor_in+
Text GLabel 2050 4075 2    60   Input ~ 0
Parkbrake_indicator_lamp
Text GLabel 1650 3775 0    60   Input ~ 0
BrakeLight
Text GLabel 2675 3775 2    60   Input ~ 0
ParkbrakeSwitch_non_invert
Text GLabel 2675 3925 2    60   Input ~ 0
~ParkbrakeSwitch_invert
Text GLabel 2675 3450 2    60   Input ~ 0
R_wheelsensor_out-
Text GLabel 2675 3600 2    60   Input ~ 0
R_wheelsensor_out+
Text GLabel 1650 3475 0    60   Input ~ 0
L_wheelsensor_out-
Text GLabel 1650 3625 0    60   Input ~ 0
L_wheelsensor_out+
Wire Wire Line
	2325 6900 2750 6900
Wire Wire Line
	2325 6900 2325 6800
Wire Wire Line
	2525 6500 3000 6500
Connection ~ 2625 6500
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
	3850 7625 600  7625
Wire Notes Line
	600  7625 600  5250
Wire Notes Line
	600  5250 3625 5250
Connection ~ 2950 6500
Connection ~ 2050 1025
Wire Wire Line
	2050 1025 2050 1175
Connection ~ 1800 1025
Wire Wire Line
	1575 1025 2300 1025
Connection ~ 2300 1625
Connection ~ 2300 2225
Wire Wire Line
	1800 1025 1800 1175
Connection ~ 2050 1325
Wire Wire Line
	1800 1325 1800 1475
Wire Wire Line
	1575 1325 2300 1325
Wire Wire Line
	2050 1325 2050 1475
Connection ~ 1800 1325
Wire Wire Line
	2300 1175 2650 1175
Wire Wire Line
	2300 1475 2650 1475
Wire Wire Line
	2050 1625 2675 1625
Wire Wire Line
	1800 1625 1625 1625
Wire Wire Line
	1800 1625 1800 1775
Wire Wire Line
	2050 1775 2675 1775
Connection ~ 2300 1775
Wire Wire Line
	1800 2075 1800 1925
Wire Wire Line
	1800 1925 1625 1925
Wire Wire Line
	2050 1925 2675 1925
Connection ~ 2300 1925
Wire Wire Line
	2050 2075 2675 2075
Connection ~ 2300 2075
Wire Wire Line
	2050 2225 2675 2225
Wire Wire Line
	2050 2375 2675 2375
Connection ~ 2300 2375
Wire Wire Line
	1800 2225 1625 2225
Wire Wire Line
	1800 2375 1625 2375
Wire Wire Line
	2675 2850 2300 2850
Wire Wire Line
	2300 2850 2300 3025
Wire Wire Line
	2675 3000 2500 3000
Wire Wire Line
	2225 2675 2050 2675
Wire Wire Line
	2050 2675 2050 3025
Wire Wire Line
	1650 2875 1800 2875
Wire Wire Line
	1800 2875 1800 3025
Wire Wire Line
	1800 3175 1725 3175
Wire Wire Line
	1725 3175 1725 3025
Wire Wire Line
	1725 3025 1650 3025
Wire Wire Line
	1800 3325 1700 3325
Wire Wire Line
	1700 3325 1700 3175
Wire Wire Line
	1700 3175 1650 3175
Wire Wire Line
	1800 3400 1800 3475
Wire Wire Line
	1800 3400 1650 3400
Wire Wire Line
	2050 3175 2050 3225
Wire Wire Line
	2050 3225 2600 3225
Wire Wire Line
	2600 3225 2600 3175
Wire Wire Line
	2600 3175 2675 3175
Wire Wire Line
	2300 3175 2300 3125
Wire Wire Line
	2300 3125 2500 3125
Wire Wire Line
	2500 3125 2500 3000
Wire Wire Line
	2600 3300 2600 3275
Wire Wire Line
	2600 3275 2050 3275
Wire Wire Line
	2050 3275 2050 3325
Wire Wire Line
	2600 3450 2600 3375
Wire Wire Line
	2600 3375 2300 3375
Wire Wire Line
	2300 3375 2300 3325
Wire Wire Line
	2675 3600 2600 3600
Wire Wire Line
	2600 3600 2600 3525
Wire Wire Line
	2600 3525 2300 3525
Wire Wire Line
	2300 3525 2300 3475
Wire Wire Line
	2050 3475 2050 3525
Wire Wire Line
	2050 3525 1700 3525
Wire Wire Line
	2600 3450 2675 3450
Wire Wire Line
	2600 3300 2675 3300
Wire Wire Line
	1650 3400 1650 3325
Wire Wire Line
	1700 3525 1700 3475
Wire Wire Line
	1700 3475 1650 3475
Wire Wire Line
	2050 3625 2050 3575
Wire Wire Line
	2050 3575 1700 3575
Wire Wire Line
	1700 3575 1700 3625
Wire Wire Line
	1700 3625 1650 3625
Wire Wire Line
	1800 3625 1725 3625
Wire Wire Line
	1725 3625 1725 3775
Wire Wire Line
	1725 3775 1650 3775
Wire Wire Line
	2675 3775 2600 3775
Wire Wire Line
	2600 3775 2600 3675
Wire Wire Line
	2600 3675 2300 3675
Wire Wire Line
	2300 3675 2300 3625
Wire Wire Line
	2675 3925 2300 3925
Wire Wire Line
	2300 3925 2300 3775
Wire Wire Line
	2050 3775 2050 4075
NoConn ~ 1800 3775
$Comp
L PWR_FLAG #FLG?
U 1 1 578EA974
P 4450 6075
F 0 "#FLG?" H 4450 6170 50  0001 C CNN
F 1 "PWR_FLAG" H 4450 6255 50  0000 C CNN
F 2 "" H 4450 6075 50  0000 C CNN
F 3 "" H 4450 6075 50  0000 C CNN
	1    4450 6075
	1    0    0    -1  
$EndComp
Wire Wire Line
	2125 6500 2000 6500
$Comp
L PWR_FLAG #FLG?
U 1 1 578EAB0E
P 2000 6500
F 0 "#FLG?" H 2000 6595 50  0001 C CNN
F 1 "PWR_FLAG" H 2000 6680 50  0001 C CNN
F 2 "" H 2000 6500 50  0000 C CNN
F 3 "" H 2000 6500 50  0000 C CNN
	1    2000 6500
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 578EAB3C
P 2950 7000
F 0 "#FLG?" H 2950 7095 50  0001 C CNN
F 1 "PWR_FLAG" H 2950 7200 50  0000 C CNN
F 2 "" H 2950 7000 50  0000 C CNN
F 3 "" H 2950 7000 50  0000 C CNN
	1    2950 7000
	0    1    1    0   
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 578EAC6F
P 5550 6575
F 0 "#FLG?" H 5550 6670 50  0001 C CNN
F 1 "PWR_FLAG" H 5575 6775 50  0000 C CNN
F 2 "" H 5550 6575 50  0000 C CNN
F 3 "" H 5550 6575 50  0000 C CNN
	1    5550 6575
	0    1    1    0   
$EndComp
$Sheet
S 6625 2650 1425 1550
U 578EB5AC
F0 "Buttonlogic" 60
F1 "buttonlogic.sch" 60
$EndSheet
$Comp
L Q_PMOS_GSD Q?
U 1 1 578EF90C
P 2325 6600
F 0 "Q?" V 2225 6800 50  0000 R CNN
F 1 "Q_PMOS_GSD" V 2550 6800 50  0000 R CNN
F 2 "" H 2525 6700 50  0000 C CNN
F 3 "" H 2325 6600 50  0000 C CNN
	1    2325 6600
	0    -1   -1   0   
$EndComp
Text GLabel 2650 1475 2    60   Input ~ 0
+12V_ign_in
Text GLabel 2000 6500 0    60   Input ~ 0
+12V_ign_in
$Comp
L Q_PMOS_GSD Q?
U 1 1 578EFF37
P 2325 5975
F 0 "Q?" V 2200 6175 50  0000 R CNN
F 1 "Q_PMOS_GSD" V 2550 6200 50  0000 R CNN
F 2 "" H 2525 6075 50  0000 C CNN
F 3 "" H 2325 5975 50  0000 C CNN
	1    2325 5975
	0    -1   1    0   
$EndComp
Wire Wire Line
	2625 6500 2625 6075
Wire Wire Line
	2625 6075 2525 6075
Wire Wire Line
	2125 6075 2000 6075
Wire Wire Line
	2325 5775 2750 5775
Wire Wire Line
	2750 5775 2750 6900
Connection ~ 2625 6900
Text GLabel 2000 6075 0    60   Input ~ 0
ParkbrakeSwitch_non_invert
$Comp
L PWR_FLAG #FLG?
U 1 1 578F074A
P 2000 6075
F 0 "#FLG?" H 2000 6170 50  0001 C CNN
F 1 "PWR_FLAG" H 2000 6255 50  0001 C CNN
F 2 "" H 2000 6075 50  0000 C CNN
F 3 "" H 2000 6075 50  0000 C CNN
	1    2000 6075
	1    0    0    -1  
$EndComp
Text Label 2225 2675 0    60   ~ 0
wheelsensor_shielding
Text Label 4625 875  2    60   ~ 0
wheelsensor_shielding
$Comp
L R R?
U 1 1 578FDBA7
P 4625 1025
F 0 "R?" V 4705 1025 50  0000 C CNN
F 1 "1k" V 4625 1025 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4555 1025 50  0001 C CNN
F 3 "" H 4625 1025 50  0000 C CNN
F 4 "9332383" V 4625 1025 60  0001 C CNN "part"
F 5 "MC01W080511K" V 4625 1025 60  0001 C CNN "farnell"
	1    4625 1025
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 578FDCF1
P 4625 1175
F 0 "#PWR?" H 4625 975 50  0001 C CNN
F 1 "GNDPWR" H 4625 1045 50  0000 C CNN
F 2 "" H 4625 1125 50  0000 C CNN
F 3 "" H 4625 1125 50  0000 C CNN
	1    4625 1175
	1    0    0    -1  
$EndComp
$Comp
L TEST_1P W?
U 1 1 578FE195
P 5475 7200
F 0 "W?" V 5475 7450 50  0000 C CNN
F 1 "TP_enable_bridge_pwr" V 5575 7725 50  0000 C CNN
F 2 "" H 5675 7200 50  0000 C CNN
F 3 "" H 5675 7200 50  0000 C CNN
	1    5475 7200
	0    1    1    0   
$EndComp
Wire Wire Line
	5475 6975 5475 7200
Wire Wire Line
	5475 6975 5400 6975
$Sheet
S 8925 4825 1350 1050
U 578FFC52
F0 "Wheelsensors" 60
F1 "wheelsensors.sch" 60
$EndSheet
$EndSCHEMATC
