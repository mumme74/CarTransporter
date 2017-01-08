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
Sheet 2 10
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
L 74LS00 U1
U 1 1 5704D186
P 1975 4775
F 0 "U1" H 1875 4875 50  0000 L CNN
F 1 "74LVC00" H 1750 4750 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 1975 4775 50  0001 C CNN
F 3 "" H 1975 4775 50  0000 C CNN
F 4 "SN74LVC00APWR" V 1975 4775 60  0001 C CNN "part"
F 5 "	1105931" V 1975 4775 60  0001 C CNN "farnell"
	1    1975 4775
	1    0    0    -1  
$EndComp
$Comp
L 74LS00 U1
U 2 1 570532D4
P 1975 5425
F 0 "U1" H 1875 5525 50  0000 L CNN
F 1 "74LVC00" H 1750 5400 50  0000 L CNN
F 2 "" H 1975 5425 50  0001 C CNN
F 3 "" H 1975 5425 50  0000 C CNN
	2    1975 5425
	1    0    0    -1  
$EndComp
$Sheet
S 7000 1350 1900 1875
U 57059FC7
F0 "H-BridgeRear" 60
F1 "H-BridgeRear.sch" 60
F2 "LeftRearHI+" I L 7000 1650 60 
F3 "LeftRearLo+" I L 7000 2550 60 
F4 "leftRearHi-" I L 7000 2950 60 
F5 "leftRearLo-" I L 7000 2075 60 
F6 "RightRearLo-" I R 8900 2075 60 
F7 "RightRearLo+" I R 8900 2550 60 
F8 "RightRearHI+" I R 8900 1650 60 
F9 "RightRearHi-" I R 8900 2950 60 
$EndSheet
$Comp
L 74LS00 U1
U 3 1 570625EF
P 5900 4825
F 0 "U1" H 5850 4725 50  0000 L CNN
F 1 "74LVC00" H 5675 4875 50  0000 L CNN
F 2 "" H 5900 4825 50  0001 C CNN
F 3 "" H 5900 4825 50  0000 C CNN
	3    5900 4825
	-1   0    0    1   
$EndComp
$Comp
L 74LS00 U1
U 4 1 5706623B
P 5900 5400
F 0 "U1" H 5850 5300 50  0000 L CNN
F 1 "74LVC00" H 5675 5450 50  0000 L CNN
F 2 "" H 5900 5400 50  0001 C CNN
F 3 "" H 5900 5400 50  0000 C CNN
	4    5900 5400
	-1   0    0    1   
$EndComp
$Comp
L C C1
U 1 1 5706DC6B
P 1425 1050
F 0 "C1" H 1540 1096 50  0000 L CNN
F 1 "100n" H 1550 1025 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1463 900 50  0001 C CNN
F 3 "" H 1425 1050 50  0000 C CNN
F 4 "0805B104J500CT" H 1425 1050 60  0001 C CNN "part"
F 5 "2496940" H 1425 1050 60  0001 C CNN "farnell"
	1    1425 1050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5706E47E
P 1425 1200
F 0 "#PWR019" H 1425 950 50  0001 C CNN
F 1 "GND" H 1433 1027 50  0000 C CNN
F 2 "" H 1425 1200 50  0000 C CNN
F 3 "" H 1425 1200 50  0000 C CNN
	1    1425 1200
	1    0    0    -1  
$EndComp
Text Label 2850 1200 0    60   ~ 0
GND
Text Label 2825 900  0    60   ~ 0
VCC
Text GLabel 1350 900  0    60   Input ~ 0
+3.3V
Text GLabel 10375 1925 2    60   Input ~ 0
~RightRear_CL
Text GLabel 10200 3250 2    60   Input ~ 0
RightRear_Loosen
Text GLabel 5675 3250 0    60   Input ~ 0
LeftRear_Loosen
Text GLabel 5400 1925 0    60   Input ~ 0
~LeftRear_CL
$Sheet
S 3050 4225 1750 1925
U 570485E7
F0 "H-BridgeFront" 60
F1 "H-BridgeFront.sch" 60
F2 "LeftFrontHI+" I L 3050 4425 60 
F3 "LeftFrontLo+" I L 3050 5775 60 
F4 "leftFrontHi-" I L 3050 5350 60 
F5 "leftFrontLo-" I L 3050 4875 60 
F6 "RightFrontHI+" I R 4800 4400 60 
F7 "RightFrontLo+" I R 4800 5775 60 
F8 "RightFrontHi-" I R 4800 5350 60 
F9 "RightFrontLo-" I R 4800 4875 60 
$EndSheet
$Comp
L C C2
U 1 1 5707D112
P 1900 1050
F 0 "C2" H 2015 1096 50  0000 L CNN
F 1 "100n" H 2015 1005 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1938 900 50  0001 C CNN
F 3 "" H 1900 1050 50  0000 C CNN
F 4 "0805B104J500CT" H 1900 1050 60  0001 C CNN "part"
F 5 "2496940" H 1900 1050 60  0001 C CNN "farnell"
	1    1900 1050
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5707D1B4
P 2275 1050
F 0 "C3" H 2390 1096 50  0000 L CNN
F 1 "100n" H 2390 1005 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 2313 900 50  0001 C CNN
F 3 "" H 2275 1050 50  0000 C CNN
F 4 "0805B104J500CT" H 2275 1050 60  0001 C CNN "part"
F 5 "2496940" H 2275 1050 60  0001 C CNN "farnell"
	1    2275 1050
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5707D256
P 2700 1050
F 0 "C4" H 2815 1096 50  0000 L CNN
F 1 "100n" H 2825 1025 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 2738 900 50  0001 C CNN
F 3 "" H 2700 1050 50  0000 C CNN
F 4 "0805B104J500CT" H 2700 1050 60  0001 C CNN "part"
F 5 "2496940" H 2700 1050 60  0001 C CNN "farnell"
	1    2700 1050
	1    0    0    -1  
$EndComp
$Comp
L LED D6
U 1 1 5707E22E
P 1625 1800
F 0 "D6" H 1625 2015 50  0000 C CNN
F 1 "LF_Release" H 1625 1924 50  0000 C CNN
F 2 "LEDs:LED_0805" H 1625 1800 50  0001 C CNN
F 3 "" H 1625 1800 50  0000 C CNN
F 4 "KP-2012LSGC" H 1625 1800 60  0001 C CNN "part"
F 5 "2463991" H 1625 1800 60  0001 C CNN "farnell"
	1    1625 1800
	1    0    0    -1  
$EndComp
$Comp
L LED D7
U 1 1 5707E3F8
P 1625 2025
F 0 "D7" H 1625 2225 50  0000 C CNN
F 1 "LF_Tighten" H 1625 2150 50  0000 C CNN
F 2 "LEDs:LED_0805" H 1625 2025 50  0001 C CNN
F 3 "" H 1625 2025 50  0000 C CNN
F 4 "KP-2012SRC-PRV" H 1625 2025 60  0001 C CNN "part"
F 5 "1318244" H 1625 2025 60  0001 C CNN "farnell"
	1    1625 2025
	-1   0    0    1   
$EndComp
$Comp
L LED D8
U 1 1 5707E530
P 1625 2725
F 0 "D8" H 1625 2940 50  0000 C CNN
F 1 "LR_Release" H 1625 2849 50  0000 C CNN
F 2 "LEDs:LED_0805" H 1625 2725 50  0001 C CNN
F 3 "" H 1625 2725 50  0000 C CNN
F 4 "KP-2012LSGC" H 1625 2725 60  0001 C CNN "part"
F 5 "2463991" H 1625 2725 60  0001 C CNN "farnell"
	1    1625 2725
	1    0    0    -1  
$EndComp
$Comp
L LED D9
U 1 1 5707E5E6
P 1625 2950
F 0 "D9" H 1625 3150 50  0000 C CNN
F 1 "LR_Tighten" H 1625 3075 50  0000 C CNN
F 2 "LEDs:LED_0805" H 1625 2950 50  0001 C CNN
F 3 "" H 1625 2950 50  0000 C CNN
F 4 "KP-2012SRC-PRV" H 1625 2950 60  0001 C CNN "part"
F 5 "1318244" H 1625 2950 60  0001 C CNN "farnell"
	1    1625 2950
	-1   0    0    1   
$EndComp
$Comp
L LED D10
U 1 1 5707E904
P 3100 1800
F 0 "D10" H 3100 2015 50  0000 C CNN
F 1 "RF_Release" H 3100 1924 50  0000 C CNN
F 2 "LEDs:LED_0805" H 3100 1800 50  0001 C CNN
F 3 "" H 3100 1800 50  0000 C CNN
F 4 "KP-2012LSGC" H 3100 1800 60  0001 C CNN "part"
F 5 "2463991" H 3100 1800 60  0001 C CNN "farnell"
	1    3100 1800
	1    0    0    -1  
$EndComp
$Comp
L LED D11
U 1 1 5707E9DA
P 3100 2025
F 0 "D11" H 3100 2225 50  0000 C CNN
F 1 "RF_tighten" H 3100 2150 50  0000 C CNN
F 2 "LEDs:LED_0805" H 3100 2025 50  0001 C CNN
F 3 "" H 3100 2025 50  0000 C CNN
F 4 "KP-2012SRC-PRV" H 3100 2025 60  0001 C CNN "part"
F 5 "1318244" H 3100 2025 60  0001 C CNN "farnell"
	1    3100 2025
	-1   0    0    1   
$EndComp
$Comp
L LED D12
U 1 1 5707EB22
P 3125 2725
F 0 "D12" H 3125 2940 50  0000 C CNN
F 1 "RR_Release" H 3125 2849 50  0000 C CNN
F 2 "LEDs:LED_0805" H 3125 2725 50  0001 C CNN
F 3 "" H 3125 2725 50  0000 C CNN
F 4 "KP-2012LSGC" H 3125 2725 60  0001 C CNN "part"
F 5 "2463991" H 3125 2725 60  0001 C CNN "farnell"
	1    3125 2725
	1    0    0    -1  
$EndComp
$Comp
L LED D13
U 1 1 5707EBD0
P 3125 2950
F 0 "D13" H 3125 3150 50  0000 C CNN
F 1 "RR_Tighten" H 3125 3075 50  0000 C CNN
F 2 "LEDs:LED_0805" H 3125 2950 50  0001 C CNN
F 3 "" H 3125 2950 50  0000 C CNN
F 4 "KP-2012SRC-PRV" H 3125 2950 60  0001 C CNN "part"
F 5 "1318244" H 3125 2950 60  0001 C CNN "farnell"
	1    3125 2950
	-1   0    0    1   
$EndComp
$Comp
L R R6
U 1 1 5707ED69
P 1250 1900
F 0 "R6" V 1457 1900 50  0000 C CNN
F 1 "1k" V 1366 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1180 1900 50  0001 C CNN
F 3 "" H 1250 1900 50  0000 C CNN
F 4 "9332383" V 1250 1900 60  0001 C CNN "farnell"
F 5 "MC01W080511K" V 1250 1900 60  0001 C CNN "part"
	1    1250 1900
	0    -1   -1   0   
$EndComp
$Comp
L R R7
U 1 1 5707EF24
P 1250 2850
F 0 "R7" V 1043 2850 50  0000 C CNN
F 1 "1k" V 1134 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1180 2850 50  0001 C CNN
F 3 "" H 1250 2850 50  0000 C CNN
F 4 "9332383" V 1250 2850 60  0001 C CNN "farnell"
F 5 "MC01W080511K" V 1250 2850 60  0001 C CNN "part"
	1    1250 2850
	0    1    1    0   
$EndComp
$Comp
L R R9
U 1 1 5707F028
P 3500 2850
F 0 "R9" V 3293 2850 50  0000 C CNN
F 1 "1k" V 3384 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3430 2850 50  0001 C CNN
F 3 "" H 3500 2850 50  0000 C CNN
F 4 "9332383" V 3500 2850 60  0001 C CNN "farnell"
F 5 "MC01W080511K" V 3500 2850 60  0001 C CNN "part"
	1    3500 2850
	0    1    1    0   
$EndComp
$Comp
L R R8
U 1 1 5707F12F
P 3475 1900
F 0 "R8" V 3268 1900 50  0000 C CNN
F 1 "1k" V 3359 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3405 1900 50  0001 C CNN
F 3 "" H 3475 1900 50  0000 C CNN
F 4 "9332383" V 3475 1900 60  0001 C CNN "farnell"
F 5 "MC01W080511K" V 3475 1900 60  0001 C CNN "part"
	1    3475 1900
	0    1    1    0   
$EndComp
Text GLabel 3625 1900 3    60   Input ~ 0
RightFrontMotor-
Text GLabel 2700 1900 3    60   Input ~ 0
RightFrontMotor+
Text GLabel 3650 2850 3    60   Input ~ 0
RightRearMotor-
Text GLabel 2700 2850 3    60   Input ~ 0
RightRearMotor+
Text GLabel 1950 2850 3    60   Input ~ 0
LeftRearMotor-
Text GLabel 1950 1900 3    60   Input ~ 0
LeftFrontMotor-
Text GLabel 1100 1900 3    60   Input ~ 0
LeftFrontMotor+
Text GLabel 1100 2850 3    60   Input ~ 0
LeftRearMotor+
Text GLabel 1375 4675 0    60   Input ~ 0
~LeftFront_CL
Text GLabel 6500 4725 2    60   Input ~ 0
~RightFront_CL
Text GLabel 1700 6050 0    60   Input ~ 0
LeftFront_Loosen
Text GLabel 6125 6050 2    60   Input ~ 0
RightFront_Loosen
Text GLabel 6125 4175 2    60   Input ~ 0
RightFront_Tighten
Text GLabel 1700 4175 0    60   Input ~ 0
LeftFront_Tighten
Text GLabel 5675 1350 0    60   Input ~ 0
LeftRear_Tighten
Text GLabel 10200 1375 2    60   Input ~ 0
RightRear_Tighten
$Comp
L 74LS08 U2
U 1 1 57135EB3
P 2300 4275
F 0 "U2" H 2275 4400 50  0000 C CNN
F 1 "74LVC08" H 2325 4275 50  0000 C CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 2300 4275 50  0001 C CNN
F 3 "" H 2300 4275 50  0000 C CNN
F 4 "2445097" H 2300 4275 60  0001 C CNN "farnell"
F 5 "74LVC08APW-Q100.11" H 2300 4275 60  0001 C CNN "part"
	1    2300 4275
	1    0    0    -1  
$EndComp
$Comp
L 74LS08 U2
U 2 1 5713618B
P 2300 5950
F 0 "U2" H 2300 6050 50  0000 C CNN
F 1 "74LVC08" H 2300 5925 50  0000 C CNN
F 2 "" H 2300 5950 50  0001 C CNN
F 3 "" H 2300 5950 50  0000 C CNN
	2    2300 5950
	1    0    0    -1  
$EndComp
Text GLabel 1375 5525 0    60   Input ~ 0
Bridge_Disable
Text Notes 7900 4225 0    60   ~ 0
~SR~ latch tripped by overcurrent, must be reset by software\nCurrentLimit triggers the SR latch, the xx_DIAG is the output \nof this latch, is high when the latch has triggered.
$Comp
L 74LS08 U2
U 3 1 5713C42D
P 5525 4275
F 0 "U2" H 5500 4175 50  0000 C CNN
F 1 "74LVC08" H 5500 4300 50  0000 C CNN
F 2 "" H 5525 4275 50  0001 C CNN
F 3 "" H 5525 4275 50  0000 C CNN
	3    5525 4275
	-1   0    0    1   
$EndComp
$Comp
L 74LS08 U2
U 4 1 5713C5B9
P 5525 5950
F 0 "U2" H 5475 5825 50  0000 C CNN
F 1 "74LVC08" H 5475 5925 50  0000 C CNN
F 2 "" H 5525 5950 50  0001 C CNN
F 3 "" H 5525 5950 50  0000 C CNN
	4    5525 5950
	-1   0    0    1   
$EndComp
Wire Wire Line
	3050 5350 2900 5350
Wire Wire Line
	2900 5350 2900 5950
Wire Wire Line
	2900 5775 3050 5775
Wire Wire Line
	2900 4425 3050 4425
Wire Wire Line
	2900 4875 3050 4875
Wire Wire Line
	2900 4275 2900 4875
Wire Wire Line
	4800 5350 4925 5350
Wire Wire Line
	4925 5350 4925 5950
Wire Wire Line
	4925 5775 4800 5775
Wire Wire Line
	4925 4275 4925 4875
Wire Wire Line
	4925 4875 4800 4875
Wire Wire Line
	4925 4400 4800 4400
Wire Wire Line
	9000 1475 9000 2075
Wire Wire Line
	9000 2075 8900 2075
Wire Wire Line
	9000 1650 8900 1650
Wire Wire Line
	9000 2550 9000 3150
Wire Wire Line
	9000 2950 8900 2950
Wire Wire Line
	9000 2550 8900 2550
Wire Wire Line
	6875 2550 6875 3150
Wire Wire Line
	6875 2950 7000 2950
Wire Wire Line
	7000 2550 6875 2550
Wire Wire Line
	6875 1450 6875 2075
Wire Wire Line
	6875 2075 7000 2075
Wire Wire Line
	7000 1650 6875 1650
Wire Wire Line
	1350 900  2825 900 
Connection ~ 1425 900 
Wire Wire Line
	1425 1200 2850 1200
Connection ~ 1900 900 
Connection ~ 2275 900 
Connection ~ 2700 900 
Connection ~ 1900 1200
Connection ~ 2275 1200
Connection ~ 2700 1200
Wire Wire Line
	3300 1800 3300 2025
Wire Wire Line
	3325 1900 3300 1900
Connection ~ 3300 1900
Wire Wire Line
	2900 1800 2900 2025
Wire Wire Line
	2900 1900 2700 1900
Connection ~ 2900 1900
Wire Wire Line
	1425 2725 1425 2950
Wire Wire Line
	1400 2850 1425 2850
Connection ~ 1425 2850
Wire Wire Line
	1825 2725 1825 2950
Wire Wire Line
	1825 2850 1950 2850
Connection ~ 1825 2850
Wire Wire Line
	1825 1800 1825 2025
Wire Wire Line
	1825 1900 1950 1900
Connection ~ 1825 1900
Wire Wire Line
	1425 1800 1425 2025
Wire Wire Line
	1425 1900 1400 1900
Connection ~ 1425 1900
Wire Wire Line
	3325 2725 3325 2950
Wire Wire Line
	3350 2850 3325 2850
Connection ~ 3325 2850
Wire Wire Line
	2925 2725 2925 2950
Wire Wire Line
	2925 2850 2700 2850
Connection ~ 2925 2850
Wire Wire Line
	2575 4775 2575 5175
Wire Wire Line
	2575 5175 1375 5175
Wire Wire Line
	1375 5175 1375 5325
Wire Wire Line
	1375 4875 1375 5050
Wire Wire Line
	1375 5050 2675 5050
Wire Wire Line
	2675 4525 2675 5700
Wire Wire Line
	2675 5425 2575 5425
Wire Wire Line
	2675 4525 1700 4525
Wire Wire Line
	1700 4525 1700 4375
Connection ~ 2675 5050
Wire Wire Line
	2675 5700 1700 5700
Wire Wire Line
	1700 5700 1700 5850
Connection ~ 2675 5425
Connection ~ 2900 4425
Connection ~ 2900 5775
Connection ~ 4925 4400
Connection ~ 4925 5775
Wire Wire Line
	5300 4825 5300 5175
Wire Wire Line
	5300 5175 6500 5175
Wire Wire Line
	6500 5175 6500 5300
Wire Wire Line
	6500 4925 6500 5075
Wire Wire Line
	6500 5075 5175 5075
Wire Wire Line
	5175 4525 5175 5700
Wire Wire Line
	5175 5400 5300 5400
Wire Wire Line
	5175 5700 6125 5700
Wire Wire Line
	6125 5700 6125 5850
Connection ~ 5175 5400
Wire Wire Line
	5175 4525 6125 4525
Wire Wire Line
	6125 4525 6125 4375
Connection ~ 5175 5075
Text GLabel 6500 5500 2    60   Input ~ 0
Bridge_Disable
$Comp
L 74LS08 U4
U 1 1 5713F73F
P 6275 1450
F 0 "U4" H 6275 1600 50  0000 C CNN
F 1 "74LVC08" H 6250 1450 50  0000 C CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 6275 1450 50  0001 C CNN
F 3 "" H 6275 1450 50  0000 C CNN
F 4 "74LVC08APW-Q100.11" H 6275 1450 60  0001 C CNN "part"
F 5 "2445097" H 6275 1450 60  0001 C CNN "farnell"
	1    6275 1450
	1    0    0    -1  
$EndComp
$Comp
L 74LS08 U4
U 2 1 5713F923
P 6275 3150
F 0 "U4" H 6225 3275 50  0000 C CNN
F 1 "74LVC08" H 6250 3150 50  0000 C CNN
F 2 "" H 6275 3150 50  0001 C CNN
F 3 "" H 6275 3150 50  0000 C CNN
	2    6275 3150
	1    0    0    -1  
$EndComp
$Comp
L 74LS08 U4
U 3 1 5713F9E5
P 9600 1475
F 0 "U4" H 9600 1350 50  0000 C CNN
F 1 "74LVC08" H 9625 1475 50  0000 C CNN
F 2 "" H 9600 1475 50  0001 C CNN
F 3 "" H 9600 1475 50  0000 C CNN
	3    9600 1475
	-1   0    0    1   
$EndComp
$Comp
L 74LS08 U4
U 4 1 5713FB8D
P 9600 3150
F 0 "U4" H 9575 3025 50  0000 C CNN
F 1 "74LVC08" H 9575 3150 50  0000 C CNN
F 2 "" H 9600 3150 50  0001 C CNN
F 3 "" H 9600 3150 50  0000 C CNN
	4    9600 3150
	-1   0    0    1   
$EndComp
$Comp
L 7400 U3
U 1 1 57141A30
P 6000 2025
F 0 "U3" H 5975 2150 50  0000 C CNN
F 1 "74LVC00" H 5975 2000 50  0000 C CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 6000 2025 50  0001 C CNN
F 3 "" H 6000 2025 50  0000 C CNN
F 4 "SN74LVC00APWR" H 6000 2025 60  0001 C CNN "part"
F 5 "1105931" H 6000 2025 60  0001 C CNN "farnell"
	1    6000 2025
	1    0    0    -1  
$EndComp
$Comp
L 7400 U3
U 2 1 57141B42
P 6000 2600
F 0 "U3" H 5975 2725 50  0000 C CNN
F 1 "74LVC00" H 5975 2550 50  0000 C CNN
F 2 "" H 6000 2600 50  0001 C CNN
F 3 "" H 6000 2600 50  0000 C CNN
F 4 "SN74HC03PW" H 6000 2600 60  0001 C CNN "part"
	2    6000 2600
	1    0    0    -1  
$EndComp
$Comp
L 7400 U3
U 3 1 57141D9A
P 9775 2025
F 0 "U3" H 9750 1900 50  0000 C CNN
F 1 "74LVC00" H 9775 2025 50  0000 C CNN
F 2 "" H 9775 2025 50  0001 C CNN
F 3 "" H 9775 2025 50  0000 C CNN
F 4 "SN74HC03PW" H 9775 2025 60  0001 C CNN "part"
	3    9775 2025
	-1   0    0    1   
$EndComp
$Comp
L 7400 U3
U 4 1 57141F56
P 9775 2600
F 0 "U3" H 9750 2475 50  0000 C CNN
F 1 "74LVC00" H 9775 2625 50  0000 C CNN
F 2 "" H 9775 2600 50  0001 C CNN
F 3 "" H 9775 2600 50  0000 C CNN
F 4 "SN74HC03PW" H 9775 2600 60  0001 C CNN "part"
	4    9775 2600
	-1   0    0    1   
$EndComp
Wire Wire Line
	6600 2025 6600 2350
Wire Wire Line
	6600 2350 5400 2350
Wire Wire Line
	5400 2350 5400 2500
Wire Wire Line
	5400 2125 5400 2275
Wire Wire Line
	5400 2275 6725 2275
Wire Wire Line
	6725 1700 6725 2900
Wire Wire Line
	6725 2600 6600 2600
Wire Wire Line
	6725 1700 5675 1700
Wire Wire Line
	5675 1700 5675 1550
Connection ~ 6725 2275
Wire Wire Line
	6725 2900 5675 2900
Wire Wire Line
	5675 2900 5675 3050
Connection ~ 6725 2600
Connection ~ 6875 2950
Connection ~ 6875 1650
Text GLabel 5400 2700 0    60   Input ~ 0
Bridge_Disable
Wire Wire Line
	9175 2025 9175 2350
Wire Wire Line
	9175 2350 10375 2350
Wire Wire Line
	10375 2350 10375 2500
Wire Wire Line
	10375 2125 10375 2275
Wire Wire Line
	10375 2275 9100 2275
Wire Wire Line
	9100 1750 9100 2900
Wire Wire Line
	9100 2600 9175 2600
Wire Wire Line
	9100 1750 10200 1750
Wire Wire Line
	10200 1750 10200 1575
Connection ~ 9100 2275
Wire Wire Line
	9100 2900 10200 2900
Wire Wire Line
	10200 2900 10200 3050
Connection ~ 9100 2600
Connection ~ 9000 2950
Connection ~ 9000 1650
Text GLabel 10375 2700 2    60   Input ~ 0
Bridge_Disable
Text GLabel 1375 5175 0    60   Input ~ 0
LeftFront_DIAG
Text GLabel 6500 5175 2    60   Input ~ 0
RightFront_DIAG
Text GLabel 5400 2350 0    60   Input ~ 0
LeftRear_DIAG
Text GLabel 10375 2350 2    60   Input ~ 0
RightRear_DIAG
Text GLabel 9250 5075 1    60   Input ~ 0
+3.3V
Wire Wire Line
	8575 5075 9250 5075
Connection ~ 8800 5075
Connection ~ 9025 5075
Text GLabel 8575 5375 3    60   Input ~ 0
~RightFront_CL
Text GLabel 8800 5375 3    60   Input ~ 0
~LeftFront_CL
Text GLabel 9025 5375 3    60   Input ~ 0
~LeftRear_CL
Text GLabel 9250 5375 3    60   Input ~ 0
~RightRear_CL
Text Notes 8350 4700 0    60   ~ 0
Pullups for currentlimit
Wire Notes Line
	8225 4525 9525 4525
Wire Notes Line
	9525 4525 9525 6125
Wire Notes Line
	9525 6125 8225 6125
Wire Notes Line
	8225 6125 8225 4525
$Comp
L R R10
U 1 1 579BFFE0
P 8575 5225
F 0 "R10" V 8655 5225 50  0000 C CNN
F 1 "10k" V 8575 5225 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8505 5225 50  0001 C CNN
F 3 "" H 8575 5225 50  0000 C CNN
F 4 "9332391" V 8575 5225 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" V 8575 5225 60  0001 C CNN "part"
	1    8575 5225
	1    0    0    -1  
$EndComp
$Comp
L R R118
U 1 1 579C0194
P 8800 5225
F 0 "R118" V 8880 5225 50  0000 C CNN
F 1 "10k" V 8800 5225 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8730 5225 50  0001 C CNN
F 3 "" H 8800 5225 50  0000 C CNN
F 4 "9332391" V 8800 5225 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" V 8800 5225 60  0001 C CNN "part"
	1    8800 5225
	1    0    0    -1  
$EndComp
$Comp
L R R119
U 1 1 579C029F
P 9025 5225
F 0 "R119" V 9105 5225 50  0000 C CNN
F 1 "10k" V 9025 5225 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8955 5225 50  0001 C CNN
F 3 "" H 9025 5225 50  0000 C CNN
F 4 "9332391" V 9025 5225 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" V 9025 5225 60  0001 C CNN "part"
	1    9025 5225
	1    0    0    -1  
$EndComp
$Comp
L R R120
U 1 1 579C0368
P 9250 5225
F 0 "R120" V 9330 5225 50  0000 C CNN
F 1 "10k" V 9250 5225 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9180 5225 50  0001 C CNN
F 3 "" H 9250 5225 50  0000 C CNN
F 4 "9332391" V 9250 5225 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" V 9250 5225 60  0001 C CNN "part"
	1    9250 5225
	1    0    0    -1  
$EndComp
$EndSCHEMATC
