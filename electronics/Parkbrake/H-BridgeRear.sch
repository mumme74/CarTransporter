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
Sheet 3 6
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
L Q_NMOS_GDS Q?
U 1 1 570104A2
P 2750 4175
F 0 "Q?" H 2944 4221 50  0000 L CNN
F 1 "IPD031N06L3 G" H 2225 4025 50  0000 L CNN
F 2 "" H 2950 4275 50  0000 C CNN
F 3 "" H 2750 4175 50  0000 C CNN
F 4 "IPD031N06L3 G" H 2750 4175 60  0001 C CNN "part"
F 5 "2480814" H 2750 4175 60  0001 C CNN "farnell"
	1    2750 4175
	1    0    0    -1  
$EndComp
$Comp
L Q_NMOS_GDS Q?
U 1 1 57010556
P 3550 4175
F 0 "Q?" H 3744 4129 50  0000 L CNN
F 1 "IPD031N06L3 G" H 3025 4375 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TO-252-2Lead" H 3744 4266 50  0001 L CNN
F 3 "" H 3550 4175 50  0000 C CNN
F 4 "IPD031N06L3 G" H 3550 4175 60  0001 C CNN "part"
F 5 "2480814" H 3550 4175 60  0001 C CNN "farnell"
	1    3550 4175
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GDS Q?
U 1 1 57012689
P 3550 3650
F 0 "Q?" H 3744 3604 50  0000 L CNN
F 1 "IPD031N06L3 G" H 3175 3450 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TO-252-2Lead" H 3744 3741 50  0001 L CNN
F 3 "" H 3550 3650 50  0000 C CNN
F 4 "IPD031N06L3 G" H 3550 3650 60  0001 C CNN "part"
F 5 "2480814" H 3550 3650 60  0001 C CNN "farnell"
	1    3550 3650
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GDS Q?
U 1 1 570126D2
P 2750 3650
F 0 "Q?" H 2944 3696 50  0000 L CNN
F 1 "IPD031N06L3 G" H 2225 3800 50  0000 L CNN
F 2 "" H 2950 3750 50  0000 C CNN
F 3 "" H 2750 3650 50  0000 C CNN
F 4 "IPD031N06L3 G" H 2750 3650 60  0001 C CNN "part"
F 5 "2480814" H 2750 3650 60  0001 C CNN "farnell"
	1    2750 3650
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57012736
P 3175 4850
F 0 "#PWR?" H 3175 4650 50  0001 C CNN
F 1 "GNDPWR" H 3182 4924 50  0001 C CNN
F 2 "" H 3175 4800 50  0000 C CNN
F 3 "" H 3175 4800 50  0000 C CNN
	1    3175 4850
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57012752
P 2850 4700
F 0 "R?" H 2900 4700 50  0000 L CNN
F 1 "0.001" H 2900 4600 50  0000 L CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 2920 4609 50  0001 L CNN
F 3 "" H 2850 4700 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 2850 4700 60  0001 C CNN "part"
F 5 "2079420" H 2850 4700 60  0001 C CNN "farnell"
	1    2850 4700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570128A4
P 3450 4700
F 0 "R?" H 3520 4746 50  0000 L CNN
F 1 "0.001" H 3520 4655 50  0000 L CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 3520 4609 50  0001 L CNN
F 3 "" H 3450 4700 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 3450 4700 60  0001 C CNN "part"
F 5 "2079420" H 3450 4700 60  0001 C CNN "farnell"
	1    3450 4700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57012A17
P 3000 3000
F 0 "R?" V 2793 3000 50  0000 C CNN
F 1 "0.001" V 2884 3000 50  0000 C CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 3070 2909 50  0001 L CNN
F 3 "" H 3000 3000 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 3000 3000 60  0001 C CNN "part"
F 5 "2079420" H 3000 3000 60  0001 C CNN "farnell"
	1    3000 3000
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57012A1F
P 3000 3250
F 0 "R?" V 2825 3275 50  0000 R CNN
F 1 "0.001" V 2900 3325 50  0000 R CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 3070 3159 50  0001 L CNN
F 3 "" H 3000 3250 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 3000 3250 60  0001 C CNN "part"
F 5 "2079420" H 3000 3250 60  0001 C CNN "farnell"
	1    3000 3250
	0    1    1    0   
$EndComp
Text GLabel 2500 2475 0    60   Input ~ 0
+12V_power
$Comp
L C C?
U 1 1 57012D23
P 3150 3850
F 0 "C?" V 2898 3850 50  0000 C CNN
F 1 "220nF" V 2989 3850 50  0000 C CNN
F 2 "" H 3188 3700 50  0000 C CNN
F 3 "" H 3150 3850 50  0000 C CNN
	1    3150 3850
	0    1    1    0   
$EndComp
Text Notes 3175 3150 0    47   ~ 0
Parallel CS resistors\n1mV = 1A
$Comp
L ZXCT1082 U?
U 1 1 57013549
P 2950 2000
F 0 "U?" H 2550 2200 60  0000 R CNN
F 1 "ZXCT1082" H 2550 2300 60  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 2950 2000 60  0001 C CNN
F 3 "" H 2950 2000 60  0000 C CNN
F 4 "ZXCT1082E5TA" H 2950 2000 60  0001 C CNN "part"
F 5 "1904025" H 2950 2000 60  0001 C CNN "farnell"
	1    2950 2000
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 57014201
P 3200 2800
F 0 "R?" H 3270 2846 50  0000 L CNN
F 1 "100" H 3270 2755 50  0000 L CNN
F 2 "" V 3130 2800 50  0000 C CNN
F 3 "" H 3200 2800 50  0000 C CNN
	1    3200 2800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57014243
P 2850 2800
F 0 "R?" H 2675 2875 50  0000 L CNN
F 1 "100" H 2650 2775 50  0000 L CNN
F 2 "" V 2780 2800 50  0000 C CNN
F 3 "" H 2850 2800 50  0000 C CNN
	1    2850 2800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570143A0
P 2550 2150
F 0 "R?" H 2525 2200 50  0000 L CNN
F 1 "10k" H 2500 2125 50  0000 L CNN
F 2 "" V 2480 2150 50  0000 C CNN
F 3 "" H 2550 2150 50  0000 C CNN
	1    2550 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 570143EA
P 2550 2300
F 0 "#PWR?" H 2550 2050 50  0001 C CNN
F 1 "GND" H 2558 2127 50  0001 C CNN
F 2 "" H 2550 2300 50  0000 C CNN
F 3 "" H 2550 2300 50  0000 C CNN
	1    2550 2300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57014418
P 3500 1700
F 0 "C?" H 3615 1746 50  0000 L CNN
F 1 "100n" H 3615 1655 50  0000 L CNN
F 2 "" H 3538 1550 50  0000 C CNN
F 3 "" H 3500 1700 50  0000 C CNN
	1    3500 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5701449A
P 3250 1550
F 0 "#PWR?" H 3250 1300 50  0001 C CNN
F 1 "GND" H 3258 1377 50  0001 C CNN
F 2 "" H 3250 1550 50  0000 C CNN
F 3 "" H 3250 1550 50  0000 C CNN
	1    3250 1550
	1    0    0    -1  
$EndComp
Text GLabel 3500 2000 2    60   Input ~ 0
+3.3V
Text Notes 2850 5225 0    60   ~ 0
protection circuit when\nwiring short +bat to motor-\ntrigger on 50A (50mV)
$Comp
L R R?
U 1 1 570153B3
P 3900 4175
F 0 "R?" V 3975 4175 50  0000 C CNN
F 1 "10" V 3900 4150 50  0000 C CNN
F 2 "" V 3830 4175 50  0000 C CNN
F 3 "" H 3900 4175 50  0000 C CNN
	1    3900 4175
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57015450
P 2400 4175
F 0 "R?" V 2475 4175 50  0000 C CNN
F 1 "10" V 2400 4150 50  0000 C CNN
F 2 "" V 2330 4175 50  0000 C CNN
F 3 "" H 2400 4175 50  0000 C CNN
	1    2400 4175
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 570154B6
P 2400 3650
F 0 "R?" V 2325 3650 50  0000 C CNN
F 1 "10" V 2400 3625 50  0000 C CNN
F 2 "" V 2330 3650 50  0000 C CNN
F 3 "" H 2400 3650 50  0000 C CNN
	1    2400 3650
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57015531
P 3900 3650
F 0 "R?" V 3800 3650 50  0000 C CNN
F 1 "10" V 3900 3625 50  0000 C CNN
F 2 "" V 3830 3650 50  0000 C CNN
F 3 "" H 3900 3650 50  0000 C CNN
	1    3900 3650
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57015F90
P 3900 3925
F 0 "C?" H 3825 3825 50  0000 C CNN
F 1 "1u" H 3825 4025 50  0000 C CNN
F 2 "" H 3938 3775 50  0000 C CNN
F 3 "" H 3900 3925 50  0000 C CNN
	1    3900 3925
	-1   0    0    1   
$EndComp
$Comp
L D D?
U 1 1 57016617
P 4425 3525
F 0 "D?" H 4450 3425 50  0000 R CNN
F 1 "1n4148W" H 4575 3625 50  0000 R CNN
F 2 "" H 4425 3525 50  0000 C CNN
F 3 "" H 4425 3525 50  0000 C CNN
	1    4425 3525
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57016A96
P 4925 3525
F 0 "C?" V 4775 3475 50  0000 L CNN
F 1 "100n" V 5050 3450 50  0000 L CNN
F 2 "" H 4963 3375 50  0000 C CNN
F 3 "" H 4925 3525 50  0000 C CNN
	1    4925 3525
	0    1    1    0   
$EndComp
$Comp
L BSS138 Q?
U 1 1 57017D07
P 3950 3150
F 0 "Q?" H 4143 3196 50  0000 L CNN
F 1 "2N7002" H 4143 3105 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 4144 3059 50  0001 L CIN
F 3 "" H 3950 3150 50  0000 L CNN
F 4 "2N7002" H 3950 3150 60  0001 C CNN "part"
F 5 "1510761" H 3950 3150 60  0001 C CNN "farnell"
	1    3950 3150
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57018CF5
P 5075 3525
F 0 "#PWR?" H 5075 3325 50  0001 C CNN
F 1 "GNDPWR" H 5082 3599 50  0001 C CNN
F 2 "" H 5075 3475 50  0000 C CNN
F 3 "" H 5075 3475 50  0000 C CNN
	1    5075 3525
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57018D81
P 4675 4525
F 0 "#PWR?" H 4675 4325 50  0001 C CNN
F 1 "GNDPWR" H 4682 4599 50  0001 C CNN
F 2 "" H 4675 4475 50  0000 C CNN
F 3 "" H 4675 4475 50  0000 C CNN
	1    4675 4525
	1    0    0    -1  
$EndComp
Text GLabel 4350 2650 1    60   Input ~ 0
gate_drive_24V
$Comp
L D D?
U 1 1 5701AFCE
P 1825 3525
F 0 "D?" H 1825 3625 50  0000 C CNN
F 1 "1n4148W" H 1900 3450 50  0000 C CNN
F 2 "" H 1825 3525 50  0000 C CNN
F 3 "" H 1825 3525 50  0000 C CNN
	1    1825 3525
	-1   0    0    1   
$EndComp
$Comp
L C C?
U 1 1 5701B08C
P 1325 3525
F 0 "C?" V 1073 3525 50  0000 C CNN
F 1 "C" V 1164 3525 50  0000 C CNN
F 2 "" H 1363 3375 50  0000 C CNN
F 3 "" H 1325 3525 50  0000 C CNN
	1    1325 3525
	0    1    1    0   
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5701B14E
P 1575 4525
F 0 "#PWR?" H 1575 4325 50  0001 C CNN
F 1 "GNDPWR" H 1582 4599 50  0001 C CNN
F 2 "" H 1575 4475 50  0000 C CNN
F 3 "" H 1575 4475 50  0000 C CNN
	1    1575 4525
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 5701B36E
P 2350 3175
F 0 "Q?" H 2550 3175 50  0000 L CNN
F 1 "2N7002" H 2550 3100 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 2543 3084 50  0001 L CIN
F 3 "" H 2350 3175 50  0000 L CNN
F 4 "2N7002" H 2350 3175 60  0001 C CNN "part"
F 5 "1510761" H 2350 3175 60  0001 C CNN "farnell"
	1    2350 3175
	-1   0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5701B66A
P 2400 3925
F 0 "C?" H 2325 3825 50  0000 C CNN
F 1 "1u" H 2325 4025 50  0000 C CNN
F 2 "" H 2438 3775 50  0000 C CNN
F 3 "" H 2400 3925 50  0000 C CNN
	1    2400 3925
	-1   0    0    1   
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5701CA40
P 1175 3525
F 0 "#PWR?" H 1175 3325 50  0001 C CNN
F 1 "GNDPWR" H 1182 3599 50  0001 C CNN
F 2 "" H 1175 3475 50  0000 C CNN
F 3 "" H 1175 3475 50  0000 C CNN
	1    1175 3525
	1    0    0    -1  
$EndComp
Text GLabel 1950 2675 0    60   Input ~ 0
gate_drive_24V
Text GLabel 1475 3150 0    60   Input ~ 0
+12V_bat_safe
Text GLabel 4775 3375 1    60   Input ~ 0
+12V_bat_safe
$Comp
L R R?
U 1 1 5701E0EE
P 2500 4600
F 0 "R?" V 2600 4600 50  0000 R CNN
F 1 "100" V 2500 4650 50  0000 R CNN
F 2 "" V 2430 4600 50  0000 C CNN
F 3 "" H 2500 4600 50  0000 C CNN
	1    2500 4600
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 5701E14E
P 1900 4750
F 0 "R?" H 2050 4700 50  0000 C CNN
F 1 "10k" H 2075 4775 50  0000 C CNN
F 2 "" V 1830 4750 50  0000 C CNN
F 3 "" H 1900 4750 50  0000 C CNN
	1    1900 4750
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 5701E1AC
P 2500 4475
F 0 "R?" V 2575 4500 50  0000 C CNN
F 1 "10k" V 2500 4475 50  0000 C CNN
F 2 "" V 2430 4475 50  0000 C CNN
F 3 "" H 2500 4475 50  0000 C CNN
	1    2500 4475
	0    -1   -1   0   
$EndComp
$Comp
L LM2903 U?
U 1 1 5701E910
P 2200 5000
F 0 "U?" V 2375 4775 50  0000 L CNN
F 1 "LM2903" V 2275 4625 50  0000 L CNN
F 2 "" H 2200 5000 50  0000 C CNN
F 3 "" H 2200 5000 50  0000 C CNN
	1    2200 5000
	0    -1   1    0   
$EndComp
$Comp
L C C?
U 1 1 5701EE32
P 1900 5050
F 0 "C?" V 1648 5050 50  0000 C CNN
F 1 "100n" V 1739 5050 50  0000 C CNN
F 2 "" H 1938 4900 50  0000 C CNN
F 3 "" H 1900 5050 50  0000 C CNN
	1    1900 5050
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5701EEBF
P 1900 5200
F 0 "#PWR?" H 1900 5000 50  0001 C CNN
F 1 "GNDPWR" H 1907 5274 50  0001 C CNN
F 2 "" H 1900 5150 50  0000 C CNN
F 3 "" H 1900 5150 50  0000 C CNN
	1    1900 5200
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5701EFC1
P 2650 4900
F 0 "#PWR?" H 2650 4700 50  0001 C CNN
F 1 "GNDPWR" H 2657 4974 50  0001 C CNN
F 2 "" H 2650 4850 50  0000 C CNN
F 3 "" H 2650 4850 50  0000 C CNN
	1    2650 4900
	1    0    0    -1  
$EndComp
$Comp
L IRS2304 U?
U 1 1 570222D0
P 4625 4025
F 0 "U?" H 4225 4300 60  0000 C CNN
F 1 "IRS2304" H 4100 4225 60  0000 C CNN
F 2 "" H 4575 3925 60  0000 C CNN
F 3 "" H 4575 3925 60  0000 C CNN
	1    4625 4025
	-1   0    0    -1  
$EndComp
Text GLabel 1775 5525 0    60   Input ~ 0
+3.3V
Wire Wire Line
	2850 4850 3450 4850
Connection ~ 3175 4850
Wire Wire Line
	2850 4375 2850 4550
Wire Wire Line
	3450 4375 3450 4550
Wire Wire Line
	3450 3400 3450 3450
Wire Wire Line
	2850 3400 2850 3450
Wire Wire Line
	3450 3850 3450 3975
Wire Wire Line
	2850 3850 2850 3975
Wire Wire Line
	3450 3850 3300 3850
Wire Wire Line
	3000 3850 2850 3850
Wire Wire Line
	2625 3975 3050 3975
Wire Wire Line
	3225 3975 3675 3975
Wire Wire Line
	3450 4550 2850 4550
Wire Wire Line
	2850 3400 3450 3400
Wire Wire Line
	2850 2950 2850 3250
Wire Wire Line
	3150 3000 3150 3400
Wire Wire Line
	2850 3000 2500 3000
Connection ~ 3150 3400
Connection ~ 3150 3250
Connection ~ 2850 3000
Wire Wire Line
	3200 2950 3200 3000
Wire Wire Line
	3200 3000 3150 3000
Wire Wire Line
	3500 1850 3500 2000
Wire Wire Line
	3000 1550 3500 1550
Connection ~ 3250 1550
Connection ~ 3450 3975
Wire Wire Line
	4575 3525 4775 3525
Wire Wire Line
	4275 3525 4225 3525
Wire Wire Line
	4225 3375 4225 3775
Connection ~ 4225 3525
Wire Wire Line
	2500 3000 2500 2475
Connection ~ 2850 3975
Wire Wire Line
	1975 3525 2025 3525
Wire Wire Line
	1675 3525 1475 3525
Connection ~ 2025 3525
Wire Wire Line
	4775 3525 4775 3375
Wire Wire Line
	1475 3525 1475 3150
Wire Wire Line
	1900 4600 2350 4600
Wire Wire Line
	2650 4600 2650 4900
Wire Wire Line
	2650 4900 2500 4900
Wire Wire Line
	2100 4700 2100 4600
Wire Wire Line
	1900 4900 1675 4900
Connection ~ 2100 4600
Wire Wire Line
	2300 4700 2300 4475
Wire Wire Line
	2300 4475 2350 4475
Wire Wire Line
	2650 4475 2850 4475
Connection ~ 2850 4475
$Comp
L R R?
U 1 1 5702654A
P 1925 5525
F 0 "R?" V 1825 5500 50  0000 C CNN
F 1 "10k" V 1925 5525 50  0000 C CNN
F 2 "" V 1855 5525 50  0000 C CNN
F 3 "" H 1925 5525 50  0000 C CNN
	1    1925 5525
	0    1    1    0   
$EndComp
Wire Wire Line
	2200 5300 2200 5525
Wire Wire Line
	2075 5525 2275 5525
Text Notes 2375 1650 0    60   ~ 0
Gain = 100\n3.3V = 33A
Wire Wire Line
	2025 2875 2025 2925
Wire Wire Line
	2550 2000 2550 1800
$Comp
L IRS2304 U?
U 1 1 5702A1D6
P 1625 4025
F 0 "U?" H 1000 4300 60  0000 C CNN
F 1 "IRS2304" H 1125 4200 60  0000 C CNN
F 2 "" H 1575 3925 60  0000 C CNN
F 3 "" H 1575 3925 60  0000 C CNN
	1    1625 4025
	1    0    0    -1  
$EndComp
Text HLabel 875  3950 1    60   Input ~ 0
LeftRearHI+
Text HLabel 875  4100 3    60   Input ~ 0
LeftRearLo+
Wire Wire Line
	1125 4075 875  4075
Wire Wire Line
	875  4075 875  4100
Wire Wire Line
	1125 3975 875  3975
Wire Wire Line
	875  3975 875  3950
Text HLabel 5400 3950 1    60   Input ~ 0
leftRearHi-
Text HLabel 5400 4100 3    60   Input ~ 0
leftRearLo-
Wire Wire Line
	5400 3950 5400 3975
Wire Wire Line
	5400 3975 5125 3975
Wire Wire Line
	5400 4100 5400 4075
Wire Wire Line
	5400 4075 5125 4075
Connection ~ 2200 5525
$Comp
L LM2903 U?
U 2 1 5702B993
P 1475 2125
F 0 "U?" H 1600 2275 50  0000 L CNN
F 1 "LM2903" H 1550 1975 50  0000 L CNN
F 2 "" H 1475 2125 50  0000 C CNN
F 3 "" H 1475 2125 50  0000 C CNN
	2    1475 2125
	-1   0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5702BD1E
P 2400 1800
F 0 "R?" V 2300 1750 50  0000 C CNN
F 1 "10k" V 2400 1775 50  0000 C CNN
F 2 "" V 2330 1800 50  0000 C CNN
F 3 "" H 2400 1800 50  0000 C CNN
	1    2400 1800
	0    1    1    0   
$EndComp
$Comp
L ZENER D?
U 1 1 5702C156
P 2250 2025
F 0 "D?" V 2250 2100 50  0000 L CNN
F 1 "3.3V" V 2125 2050 50  0000 L CNN
F 2 "" H 2250 2025 50  0000 C CNN
F 3 "" H 2250 2025 50  0000 C CNN
	1    2250 2025
	0    1    1    0   
$EndComp
Wire Wire Line
	2550 2300 2250 2300
Wire Wire Line
	2250 2300 2250 2225
Wire Wire Line
	2250 1800 2250 1825
Text GLabel 1475 1825 0    60   Input ~ 0
+12V_power
Wire Wire Line
	1575 1825 1475 1825
Wire Wire Line
	2150 2225 1775 2225
Wire Wire Line
	2150 1750 2150 2225
Wire Wire Line
	2150 1800 2250 1800
Connection ~ 2150 1800
Wire Wire Line
	1175 2125 875  2125
Wire Wire Line
	875  2125 875  2150
Text GLabel 1675 4900 0    60   Input ~ 0
+12V_power
Text GLabel 1775 2025 2    60   Input ~ 0
+3.3V
Text Notes 850  1725 0    60   ~ 0
Overcurrent protection\nTrigger on 33A = 3.3V
$Comp
L Q_NMOS_GDS Q?
U 1 1 5703137A
P 7875 4150
F 0 "Q?" H 8069 4196 50  0000 L CNN
F 1 "IPD031N06L3 G" H 7350 4000 50  0000 L CNN
F 2 "" H 8075 4250 50  0000 C CNN
F 3 "" H 7875 4150 50  0000 C CNN
F 4 "IPD031N06L3 G" H 7875 4150 60  0001 C CNN "part"
F 5 "2480814" H 7875 4150 60  0001 C CNN "farnell"
	1    7875 4150
	1    0    0    -1  
$EndComp
$Comp
L Q_NMOS_GDS Q?
U 1 1 57031382
P 8675 4150
F 0 "Q?" H 8869 4104 50  0000 L CNN
F 1 "IPD031N06L3 G" H 8150 4350 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TO-252-2Lead" H 8869 4241 50  0001 L CNN
F 3 "" H 8675 4150 50  0000 C CNN
F 4 "IPD031N06L3 G" H 8675 4150 60  0001 C CNN "part"
F 5 "2480814" H 8675 4150 60  0001 C CNN "farnell"
	1    8675 4150
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GDS Q?
U 1 1 5703138A
P 8675 3625
F 0 "Q?" H 8869 3579 50  0000 L CNN
F 1 "IPD031N06L3 G" H 8300 3425 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:TO-252-2Lead" H 8869 3716 50  0001 L CNN
F 3 "" H 8675 3625 50  0000 C CNN
F 4 "IPD031N06L3 G" H 8675 3625 60  0001 C CNN "part"
F 5 "2480814" H 8675 3625 60  0001 C CNN "farnell"
	1    8675 3625
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GDS Q?
U 1 1 57031392
P 7875 3625
F 0 "Q?" H 8069 3671 50  0000 L CNN
F 1 "IPD031N06L3 G" H 7350 3775 50  0000 L CNN
F 2 "" H 8075 3725 50  0000 C CNN
F 3 "" H 7875 3625 50  0000 C CNN
F 4 "IPD031N06L3 G" H 7875 3625 60  0001 C CNN "part"
F 5 "2480814" H 7875 3625 60  0001 C CNN "farnell"
	1    7875 3625
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57031398
P 8300 4825
F 0 "#PWR?" H 8300 4625 50  0001 C CNN
F 1 "GNDPWR" H 8307 4899 50  0001 C CNN
F 2 "" H 8300 4775 50  0000 C CNN
F 3 "" H 8300 4775 50  0000 C CNN
	1    8300 4825
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570313A0
P 7975 4675
F 0 "R?" H 7900 4725 50  0000 L CNN
F 1 "0.001" H 7900 4625 50  0000 L CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 8045 4584 50  0001 L CNN
F 3 "" H 7975 4675 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 7975 4675 60  0001 C CNN "part"
F 5 "2079420" H 7975 4675 60  0001 C CNN "farnell"
	1    7975 4675
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570313A8
P 8575 4675
F 0 "R?" H 8645 4721 50  0000 L CNN
F 1 "0.001" H 8645 4630 50  0000 L CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 8645 4584 50  0001 L CNN
F 3 "" H 8575 4675 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 8575 4675 60  0001 C CNN "part"
F 5 "2079420" H 8575 4675 60  0001 C CNN "farnell"
	1    8575 4675
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570313B0
P 8125 2975
F 0 "R?" V 7918 2975 50  0000 C CNN
F 1 "0.001" V 8009 2975 50  0000 C CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 8195 2884 50  0001 L CNN
F 3 "" H 8125 2975 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 8125 2975 60  0001 C CNN "part"
F 5 "2079420" H 8125 2975 60  0001 C CNN "farnell"
	1    8125 2975
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 570313B8
P 8125 3225
F 0 "R?" V 7950 3250 50  0000 R CNN
F 1 "0.001" V 8025 3300 50  0000 R CNN
F 2 "Resistors_SMD:R_2512_HandSoldering" H 8195 3134 50  0001 L CNN
F 3 "" H 8125 3225 50  0000 C CNN
F 4 "LRMAT2512-R002FT4" H 8125 3225 60  0001 C CNN "part"
F 5 "2079420" H 8125 3225 60  0001 C CNN "farnell"
	1    8125 3225
	0    1    1    0   
$EndComp
Text GLabel 7625 2475 0    60   Input ~ 0
+12V_power
$Comp
L C C?
U 1 1 570313BF
P 8275 3825
F 0 "C?" V 8023 3825 50  0000 C CNN
F 1 "220nF" V 8114 3825 50  0000 C CNN
F 2 "" H 8313 3675 50  0000 C CNN
F 3 "" H 8275 3825 50  0000 C CNN
	1    8275 3825
	0    1    1    0   
$EndComp
Text Notes 8300 3125 0    47   ~ 0
Parallel CS resistors\n1mV = 1A
$Comp
L ZXCT1082 U?
U 1 1 570313CA
P 8075 1975
F 0 "U?" H 7675 2175 60  0000 R CNN
F 1 "ZXCT1082" H 7675 2275 60  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 8075 1975 60  0001 C CNN
F 3 "" H 8075 1975 60  0000 C CNN
F 4 "ZXCT1082E5TA" H 8075 1975 60  0001 C CNN "part"
F 5 "1904025" H 8075 1975 60  0001 C CNN "farnell"
	1    8075 1975
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 570313D0
P 8325 2775
F 0 "R?" H 8395 2821 50  0000 L CNN
F 1 "100" H 8395 2730 50  0000 L CNN
F 2 "" V 8255 2775 50  0000 C CNN
F 3 "" H 8325 2775 50  0000 C CNN
	1    8325 2775
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570313D6
P 7975 2775
F 0 "R?" H 7800 2850 50  0000 L CNN
F 1 "100" H 7775 2750 50  0000 L CNN
F 2 "" V 7905 2775 50  0000 C CNN
F 3 "" H 7975 2775 50  0000 C CNN
	1    7975 2775
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570313DC
P 7675 2125
F 0 "R?" H 7650 2175 50  0000 L CNN
F 1 "10k" H 7625 2100 50  0000 L CNN
F 2 "" V 7605 2125 50  0000 C CNN
F 3 "" H 7675 2125 50  0000 C CNN
	1    7675 2125
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 570313E2
P 7675 2275
F 0 "#PWR?" H 7675 2025 50  0001 C CNN
F 1 "GND" H 7683 2102 50  0001 C CNN
F 2 "" H 7675 2275 50  0000 C CNN
F 3 "" H 7675 2275 50  0000 C CNN
	1    7675 2275
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 570313E8
P 8625 1675
F 0 "C?" H 8740 1721 50  0000 L CNN
F 1 "100n" H 8740 1630 50  0000 L CNN
F 2 "" H 8663 1525 50  0000 C CNN
F 3 "" H 8625 1675 50  0000 C CNN
	1    8625 1675
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 570313EE
P 8375 1525
F 0 "#PWR?" H 8375 1275 50  0001 C CNN
F 1 "GND" H 8383 1352 50  0001 C CNN
F 2 "" H 8375 1525 50  0000 C CNN
F 3 "" H 8375 1525 50  0000 C CNN
	1    8375 1525
	1    0    0    -1  
$EndComp
Text GLabel 8625 1975 2    60   Input ~ 0
+3.3V
Text Notes 7975 5200 0    60   ~ 0
protection circuit when\nwiring short +bat to motor-\ntrigger on 50A (50mV)
$Comp
L R R?
U 1 1 570313F7
P 9025 4150
F 0 "R?" V 9125 4150 50  0000 C CNN
F 1 "10" V 9025 4125 50  0000 C CNN
F 2 "" V 8955 4150 50  0000 C CNN
F 3 "" H 9025 4150 50  0000 C CNN
	1    9025 4150
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 570313FD
P 7525 4150
F 0 "R?" V 7450 4075 50  0000 C CNN
F 1 "10" V 7525 4125 50  0000 C CNN
F 2 "" V 7455 4150 50  0000 C CNN
F 3 "" H 7525 4150 50  0000 C CNN
	1    7525 4150
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57031403
P 7525 3625
F 0 "R?" V 7625 3575 50  0000 C CNN
F 1 "10" V 7525 3600 50  0000 C CNN
F 2 "" V 7455 3625 50  0000 C CNN
F 3 "" H 7525 3625 50  0000 C CNN
	1    7525 3625
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57031409
P 9025 3625
F 0 "R?" V 8925 3625 50  0000 C CNN
F 1 "10" V 9025 3600 50  0000 C CNN
F 2 "" V 8955 3625 50  0000 C CNN
F 3 "" H 9025 3625 50  0000 C CNN
	1    9025 3625
	0    1    1    0   
$EndComp
$Comp
L D D?
U 1 1 57031415
P 9550 3500
F 0 "D?" H 9575 3400 50  0000 R CNN
F 1 "1n4148W" H 9700 3600 50  0000 R CNN
F 2 "" H 9550 3500 50  0000 C CNN
F 3 "" H 9550 3500 50  0000 C CNN
	1    9550 3500
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5703141B
P 10050 3500
F 0 "C?" V 9900 3450 50  0000 L CNN
F 1 "100n" V 10175 3425 50  0000 L CNN
F 2 "" H 10088 3350 50  0000 C CNN
F 3 "" H 10050 3500 50  0000 C CNN
	1    10050 3500
	0    1    1    0   
$EndComp
$Comp
L BSS138 Q?
U 1 1 57031423
P 9075 3150
F 0 "Q?" H 9268 3196 50  0000 L CNN
F 1 "2N7002" H 9268 3105 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 9269 3059 50  0001 L CIN
F 3 "" H 9075 3150 50  0000 L CNN
F 4 "2N7002" H 9075 3150 60  0001 C CNN "part"
F 5 "1510761" H 9075 3150 60  0001 C CNN "farnell"
	1    9075 3150
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57031429
P 10200 3500
F 0 "#PWR?" H 10200 3300 50  0001 C CNN
F 1 "GNDPWR" H 10207 3574 50  0001 C CNN
F 2 "" H 10200 3450 50  0000 C CNN
F 3 "" H 10200 3450 50  0000 C CNN
	1    10200 3500
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5703142F
P 9800 4500
F 0 "#PWR?" H 9800 4300 50  0001 C CNN
F 1 "GNDPWR" H 9807 4574 50  0001 C CNN
F 2 "" H 9800 4450 50  0000 C CNN
F 3 "" H 9800 4450 50  0000 C CNN
	1    9800 4500
	1    0    0    -1  
$EndComp
Text GLabel 9475 2650 1    60   Input ~ 0
gate_drive_24V
$Comp
L D D?
U 1 1 57031436
P 6950 3500
F 0 "D?" H 6950 3600 50  0000 C CNN
F 1 "1n4148W" H 7025 3425 50  0000 C CNN
F 2 "" H 6950 3500 50  0000 C CNN
F 3 "" H 6950 3500 50  0000 C CNN
	1    6950 3500
	-1   0    0    1   
$EndComp
$Comp
L C C?
U 1 1 5703143C
P 6450 3500
F 0 "C?" V 6198 3500 50  0000 C CNN
F 1 "C" V 6289 3500 50  0000 C CNN
F 2 "" H 6488 3350 50  0000 C CNN
F 3 "" H 6450 3500 50  0000 C CNN
	1    6450 3500
	0    1    1    0   
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57031442
P 6700 4500
F 0 "#PWR?" H 6700 4300 50  0001 C CNN
F 1 "GNDPWR" H 6707 4574 50  0001 C CNN
F 2 "" H 6700 4450 50  0000 C CNN
F 3 "" H 6700 4450 50  0000 C CNN
	1    6700 4500
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 5703144A
P 7475 3150
F 0 "Q?" H 7669 3196 50  0000 L CNN
F 1 "2N7002" H 7700 3125 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 7668 3059 50  0001 L CIN
F 3 "" H 7475 3150 50  0000 L CNN
F 4 "2N7002" H 7475 3150 60  0001 C CNN "part"
F 5 "1510761" H 7475 3150 60  0001 C CNN "farnell"
	1    7475 3150
	-1   0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57031450
P 7525 3900
F 0 "C?" H 7450 3800 50  0000 C CNN
F 1 "1u" H 7450 3975 50  0000 C CNN
F 2 "" H 7563 3750 50  0000 C CNN
F 3 "" H 7525 3900 50  0000 C CNN
	1    7525 3900
	-1   0    0    1   
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57031456
P 6300 3500
F 0 "#PWR?" H 6300 3300 50  0001 C CNN
F 1 "GNDPWR" H 6307 3574 50  0001 C CNN
F 2 "" H 6300 3450 50  0000 C CNN
F 3 "" H 6300 3450 50  0000 C CNN
	1    6300 3500
	1    0    0    -1  
$EndComp
Text GLabel 7075 2650 0    60   Input ~ 0
gate_drive_24V
Text GLabel 6600 3125 0    60   Input ~ 0
+12V_bat_safe
Text GLabel 9900 3350 1    60   Input ~ 0
+12V_bat_safe
$Comp
L R R?
U 1 1 5703145F
P 7625 4575
F 0 "R?" V 7725 4575 50  0000 R CNN
F 1 "100" V 7625 4625 50  0000 R CNN
F 2 "" V 7555 4575 50  0000 C CNN
F 3 "" H 7625 4575 50  0000 C CNN
	1    7625 4575
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57031465
P 7025 4725
F 0 "R?" H 7175 4675 50  0000 C CNN
F 1 "10k" H 7200 4750 50  0000 C CNN
F 2 "" V 6955 4725 50  0000 C CNN
F 3 "" H 7025 4725 50  0000 C CNN
	1    7025 4725
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 5703146B
P 7625 4450
F 0 "R?" V 7700 4475 50  0000 C CNN
F 1 "10k" V 7625 4450 50  0000 C CNN
F 2 "" V 7555 4450 50  0000 C CNN
F 3 "" H 7625 4450 50  0000 C CNN
	1    7625 4450
	0    -1   -1   0   
$EndComp
$Comp
L LM2903 U?
U 1 1 57031471
P 7325 4975
F 0 "U?" V 7500 4750 50  0000 L CNN
F 1 "LM2903" V 7400 4600 50  0000 L CNN
F 2 "" H 7325 4975 50  0000 C CNN
F 3 "" H 7325 4975 50  0000 C CNN
	1    7325 4975
	0    -1   1    0   
$EndComp
$Comp
L C C?
U 1 1 57031477
P 7025 5025
F 0 "C?" V 6773 5025 50  0000 C CNN
F 1 "100n" V 6864 5025 50  0000 C CNN
F 2 "" H 7063 4875 50  0000 C CNN
F 3 "" H 7025 5025 50  0000 C CNN
	1    7025 5025
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5703147D
P 7025 5175
F 0 "#PWR?" H 7025 4975 50  0001 C CNN
F 1 "GNDPWR" H 7032 5249 50  0001 C CNN
F 2 "" H 7025 5125 50  0000 C CNN
F 3 "" H 7025 5125 50  0000 C CNN
	1    7025 5175
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57031483
P 7775 4875
F 0 "#PWR?" H 7775 4675 50  0001 C CNN
F 1 "GNDPWR" H 7782 4949 50  0001 C CNN
F 2 "" H 7775 4825 50  0000 C CNN
F 3 "" H 7775 4825 50  0000 C CNN
	1    7775 4875
	1    0    0    -1  
$EndComp
Text GLabel 6900 5500 0    60   Input ~ 0
+3.3V
Wire Wire Line
	7975 4825 8575 4825
Connection ~ 8300 4825
Wire Wire Line
	7975 4350 7975 4525
Wire Wire Line
	8575 4350 8575 4525
Wire Wire Line
	8575 3375 8575 3425
Wire Wire Line
	7975 3375 7975 3425
Wire Wire Line
	8575 3825 8575 3950
Wire Wire Line
	7975 3825 7975 3950
Wire Wire Line
	8575 3825 8425 3825
Wire Wire Line
	8125 3825 7975 3825
Wire Wire Line
	7725 3950 8175 3950
Wire Wire Line
	8350 3950 8775 3950
Wire Wire Line
	8575 4525 7975 4525
Wire Wire Line
	7975 3375 8575 3375
Wire Wire Line
	7975 2925 7975 3225
Wire Wire Line
	8275 2975 8275 3375
Wire Wire Line
	7975 2975 7625 2975
Connection ~ 8275 3375
Connection ~ 8275 3225
Connection ~ 7975 2975
Wire Wire Line
	8325 2925 8325 2975
Wire Wire Line
	8325 2975 8275 2975
Wire Wire Line
	8625 1825 8625 1975
Wire Wire Line
	8125 1525 8625 1525
Connection ~ 8375 1525
Connection ~ 8575 3950
Wire Wire Line
	9400 3500 9350 3500
Wire Wire Line
	9350 3325 9350 3750
Connection ~ 9350 3500
Wire Wire Line
	8875 3625 8875 3200
Wire Wire Line
	7625 2975 7625 2475
Connection ~ 7975 3950
Wire Wire Line
	7100 3500 7150 3500
Wire Wire Line
	6800 3500 6600 3500
Connection ~ 7150 3500
Wire Wire Line
	9900 3500 9900 3350
Wire Wire Line
	6600 3500 6600 3125
Wire Wire Line
	7025 4575 7475 4575
Wire Wire Line
	7775 4575 7775 4875
Wire Wire Line
	7775 4875 7625 4875
Wire Wire Line
	7225 4675 7225 4575
Wire Wire Line
	7025 4875 6800 4875
Connection ~ 7225 4575
Wire Wire Line
	7425 4675 7425 4450
Wire Wire Line
	7425 4450 7475 4450
Wire Wire Line
	7775 4450 7975 4450
Connection ~ 7975 4450
$Comp
L R R?
U 1 1 570314DA
P 7050 5500
F 0 "R?" V 6950 5475 50  0000 C CNN
F 1 "10k" V 7050 5500 50  0000 C CNN
F 2 "" V 6980 5500 50  0000 C CNN
F 3 "" H 7050 5500 50  0000 C CNN
	1    7050 5500
	0    1    1    0   
$EndComp
Wire Wire Line
	7325 5275 7325 5500
Wire Wire Line
	7200 5500 7400 5500
Text Notes 7500 1625 0    60   ~ 0
Gain = 100\n3.3V = 33A
Wire Wire Line
	7150 2850 7150 2900
Wire Wire Line
	7675 1975 7675 1775
$Comp
L IRS2304 U?
U 1 1 570314E6
P 6750 4000
F 0 "U?" H 6125 4275 60  0000 C CNN
F 1 "IRS2304" H 6250 4175 60  0000 C CNN
F 2 "" H 6700 3900 60  0000 C CNN
F 3 "" H 6700 3900 60  0000 C CNN
	1    6750 4000
	1    0    0    -1  
$EndComp
Text HLabel 6000 3925 1    60   Input ~ 0
RightRearHI+
Text HLabel 6000 4075 3    60   Input ~ 0
RightRearLo+
Wire Wire Line
	6250 4050 6000 4050
Wire Wire Line
	6000 4050 6000 4075
Wire Wire Line
	6250 3950 6000 3950
Wire Wire Line
	6000 3950 6000 3925
Text HLabel 10525 3925 1    60   Input ~ 0
RightRearHi-
Text HLabel 10525 4075 3    60   Input ~ 0
RightRearLo-
Wire Wire Line
	10525 3925 10525 3950
Wire Wire Line
	10525 3950 10250 3950
Wire Wire Line
	10525 4075 10525 4050
Wire Wire Line
	10525 4050 10250 4050
Connection ~ 7325 5500
$Comp
L LM2903 U?
U 1 1 570314F9
P 6600 2100
F 0 "U?" H 6725 2250 50  0000 L CNN
F 1 "LM2903" H 6675 1950 50  0000 L CNN
F 2 "" H 6600 2100 50  0000 C CNN
F 3 "" H 6600 2100 50  0000 C CNN
	1    6600 2100
	-1   0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570314FF
P 7525 1775
F 0 "R?" V 7425 1725 50  0000 C CNN
F 1 "10k" V 7525 1750 50  0000 C CNN
F 2 "" V 7455 1775 50  0000 C CNN
F 3 "" H 7525 1775 50  0000 C CNN
	1    7525 1775
	0    1    1    0   
$EndComp
$Comp
L ZENER D?
U 1 1 57031505
P 7375 2000
F 0 "D?" V 7375 2075 50  0000 L CNN
F 1 "3.3V" V 7250 2025 50  0000 L CNN
F 2 "" H 7375 2000 50  0000 C CNN
F 3 "" H 7375 2000 50  0000 C CNN
	1    7375 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	7675 2275 7375 2275
Wire Wire Line
	7375 2275 7375 2200
Wire Wire Line
	7375 1775 7375 1800
Text GLabel 6600 1800 0    60   Input ~ 0
+12V_power
Wire Wire Line
	6700 1800 6600 1800
Wire Wire Line
	7275 2200 6900 2200
Wire Wire Line
	7275 1725 7275 2200
Wire Wire Line
	7275 1775 7375 1775
Connection ~ 7275 1775
Wire Wire Line
	6300 2100 6000 2100
Wire Wire Line
	6000 2100 6000 2125
Text GLabel 6800 4875 0    60   Input ~ 0
+12V_power
Text GLabel 6900 2000 2    60   Input ~ 0
+3.3V
Text Notes 5975 1700 0    60   ~ 0
Overcurrent protection\nTrigger on 33A = 3.3V
$Comp
L Q_PMOS_GSD Q?
U 1 1 5703512C
P 4250 2950
F 0 "Q?" H 4443 2904 50  0000 L CNN
F 1 "BSS84" H 4443 2995 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 4444 2859 50  0001 L CNN
F 3 "" H 4250 2950 50  0000 C CNN
F 4 "BSS84" H 4250 2950 60  0001 C CNN "part"
F 5 "1510765" H 4250 2950 60  0001 C CNN "farnell"
	1    4250 2950
	1    0    0    1   
$EndComp
$Comp
L IRS2304 U?
U 1 1 570364A1
P 9750 4000
F 0 "U?" H 9350 4275 60  0000 C CNN
F 1 "IRS2304" H 9250 4200 60  0000 C CNN
F 2 "" H 9700 3900 60  0000 C CNN
F 3 "" H 9700 3900 60  0000 C CNN
	1    9750 4000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9700 3500 9900 3500
Connection ~ 9800 3500
$Comp
L C C?
U 1 1 5703140F
P 9025 3900
F 0 "C?" H 9075 4000 50  0000 C CNN
F 1 "1u" H 9075 3825 50  0000 C CNN
F 2 "" H 9063 3750 50  0000 C CNN
F 3 "" H 9025 3900 50  0000 C CNN
	1    9025 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9175 4150 9175 4200
Wire Wire Line
	9175 4200 9350 4200
Wire Wire Line
	8775 4050 9350 4050
Wire Wire Line
	9350 3900 9250 3900
Wire Wire Line
	9250 3900 9250 3625
Wire Wire Line
	9250 3625 9175 3625
Wire Wire Line
	7375 4150 7375 4200
Wire Wire Line
	7375 4200 7150 4200
Wire Wire Line
	7725 4050 7725 3950
Wire Wire Line
	7375 3625 7300 3625
Wire Wire Line
	7300 3625 7300 3900
Wire Wire Line
	7300 3900 7150 3900
Wire Wire Line
	7150 3750 7525 3750
Wire Wire Line
	7150 3350 7150 3750
Wire Wire Line
	7150 4050 7725 4050
Connection ~ 7525 4050
Wire Wire Line
	9350 3750 9025 3750
Wire Wire Line
	8775 3950 8775 4050
Connection ~ 9025 4050
Wire Wire Line
	4225 3775 3900 3775
Wire Wire Line
	3675 4075 4225 4075
Wire Wire Line
	3675 3975 3675 4075
Connection ~ 3900 4075
Wire Wire Line
	4225 3925 4125 3925
Wire Wire Line
	4125 3925 4125 3650
Wire Wire Line
	4125 3650 4050 3650
Wire Wire Line
	2025 4075 2625 4075
Wire Wire Line
	2625 4075 2625 3975
Connection ~ 2400 4075
Wire Wire Line
	2400 3775 2025 3775
Wire Wire Line
	2250 4175 2250 4225
Wire Wire Line
	2250 4225 2025 4225
Wire Wire Line
	2025 3375 2025 3925
Wire Wire Line
	2250 3650 2175 3650
Wire Wire Line
	2175 3650 2175 3925
Wire Wire Line
	2175 3925 2025 3925
$Comp
L R R?
U 1 1 5703E4A9
P 4050 2800
F 0 "R?" H 3800 2850 50  0000 L CNN
F 1 "10k" H 3800 2750 50  0000 L CNN
F 2 "" V 3980 2800 50  0000 C CNN
F 3 "" H 4050 2800 50  0000 C CNN
	1    4050 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2750 4350 2650
Wire Wire Line
	4350 2650 4050 2650
Wire Wire Line
	4225 3375 4350 3375
Wire Wire Line
	4350 3375 4350 3150
Wire Wire Line
	3750 3650 3750 3200
Wire Wire Line
	2550 3225 2550 3650
$Comp
L Q_PMOS_GSD Q?
U 1 1 5703FCCC
P 2050 2975
F 0 "Q?" H 2244 2929 50  0000 L CNN
F 1 "BSS84" H 2244 3020 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 2244 2884 50  0001 L CNN
F 3 "" H 2050 2975 50  0000 C CNN
F 4 "BSS84" H 2050 2975 60  0001 C CNN "part"
F 5 "1510765" H 2050 2975 60  0001 C CNN "farnell"
	1    2050 2975
	-1   0    0    1   
$EndComp
Wire Wire Line
	2025 3375 1950 3375
Wire Wire Line
	1950 3375 1950 3175
$Comp
L R R?
U 1 1 5704025A
P 2250 2825
F 0 "R?" H 2320 2871 50  0000 L CNN
F 1 "10k" H 2320 2780 50  0000 L CNN
F 2 "" V 2180 2825 50  0000 C CNN
F 3 "" H 2250 2825 50  0000 C CNN
	1    2250 2825
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 2675 1950 2675
Wire Wire Line
	1950 2675 1950 2775
$Comp
L Q_PMOS_GSD Q?
U 1 1 57041422
P 9375 2950
F 0 "Q?" H 9568 2904 50  0000 L CNN
F 1 "BSS84" H 9568 2995 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 9569 2859 50  0001 L CNN
F 3 "" H 9375 2950 50  0000 C CNN
F 4 "BSS84" H 9375 2950 60  0001 C CNN "part"
F 5 "1510765" H 9375 2950 60  0001 C CNN "farnell"
	1    9375 2950
	1    0    0    1   
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57042056
P 4050 3350
F 0 "#PWR?" H 4050 3150 50  0001 C CNN
F 1 "GNDPWR" H 4057 3424 50  0001 C CNN
F 2 "" H 4050 3300 50  0000 C CNN
F 3 "" H 4050 3300 50  0000 C CNN
	1    4050 3350
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 5704214E
P 2250 3375
F 0 "#PWR?" H 2250 3175 50  0001 C CNN
F 1 "GNDPWR" H 2257 3449 50  0001 C CNN
F 2 "" H 2250 3325 50  0000 C CNN
F 3 "" H 2250 3325 50  0000 C CNN
	1    2250 3375
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57042699
P 9175 3350
F 0 "#PWR?" H 9175 3150 50  0001 C CNN
F 1 "GNDPWR" H 9182 3424 50  0001 C CNN
F 2 "" H 9175 3300 50  0000 C CNN
F 3 "" H 9175 3300 50  0000 C CNN
	1    9175 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9475 3150 9475 3325
Wire Wire Line
	9475 3325 9350 3325
$Comp
L R R?
U 1 1 57042AF2
P 9175 2800
F 0 "R?" H 9000 2850 50  0000 L CNN
F 1 "10k" H 8975 2750 50  0000 L CNN
F 2 "" V 9105 2800 50  0000 C CNN
F 3 "" H 9175 2800 50  0000 C CNN
	1    9175 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9475 2750 9475 2650
Wire Wire Line
	9475 2650 9175 2650
$Comp
L Q_PMOS_GSD Q?
U 1 1 570442A2
P 7175 2950
F 0 "Q?" H 7369 2904 50  0000 L CNN
F 1 "BSS84" H 7369 2995 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 7369 2859 50  0001 L CNN
F 3 "" H 7175 2950 50  0000 C CNN
F 4 "BSS84" H 7175 2950 60  0001 C CNN "part"
F 5 "1510765" H 7175 2950 60  0001 C CNN "farnell"
	1    7175 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	7675 3625 7675 3200
$Comp
L GNDPWR #PWR?
U 1 1 57044DA1
P 7375 3350
F 0 "#PWR?" H 7375 3150 50  0001 C CNN
F 1 "GNDPWR" H 7382 3424 50  0001 C CNN
F 2 "" H 7375 3300 50  0000 C CNN
F 3 "" H 7375 3300 50  0000 C CNN
	1    7375 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 3350 7075 3350
Wire Wire Line
	7075 3350 7075 3150
$Comp
L R R?
U 1 1 57045813
P 7375 2800
F 0 "R?" H 7445 2846 50  0000 L CNN
F 1 "10k" H 7445 2755 50  0000 L CNN
F 2 "" V 7305 2800 50  0000 C CNN
F 3 "" H 7375 2800 50  0000 C CNN
	1    7375 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7375 2650 7075 2650
Wire Wire Line
	7075 2650 7075 2750
$Comp
L GNDPWR #PWR?
U 1 1 570464B5
P 1575 2425
F 0 "#PWR?" H 1575 2225 50  0001 C CNN
F 1 "GNDPWR" H 1582 2499 50  0001 C CNN
F 2 "" H 1575 2375 50  0000 C CNN
F 3 "" H 1575 2375 50  0000 C CNN
	1    1575 2425
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR?
U 1 1 57046851
P 6700 2400
F 0 "#PWR?" H 6700 2200 50  0001 C CNN
F 1 "GNDPWR" H 6707 2474 50  0001 C CNN
F 2 "" H 6700 2350 50  0000 C CNN
F 3 "" H 6700 2350 50  0000 C CNN
	1    6700 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4225 4225 4050 4225
Wire Wire Line
	4050 4225 4050 4175
Text GLabel 2275 5525 2    60   Input ~ 0
~LeftRear_CL
Text GLabel 3050 3975 3    60   Input ~ 0
LeftRearMotor+
Text GLabel 3225 3975 3    60   Input ~ 0
LeftRearMotor-
Text GLabel 2150 1750 1    60   Input ~ 0
LeftRear_CS
Text GLabel 875  2150 3    60   Input ~ 0
~LeftRear_CL
Text GLabel 6000 2125 3    60   Input ~ 0
~RightRear_CL
Text GLabel 7275 1725 1    60   Input ~ 0
RightRear_CS
Text GLabel 8175 3950 3    60   Input ~ 0
RightRearMotor+
Text GLabel 8350 3950 3    60   Input ~ 0
RightRearMotor-
Text GLabel 7400 5500 2    60   Input ~ 0
~RightRear_CL
$EndSCHEMATC
