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
Text GLabel 5200 2900 0    60   Input ~ 0
LR_wheelsensor_in+
Text GLabel 5175 3500 0    60   Input ~ 0
LR_wheelsensor_in-
Text GLabel 2150 2450 0    60   Input ~ 0
RF_wheelsensor_in+
Text GLabel 2150 3025 0    60   Input ~ 0
RF_wheelsensor_in-
Text GLabel 5200 3100 0    60   Input ~ 0
RR_wheelsensor_in+
Text GLabel 5175 3725 0    60   Input ~ 0
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
L NCV1124 U34
U 1 1 57900E18
P 6675 3000
F 0 "U34" H 6925 3500 60  0000 C CNN
F 1 "NCV1124" H 6575 3500 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6825 4100 60  0001 C CNN
F 3 "" H 6825 4100 60  0000 C CNN
F 4 "NCV1124DR2G" H 6675 3000 60  0001 C CNN "part"
F 5 "2534601" H 6675 3000 60  0001 C CNN "farnell"
	1    6675 3000
	1    0    0    -1  
$EndComp
$Comp
L C C95
U 1 1 57900EFD
P 6725 3675
F 0 "C95" H 6750 3775 50  0000 L CNN
F 1 "100n" H 6750 3575 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6763 3525 50  0001 C CNN
F 3 "" H 6725 3675 50  0000 C CNN
F 4 "0805B104J500CT" H 6725 3675 60  0001 C CNN "part"
F 5 "2496940" H 6725 3675 60  0001 C CNN "farnell"
	1    6725 3675
	1    0    0    -1  
$EndComp
$Comp
L C C97
U 1 1 57901042
P 6975 3675
F 0 "C97" H 7000 3775 50  0000 L CNN
F 1 "1n" H 7000 3575 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7013 3525 50  0001 C CNN
F 3 "" H 6975 3675 50  0000 C CNN
F 4 "GRM2165C1H102JA01D" H 6975 3675 60  0001 C CNN "part"
F 5 "2494247" H 6975 3675 60  0001 C CNN "farnell"
	1    6975 3675
	1    0    0    -1  
$EndComp
$Comp
L CP C93
U 1 1 579010A2
P 6475 3675
F 0 "C93" H 6500 3775 50  0000 L CNN
F 1 "10u" H 6500 3575 50  0000 L CNN
F 2 "myfootprints:CP_0805" H 6513 3525 50  0001 C CNN
F 3 "" H 6475 3675 50  0000 C CNN
F 4 "2491430" H 6475 3675 60  0001 C CNN "farnell"
F 5 "TMCP0J106KTRF" H 6475 3675 60  0001 C CNN "part"
	1    6475 3675
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0102
U 1 1 579010CB
P 6975 3825
F 0 "#PWR0102" H 6975 3575 50  0001 C CNN
F 1 "GND" H 6975 3675 50  0001 C CNN
F 2 "" H 6975 3825 50  0000 C CNN
F 3 "" H 6975 3825 50  0000 C CNN
	1    6975 3825
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0103
U 1 1 57901107
P 6225 3300
F 0 "#PWR0103" H 6225 3050 50  0001 C CNN
F 1 "GND" H 6225 3150 50  0000 C CNN
F 2 "" H 6225 3300 50  0000 C CNN
F 3 "" H 6225 3300 50  0000 C CNN
	1    6225 3300
	1    0    0    -1  
$EndComp
Text GLabel 7175 3525 2    60   Input ~ 0
+5V
Text GLabel 7175 2700 2    60   Input ~ 0
+5V
$Comp
L R R116
U 1 1 5790171F
P 6075 2700
F 0 "R116" V 6000 2700 50  0000 C CNN
F 1 "16.9k" V 6075 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6005 2700 50  0001 C CNN
F 3 "" H 6075 2700 50  0001 C CNN
F 4 "MC01W0805116K9" V 6075 2700 60  0001 C CNN "part"
F 5 "2141858" V 6075 2700 60  0001 C CNN "farnell"
	1    6075 2700
	0    1    1    0   
$EndComp
$Comp
L GND #PWR0104
U 1 1 579017A6
P 5925 2700
F 0 "#PWR0104" H 5925 2450 50  0001 C CNN
F 1 "GND" H 5925 2550 50  0001 C CNN
F 2 "" H 5925 2700 50  0000 C CNN
F 3 "" H 5925 2700 50  0000 C CNN
	1    5925 2700
	1    0    0    -1  
$EndComp
$Comp
L R R113
U 1 1 57901906
P 5350 2900
F 0 "R113" V 5430 2900 50  0000 C CNN
F 1 "15k" V 5350 2900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5280 2900 50  0001 C CNN
F 3 "" H 5350 2900 50  0000 C CNN
F 4 "1750772" V 5350 2900 60  0001 C CNN "farnell"
F 5 "ERJP06F1502V" V 5350 2900 60  0001 C CNN "part"
	1    5350 2900
	0    1    1    0   
$EndComp
$Comp
L R R114
U 1 1 57901951
P 5350 3100
F 0 "R114" V 5430 3100 50  0000 C CNN
F 1 "15k" V 5350 3100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5280 3100 50  0001 C CNN
F 3 "" H 5350 3100 50  0000 C CNN
F 4 "1750772" V 5350 3100 60  0001 C CNN "farnell"
F 5 "ERJP06F1502V" V 5350 3100 60  0001 C CNN "part"
	1    5350 3100
	0    1    1    0   
$EndComp
$Comp
L C C84
U 1 1 5790199C
P 5600 3350
F 0 "C84" H 5625 3450 50  0000 L CNN
F 1 "47n" H 5625 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5638 3200 50  0001 C CNN
F 3 "" H 5600 3350 50  0000 C CNN
F 4 "0805B473J500CT" H 5600 3350 60  0001 C CNN "part"
F 5 "2496981" H 5600 3350 60  0001 C CNN "farnell"
	1    5600 3350
	1    0    0    -1  
$EndComp
$Comp
L C C86
U 1 1 57901A21
P 5875 3350
F 0 "C86" H 5900 3450 50  0000 L CNN
F 1 "47n" H 5900 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5913 3200 50  0001 C CNN
F 3 "" H 5875 3350 50  0000 C CNN
F 4 "0805B473J500CT" H 5875 3350 60  0001 C CNN "part"
F 5 "2496981" H 5875 3350 60  0001 C CNN "farnell"
	1    5875 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0105
U 1 1 57901C47
P 5875 3500
F 0 "#PWR0105" H 5875 3250 50  0001 C CNN
F 1 "GND" H 5875 3350 50  0000 C CNN
F 2 "" H 5875 3500 50  0000 C CNN
F 3 "" H 5875 3500 50  0000 C CNN
	1    5875 3500
	1    0    0    -1  
$EndComp
Text Notes 5000 2775 0    60   ~ 0
1/2W resistors
Text Label 7175 3300 0    60   ~ 0
VR_diag
$Comp
L R R117
U 1 1 57902558
P 9750 4825
F 0 "R117" V 9830 4825 50  0000 C CNN
F 1 "10k" V 9750 4825 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9680 4825 50  0001 C CNN
F 3 "" H 9750 4825 50  0000 C CNN
F 4 "9332391" V 9750 4825 60  0001 C CNN "farnell"
F 5 "MC01W0805110K" V 9750 4825 60  0001 C CNN "part"
	1    9750 4825
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0106
U 1 1 57902641
P 10075 5525
F 0 "#PWR0106" H 10075 5275 50  0001 C CNN
F 1 "GND" H 10075 5375 50  0000 C CNN
F 2 "" H 10075 5525 50  0000 C CNN
F 3 "" H 10075 5525 50  0000 C CNN
	1    10075 5525
	1    0    0    -1  
$EndComp
Text Label 10325 4675 0    60   ~ 0
VR_diag
Text GLabel 9650 4675 0    60   Input ~ 0
+5V
Text GLabel 9775 5375 0    60   Input ~ 0
VR_diag_test
Text GLabel 9925 3450 2    60   Input ~ 0
LR_speed_in
Text GLabel 9925 2975 2    60   Input ~ 0
RR_speed_in
$Comp
L NCV1124 U33
U 1 1 579039B4
P 6675 1375
F 0 "U33" H 6925 1875 60  0000 C CNN
F 1 "NCV1124" H 6575 1875 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6825 2475 60  0001 C CNN
F 3 "" H 6825 2475 60  0000 C CNN
F 4 "NCV1124DR2G" H 6675 1375 60  0001 C CNN "part"
F 5 "2534601" H 6675 1375 60  0001 C CNN "farnell"
	1    6675 1375
	1    0    0    -1  
$EndComp
$Comp
L C C94
U 1 1 579039BA
P 6675 2025
F 0 "C94" H 6700 2125 50  0000 L CNN
F 1 "100n" H 6700 1925 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6713 1875 50  0001 C CNN
F 3 "" H 6675 2025 50  0000 C CNN
F 4 "0805B104J500CT" H 6675 2025 60  0001 C CNN "part"
F 5 "2496940" H 6675 2025 60  0001 C CNN "farnell"
	1    6675 2025
	1    0    0    -1  
$EndComp
$Comp
L C C96
U 1 1 579039C0
P 6925 2025
F 0 "C96" H 6950 2125 50  0000 L CNN
F 1 "1n" H 6950 1925 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6963 1875 50  0001 C CNN
F 3 "" H 6925 2025 50  0000 C CNN
F 4 "GRM2165C1H102JA01D" H 6925 2025 60  0001 C CNN "part"
F 5 "2494247" H 6925 2025 60  0001 C CNN "farnell"
	1    6925 2025
	1    0    0    -1  
$EndComp
$Comp
L CP C92
U 1 1 579039C6
P 6425 2025
F 0 "C92" H 6450 2125 50  0000 L CNN
F 1 "10u" H 6450 1925 50  0000 L CNN
F 2 "myfootprints:CP_0805" H 6463 1875 50  0001 C CNN
F 3 "" H 6425 2025 50  0000 C CNN
F 4 "2491430" H 6425 2025 60  0001 C CNN "farnell"
F 5 "TMCP0J106KTRF" H 6425 2025 60  0001 C CNN "part"
	1    6425 2025
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0107
U 1 1 579039CC
P 6925 2175
F 0 "#PWR0107" H 6925 1925 50  0001 C CNN
F 1 "GND" H 6925 2025 50  0001 C CNN
F 2 "" H 6925 2175 50  0000 C CNN
F 3 "" H 6925 2175 50  0000 C CNN
	1    6925 2175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0108
U 1 1 579039D2
P 6225 1675
F 0 "#PWR0108" H 6225 1425 50  0001 C CNN
F 1 "GND" H 6225 1525 50  0000 C CNN
F 2 "" H 6225 1675 50  0000 C CNN
F 3 "" H 6225 1675 50  0000 C CNN
	1    6225 1675
	1    0    0    -1  
$EndComp
Text GLabel 7125 1875 2    60   Input ~ 0
+5V
Text GLabel 7175 1075 2    60   Input ~ 0
+5V
$Comp
L R R115
U 1 1 57903A03
P 6075 1075
F 0 "R115" V 6000 1075 50  0000 C CNN
F 1 "16.9k" V 6075 1075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6005 1075 50  0001 C CNN
F 3 "" H 6075 1075 50  0000 C CNN
F 4 "MC01W0805116K9" V 6075 1075 60  0001 C CNN "part"
F 5 "2141858" V 6075 1075 60  0001 C CNN "farnell"
	1    6075 1075
	0    1    1    0   
$EndComp
$Comp
L GND #PWR0109
U 1 1 57903A09
P 5925 1075
F 0 "#PWR0109" H 5925 825 50  0001 C CNN
F 1 "GND" H 5925 925 50  0001 C CNN
F 2 "" H 5925 1075 50  0000 C CNN
F 3 "" H 5925 1075 50  0000 C CNN
	1    5925 1075
	1    0    0    -1  
$EndComp
$Comp
L R R111
U 1 1 57903A0F
P 5350 1275
F 0 "R111" V 5430 1275 50  0000 C CNN
F 1 "15k" V 5350 1275 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5280 1275 50  0001 C CNN
F 3 "" H 5350 1275 50  0000 C CNN
F 4 "ERJP06F1502V" V 5350 1275 60  0001 C CNN "part"
F 5 "1750772" V 5350 1275 60  0001 C CNN "farnell"
	1    5350 1275
	0    1    1    0   
$EndComp
$Comp
L R R112
U 1 1 57903A15
P 5350 1475
F 0 "R112" V 5430 1475 50  0000 C CNN
F 1 "15k" V 5350 1475 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5280 1475 50  0001 C CNN
F 3 "" H 5350 1475 50  0000 C CNN
F 4 "1750772" V 5350 1475 60  0001 C CNN "farnell"
F 5 "ERJP06F1502V" V 5350 1475 60  0001 C CNN "part"
	1    5350 1475
	0    1    1    0   
$EndComp
$Comp
L C C83
U 1 1 57903A1B
P 5600 1725
F 0 "C83" H 5625 1825 50  0000 L CNN
F 1 "47n" H 5625 1625 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5638 1575 50  0001 C CNN
F 3 "" H 5600 1725 50  0000 C CNN
F 4 "0805B473J500CT" H 5600 1725 60  0001 C CNN "part"
F 5 "2496981" H 5600 1725 60  0001 C CNN "farnell"
	1    5600 1725
	1    0    0    -1  
$EndComp
$Comp
L C C85
U 1 1 57903A21
P 5875 1725
F 0 "C85" H 5900 1825 50  0000 L CNN
F 1 "47n" H 5900 1625 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5913 1575 50  0001 C CNN
F 3 "" H 5875 1725 50  0000 C CNN
F 4 "0805B473J500CT" H 5875 1725 60  0001 C CNN "part"
F 5 "2496981" H 5875 1725 60  0001 C CNN "farnell"
	1    5875 1725
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0110
U 1 1 57903A27
P 5875 1875
F 0 "#PWR0110" H 5875 1625 50  0001 C CNN
F 1 "GND" H 5875 1725 50  0000 C CNN
F 2 "" H 5875 1875 50  0000 C CNN
F 3 "" H 5875 1875 50  0000 C CNN
	1    5875 1875
	1    0    0    -1  
$EndComp
Text Notes 5000 1150 0    60   ~ 0
1/2W resistors
Text Label 7175 1675 0    60   ~ 0
VR_diag
Text GLabel 9925 2500 2    60   Input ~ 0
LF_speed_in
Text GLabel 9925 2025 2    60   Input ~ 0
RF_speed_in
$Comp
L Jumper_NC_Small JP4
U 1 1 57903C51
P 2425 1225
F 0 "JP4" H 2425 1305 50  0000 C CNN
F 1 "L_passth+" H 2435 1165 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 1225 50  0001 C CNN
F 3 "" H 2425 1225 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 1225 60  0001 C CNN "part"
F 5 "9240276" H 2425 1225 60  0001 C CNN "farnell"
	1    2425 1225
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP5
U 1 1 57903D98
P 2425 1400
F 0 "JP5" H 2425 1480 50  0000 C CNN
F 1 "LF+_to_in" H 2435 1340 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 1400 50  0001 C CNN
F 3 "" H 2425 1400 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 1400 60  0001 C CNN "part"
F 5 "9240276" H 2425 1400 60  0001 C CNN "farnell"
	1    2425 1400
	1    0    0    -1  
$EndComp
Text Label 2675 1400 0    60   ~ 0
LF_wheelsensor+
$Comp
L Jumper_NO_Small JP3
U 1 1 57904332
P 2425 1050
F 0 "JP3" H 2425 1130 50  0000 C CNN
F 1 "soft_L_out+" H 2435 990 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 1050 50  0001 C CNN
F 3 "" H 2425 1050 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 1050 60  0001 C CNN "part"
F 5 "9240276" H 2425 1050 60  0001 C CNN "farnell"
	1    2425 1050
	1    0    0    -1  
$EndComp
Text Label 2650 1925 0    60   ~ 0
LF_wheelsensor-
$Comp
L Jumper_NO_Small JP6
U 1 1 579049E5
P 2425 1600
F 0 "JP6" H 2425 1680 50  0000 C CNN
F 1 "soft_L_out-" H 2435 1540 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 1600 50  0001 C CNN
F 3 "" H 2425 1600 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 1600 60  0001 C CNN "part"
F 5 "9240276" H 2425 1600 60  0001 C CNN "farnell"
	1    2425 1600
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP8
U 1 1 57904A62
P 2425 1925
F 0 "JP8" H 2425 2005 50  0000 C CNN
F 1 "LF-_to_in" H 2435 1865 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 1925 50  0001 C CNN
F 3 "" H 2425 1925 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 1925 60  0001 C CNN "part"
F 5 "9240276" H 2425 1925 60  0001 C CNN "farnell"
	1    2425 1925
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NC_Small JP7
U 1 1 57904AC7
P 2425 1750
F 0 "JP7" H 2425 1830 50  0000 C CNN
F 1 "L_passth-" H 2435 1690 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 1750 50  0001 C CNN
F 3 "" H 2425 1750 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 1750 60  0001 C CNN "part"
F 5 "9240276" H 2425 1750 60  0001 C CNN "farnell"
	1    2425 1750
	1    0    0    -1  
$EndComp
Text Label 2675 2625 0    60   ~ 0
RF_wheelsensor+
$Comp
L Jumper_NO_Small JP9
U 1 1 579051C2
P 2425 2275
F 0 "JP9" H 2425 2355 50  0000 C CNN
F 1 "soft_R_out+" H 2435 2215 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 2275 50  0001 C CNN
F 3 "" H 2425 2275 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 2275 60  0001 C CNN "part"
F 5 "9240276" H 2425 2275 60  0001 C CNN "farnell"
	1    2425 2275
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP11
U 1 1 57905290
P 2425 2625
F 0 "JP11" H 2425 2705 50  0000 C CNN
F 1 "RF+_to_in" H 2435 2565 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 2625 50  0001 C CNN
F 3 "" H 2425 2625 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 2625 60  0001 C CNN "part"
F 5 "9240276" H 2425 2625 60  0001 C CNN "farnell"
	1    2425 2625
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NC_Small JP10
U 1 1 579052F9
P 2425 2450
F 0 "JP10" H 2425 2530 50  0000 C CNN
F 1 "RF_passth+" H 2435 2390 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 2450 50  0001 C CNN
F 3 "" H 2425 2450 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 2450 60  0001 C CNN "part"
F 5 "9240276" H 2425 2450 60  0001 C CNN "farnell"
	1    2425 2450
	1    0    0    -1  
$EndComp
Text Label 2675 3200 0    60   ~ 0
RF_wheelsensor-
$Comp
L Jumper_NC_Small JP13
U 1 1 57905A59
P 2425 3025
F 0 "JP13" H 2425 3105 50  0000 C CNN
F 1 "RF_passth-" H 2435 2965 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 3025 50  0001 C CNN
F 3 "" H 2425 3025 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 3025 60  0001 C CNN "part"
F 5 "9240276" H 2425 3025 60  0001 C CNN "farnell"
	1    2425 3025
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP12
U 1 1 57905B4F
P 2425 2850
F 0 "JP12" H 2425 2930 50  0000 C CNN
F 1 "soft_R_out-" H 2435 2790 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 2850 50  0001 C CNN
F 3 "" H 2425 2850 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 2850 60  0001 C CNN "part"
F 5 "9240276" H 2425 2850 60  0001 C CNN "farnell"
	1    2425 2850
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP14
U 1 1 57905C2E
P 2425 3200
F 0 "JP14" H 2425 3280 50  0000 C CNN
F 1 "RF-_to_in" H 2435 3140 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2425 3200 50  0001 C CNN
F 3 "" H 2425 3200 50  0000 C CNN
F 4 "RC1206JR-070RL" H 2425 3200 60  0001 C CNN "part"
F 5 "9240276" H 2425 3200 60  0001 C CNN "farnell"
	1    2425 3200
	1    0    0    -1  
$EndComp
Text Label 5200 1275 2    60   ~ 0
LF_wheelsensor+
Text Label 5200 1475 2    60   ~ 0
RF_wheelsensor+
Text Label 5175 1875 2    60   ~ 0
LF_wheelsensor-
Text Label 5175 2100 2    60   ~ 0
RF_wheelsensor-
Text Label 2850 6725 2    60   ~ 0
L_sensor_interpolated_out-
Text Label 2850 6575 2    60   ~ 0
L_sensor_interpolated_out+
$Comp
L MAX232 U31
U 1 1 579078A3
P 2325 5650
F 0 "U31" H 1875 6500 50  0000 L CNN
F 1 "MAX232" H 2525 6500 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-16_4.4x5mm_Pitch0.65mm" H 2325 5650 50  0001 C CNN
F 3 "" H 2325 5650 50  0000 C CNN
F 4 "2335478" H 2325 5650 60  0001 C CNN "farnell"
F 5 "MAX202IPW" H 2325 5650 60  0001 C CNN "part"
	1    2325 5650
	1    0    0    -1  
$EndComp
$Comp
L C C79
U 1 1 57907954
P 3225 5100
F 0 "C79" H 3250 5200 50  0000 L CNN
F 1 "1u" H 3250 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3263 4950 50  0001 C CNN
F 3 "" H 3225 5100 50  0000 C CNN
F 4 "2362126" H 3225 5100 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 3225 5100 60  0001 C CNN "part"
	1    3225 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0111
U 1 1 57907ABF
P 3075 5550
F 0 "#PWR0111" H 3075 5300 50  0001 C CNN
F 1 "GND" H 3075 5400 50  0001 C CNN
F 2 "" H 3075 5550 50  0000 C CNN
F 3 "" H 3075 5550 50  0000 C CNN
	1    3075 5550
	1    0    0    -1  
$EndComp
$Comp
L C C76
U 1 1 57907B2D
P 3075 5350
F 0 "C76" H 3100 5450 50  0000 L CNN
F 1 "1u" H 3100 5250 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3113 5200 50  0001 C CNN
F 3 "" H 3075 5350 50  0000 C CNN
F 4 "2362126" H 3075 5350 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 3075 5350 60  0001 C CNN "part"
	1    3075 5350
	0    1    1    0   
$EndComp
$Comp
L C C77
U 1 1 57907BDA
P 3075 5850
F 0 "C77" H 3100 5950 50  0000 L CNN
F 1 "1u" H 3100 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3113 5700 50  0001 C CNN
F 3 "" H 3075 5850 50  0000 C CNN
F 4 "2362126" H 3075 5850 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 3075 5850 60  0001 C CNN "part"
	1    3075 5850
	0    1    1    0   
$EndComp
$Comp
L C C74
U 1 1 57908438
P 1575 5150
F 0 "C74" H 1600 5250 50  0000 L CNN
F 1 "1u" H 1600 5050 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 1613 5000 50  0001 C CNN
F 3 "" H 1575 5150 50  0000 C CNN
F 4 "2362126" H 1575 5150 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 1575 5150 60  0001 C CNN "part"
	1    1575 5150
	1    0    0    -1  
$EndComp
$Comp
L C C75
U 1 1 579084C7
P 1575 5650
F 0 "C75" H 1600 5750 50  0000 L CNN
F 1 "1u" H 1600 5550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 1613 5500 50  0001 C CNN
F 3 "" H 1575 5650 50  0000 C CNN
F 4 "2362126" H 1575 5650 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 1575 5650 60  0001 C CNN "part"
	1    1575 5650
	1    0    0    -1  
$EndComp
Text GLabel 3225 4950 1    60   Input ~ 0
+5V
$Comp
L C C78
U 1 1 57909216
P 3125 6350
F 0 "C78" H 3150 6450 50  0000 L CNN
F 1 "1u" H 3150 6250 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3163 6200 50  0001 C CNN
F 3 "" H 3125 6350 50  0000 C CNN
F 4 "2362126" H 3125 6350 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 3125 6350 60  0001 C CNN "part"
	1    3125 6350
	1    0    0    -1  
$EndComp
$Comp
L C C80
U 1 1 579093B9
P 3350 6350
F 0 "C80" H 3375 6450 50  0000 L CNN
F 1 "1u" H 3375 6250 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 3388 6200 50  0001 C CNN
F 3 "" H 3350 6350 50  0000 C CNN
F 4 "2362126" H 3350 6350 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 3350 6350 60  0001 C CNN "part"
	1    3350 6350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0112
U 1 1 5790981F
P 3125 7025
F 0 "#PWR0112" H 3125 6775 50  0001 C CNN
F 1 "GND" H 3125 6875 50  0000 C CNN
F 2 "" H 3125 7025 50  0000 C CNN
F 3 "" H 3125 7025 50  0000 C CNN
	1    3125 7025
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0113
U 1 1 5790996D
P 2925 6350
F 0 "#PWR0113" H 2925 6100 50  0001 C CNN
F 1 "GND" H 2925 6200 50  0001 C CNN
F 2 "" H 2925 6350 50  0000 C CNN
F 3 "" H 2925 6350 50  0000 C CNN
	1    2925 6350
	1    0    0    -1  
$EndComp
NoConn ~ 1725 6350
NoConn ~ 1725 6250
Text GLabel 1725 6050 0    60   Input ~ 0
L_speed_out+
Text Label 5900 6725 2    60   ~ 0
R_sensor_interpolated_out-
Text Label 5900 6575 2    60   ~ 0
R_sensor_interpolated_out+
$Comp
L MAX232 U32
U 1 1 5790C72B
P 5375 5650
F 0 "U32" H 4925 6500 50  0000 L CNN
F 1 "MAX232" H 5575 6500 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-16_4.4x5mm_Pitch0.65mm" H 5375 5650 50  0001 C CNN
F 3 "" H 5375 5650 50  0000 C CNN
F 4 "MAX202IPW" H 5375 5650 60  0001 C CNN "part"
F 5 "2335478" H 5375 5650 60  0001 C CNN "farnell"
	1    5375 5650
	1    0    0    -1  
$EndComp
$Comp
L C C90
U 1 1 5790C731
P 6275 5100
F 0 "C90" H 6300 5200 50  0000 L CNN
F 1 "1u" H 6300 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 6313 4950 50  0001 C CNN
F 3 "" H 6275 5100 50  0000 C CNN
F 4 "2362126" H 6275 5100 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 6275 5100 60  0001 C CNN "part"
	1    6275 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0114
U 1 1 5790C737
P 6125 5550
F 0 "#PWR0114" H 6125 5300 50  0001 C CNN
F 1 "GND" H 6125 5400 50  0001 C CNN
F 2 "" H 6125 5550 50  0000 C CNN
F 3 "" H 6125 5550 50  0000 C CNN
	1    6125 5550
	1    0    0    -1  
$EndComp
$Comp
L C C87
U 1 1 5790C73D
P 6125 5350
F 0 "C87" H 6150 5450 50  0000 L CNN
F 1 "1u" H 6150 5250 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 6163 5200 50  0001 C CNN
F 3 "" H 6125 5350 50  0000 C CNN
F 4 "2362126" H 6125 5350 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 6125 5350 60  0001 C CNN "part"
	1    6125 5350
	0    1    1    0   
$EndComp
$Comp
L C C88
U 1 1 5790C743
P 6125 5850
F 0 "C88" H 6150 5950 50  0000 L CNN
F 1 "1u" H 6150 5750 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 6163 5700 50  0001 C CNN
F 3 "" H 6125 5850 50  0000 C CNN
F 4 "2362126" H 6125 5850 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 6125 5850 60  0001 C CNN "part"
	1    6125 5850
	0    1    1    0   
$EndComp
$Comp
L C C81
U 1 1 5790C74F
P 4625 5150
F 0 "C81" H 4650 5250 50  0000 L CNN
F 1 "1u" H 4650 5050 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 4663 5000 50  0001 C CNN
F 3 "" H 4625 5150 50  0000 C CNN
F 4 "2362126" H 4625 5150 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 4625 5150 60  0001 C CNN "part"
	1    4625 5150
	1    0    0    -1  
$EndComp
$Comp
L C C82
U 1 1 5790C755
P 4625 5650
F 0 "C82" H 4650 5750 50  0000 L CNN
F 1 "1u" H 4650 5550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 4663 5500 50  0001 C CNN
F 3 "" H 4625 5650 50  0000 C CNN
F 4 "2362126" H 4625 5650 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 4625 5650 60  0001 C CNN "part"
	1    4625 5650
	1    0    0    -1  
$EndComp
Text GLabel 6275 4950 1    60   Input ~ 0
+5V
$Comp
L C C89
U 1 1 5790C764
P 6175 6300
F 0 "C89" H 6200 6400 50  0000 L CNN
F 1 "1u" H 6200 6200 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 6213 6150 50  0001 C CNN
F 3 "" H 6175 6300 50  0000 C CNN
F 4 "2362126" H 6175 6300 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 6175 6300 60  0001 C CNN "part"
	1    6175 6300
	1    0    0    -1  
$EndComp
$Comp
L C C91
U 1 1 5790C76A
P 6400 6300
F 0 "C91" H 6425 6400 50  0000 L CNN
F 1 "1u" H 6425 6200 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 6438 6150 50  0001 C CNN
F 3 "" H 6400 6300 50  0000 C CNN
F 4 "2362126" H 6400 6300 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 6400 6300 60  0001 C CNN "part"
	1    6400 6300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0115
U 1 1 5790C770
P 6175 7025
F 0 "#PWR0115" H 6175 6775 50  0001 C CNN
F 1 "GND" H 6175 6875 50  0000 C CNN
F 2 "" H 6175 7025 50  0000 C CNN
F 3 "" H 6175 7025 50  0000 C CNN
	1    6175 7025
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0116
U 1 1 5790C776
P 5975 6350
F 0 "#PWR0116" H 5975 6100 50  0001 C CNN
F 1 "GND" H 5975 6200 50  0001 C CNN
F 2 "" H 5975 6350 50  0000 C CNN
F 3 "" H 5975 6350 50  0000 C CNN
	1    5975 6350
	1    0    0    -1  
$EndComp
NoConn ~ 4775 6350
NoConn ~ 4775 6250
Text Notes 825  4450 0    60   ~ 0
output drivers, interfaces with car ABS rear wheel sensor circuit\noutputs a AC squarewave (+ - 16V pk to pk)
Text Notes 4175 825  0    60   ~ 0
Converts VR sensor analog to digital
Text Notes 8200 4450 0    60   ~ 0
switch to diag mode (check wheelsensors for continuity)\nif output toggles sensor is broken
Text GLabel 4775 6150 0    60   Input ~ 0
R_speed_out+
Text GLabel 1725 6150 0    60   Input ~ 0
L_speed_out-
Text GLabel 4775 6050 0    60   Input ~ 0
R_speed_out-
$Comp
L NX3008CBKS Q33
U 1 1 5793388A
P 9975 5325
F 0 "Q33" H 10225 5400 50  0000 L CNN
F 1 "NX3008CBKS" H 10225 5325 50  0000 L CNN
F 2 "SOT363:SOT-363" H 10225 5250 50  0001 L CNN
F 3 "" H 9975 5325 50  0000 L CNN
F 4 "2069539" H 9975 5325 60  0001 C CNN "farnell"
F 5 "NX3008CBKS" H 9975 5325 60  0001 C CNN "part"
	1    9975 5325
	1    0    0    -1  
$EndComp
$Comp
L NX3008CBKS Q33
U 2 1 57933C7E
P 10125 4775
F 0 "Q33" V 10375 4650 50  0000 L CNN
F 1 "NX3008CBKS" V 10025 4850 50  0000 L CNN
F 2 "SOT363:SOT-363" V 10100 4900 50  0001 L CNN
F 3 "" H 10125 4775 50  0000 L CNN
F 4 "NX3008CBKS" V 10125 4775 60  0001 C CNN "part"
F 5 "2069539" V 10125 4775 60  0001 C CNN "farnell"
	2    10125 4775
	0    1    -1   0   
$EndComp
$Comp
L 74LS08 U35
U 1 1 57954584
P 9325 2025
F 0 "U35" H 9325 2075 50  0000 C CNN
F 1 "74LVC08" H 9325 1975 50  0000 C CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 9325 2025 50  0001 C CNN
F 3 "" H 9325 2025 50  0000 C CNN
F 4 "74LVC08APW-Q100.11" H 9325 2025 60  0001 C CNN "part"
F 5 "2445097" H 9325 2025 60  0001 C CNN "farnell"
	1    9325 2025
	1    0    0    -1  
$EndComp
$Comp
L 74LS08 U35
U 2 1 5795463F
P 9325 2500
F 0 "U35" H 9325 2550 50  0000 C CNN
F 1 "74LVC08" H 9325 2450 50  0000 C CNN
F 2 "" H 9325 2500 50  0000 C CNN
F 3 "" H 9325 2500 50  0000 C CNN
	2    9325 2500
	1    0    0    -1  
$EndComp
Text Label 7175 1275 0    60   ~ 0
LF_5to3.3
Text Label 7175 1475 0    60   ~ 0
RF_5to3.3
Text Label 7175 2900 0    60   ~ 0
LR_5to3.3
Text Label 7175 3100 0    60   ~ 0
RR_5to3.3
Text Label 8725 2400 2    60   ~ 0
LF_5to3.3
Text Label 8725 1925 2    60   ~ 0
RF_5to3.3
$Comp
L 74LS08 U35
U 3 1 5795A8AF
P 9325 2975
F 0 "U35" H 9325 3025 50  0000 C CNN
F 1 "74LVC08" H 9325 2925 50  0000 C CNN
F 2 "" H 9325 2975 50  0000 C CNN
F 3 "" H 9325 2975 50  0000 C CNN
	3    9325 2975
	1    0    0    -1  
$EndComp
$Comp
L 74LS08 U35
U 4 1 5795AA16
P 9325 3450
F 0 "U35" H 9325 3500 50  0000 C CNN
F 1 "74LVC08" H 9325 3400 50  0000 C CNN
F 2 "" H 9325 3450 50  0000 C CNN
F 3 "" H 9325 3450 50  0000 C CNN
	4    9325 3450
	1    0    0    -1  
$EndComp
Text Label 8725 3350 2    60   ~ 0
LR_5to3.3
Text Label 8725 2875 2    60   ~ 0
RR_5to3.3
Text GLabel 10250 1150 2    60   Input ~ 0
+3.3V
$Comp
L GND #PWR0117
U 1 1 5795B22F
P 10100 1450
F 0 "#PWR0117" H 10100 1200 50  0001 C CNN
F 1 "GND" H 10100 1300 50  0001 C CNN
F 2 "" H 10100 1450 50  0000 C CNN
F 3 "" H 10100 1450 50  0000 C CNN
	1    10100 1450
	1    0    0    -1  
$EndComp
Text Label 9725 1450 2    60   ~ 0
VSS
Text Label 9700 1150 2    60   ~ 0
VDD
Text Notes 8225 1000 0    60   ~ 0
Levelshift 5 -> 3.3V\nTakes less boardspace than resistor dividers\nAlso NCV1124 dont handle out current very well
$Comp
L C C98
U 1 1 5795CA1A
P 9825 1300
F 0 "C98" H 9850 1400 50  0000 L CNN
F 1 "100n" H 9850 1200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 9863 1150 50  0001 C CNN
F 3 "" H 9825 1300 50  0000 C CNN
F 4 "0805B104J500CT" H 9825 1300 60  0001 C CNN "part"
F 5 "2496940" H 9825 1300 60  0001 C CNN "farnell"
	1    9825 1300
	1    0    0    -1  
$EndComp
$Comp
L C C99
U 1 1 5795CD42
P 10100 1300
F 0 "C99" H 10125 1400 50  0000 L CNN
F 1 "1n" H 10125 1200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 10138 1150 50  0001 C CNN
F 3 "" H 10100 1300 50  0000 C CNN
F 4 "GRM2165C1H102JA01D" H 10100 1300 60  0001 C CNN "part"
F 5 "2494247" H 10100 1300 60  0001 C CNN "farnell"
	1    10100 1300
	1    0    0    -1  
$EndComp
$Comp
L Rnet4 R110
U 3 1 579608F6
P 3125 6875
F 0 "R110" H 3125 6955 50  0000 C CNN
F 1 "1k" H 3125 6875 50  0000 C CNN
F 2 "Resistors_SMD:R_Array_Concave_4x0603" H 3125 6805 50  0001 C CNN
F 3 "" V 3125 6875 50  0000 C CNN
F 4 "EXB38V102JV" H 3125 6875 60  0001 C CNN "part"
F 5 "2060100" H 3125 6875 60  0001 C CNN "farnell"
	3    3125 6875
	0    -1   -1   0   
$EndComp
$Comp
L Rnet4 R110
U 4 1 5796124E
P 3350 6875
F 0 "R110" H 3350 6955 50  0000 C CNN
F 1 "1k" H 3350 6875 50  0000 C CNN
F 2 "" H 3350 6805 50  0000 C CNN
F 3 "" V 3350 6875 50  0000 C CNN
	4    3350 6875
	0    -1   -1   0   
$EndComp
$Comp
L Rnet4 R110
U 1 1 57961701
P 6175 6875
F 0 "R110" H 6175 6955 50  0000 C CNN
F 1 "1k" H 6175 6875 50  0000 C CNN
F 2 "" H 6175 6805 50  0000 C CNN
F 3 "" V 6175 6875 50  0000 C CNN
	1    6175 6875
	0    -1   -1   0   
$EndComp
$Comp
L Rnet4 R110
U 2 1 5796179C
P 6400 6875
F 0 "R110" H 6400 6955 50  0000 C CNN
F 1 "1k" H 6400 6875 50  0000 C CNN
F 2 "" H 6400 6805 50  0000 C CNN
F 3 "" V 6400 6875 50  0000 C CNN
	2    6400 6875
	0    -1   -1   0   
$EndComp
Text GLabel 5175 3725 2    60   Input ~ 0
SensorGND
Wire Wire Line
	6475 3825 6975 3825
Connection ~ 6725 3825
Wire Wire Line
	6475 3525 7175 3525
Connection ~ 6725 3525
Wire Wire Line
	5175 3500 5875 3500
Wire Wire Line
	5500 2900 6225 2900
Wire Wire Line
	5500 3100 6225 3100
Wire Wire Line
	5875 3200 5875 2900
Connection ~ 5875 2900
Wire Wire Line
	5600 3200 5600 3100
Connection ~ 5600 3100
Connection ~ 5600 3500
Wire Wire Line
	5175 3725 5175 3500
Wire Wire Line
	9650 4675 9925 4675
Connection ~ 9750 4675
Wire Wire Line
	9750 4975 10075 4975
Wire Wire Line
	10075 4975 10075 5125
Wire Wire Line
	6425 2175 6925 2175
Connection ~ 6675 2175
Wire Wire Line
	6425 1875 7125 1875
Connection ~ 6675 1875
Wire Wire Line
	5175 1875 5875 1875
Wire Wire Line
	5500 1275 6225 1275
Wire Wire Line
	5500 1475 6225 1475
Wire Wire Line
	5875 1575 5875 1275
Connection ~ 5875 1275
Wire Wire Line
	5600 1575 5600 1475
Connection ~ 5600 1475
Connection ~ 5600 1875
Wire Wire Line
	5175 2100 5175 1875
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
Wire Wire Line
	2925 6350 2925 6250
Wire Wire Line
	3350 7025 3125 7025
Wire Wire Line
	3350 6500 3350 6725
Wire Wire Line
	3125 6575 2850 6575
Wire Wire Line
	3125 6500 3125 6725
Connection ~ 3125 6575
Wire Wire Line
	3350 6725 2850 6725
Wire Wire Line
	6275 5550 5975 5550
Wire Wire Line
	6275 5250 6275 5850
Connection ~ 6125 5550
Connection ~ 6275 5550
Wire Wire Line
	6275 4950 5975 4950
Connection ~ 6275 5350
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
Wire Notes Line
	7825 4050 4100 4050
Wire Notes Line
	4100 4050 4100 625 
Wire Notes Line
	4100 625  7825 625 
Wire Notes Line
	7825 625  7825 4050
Wire Notes Line
	8150 4250 10850 4250
Wire Notes Line
	10850 4250 10850 5875
Wire Notes Line
	10850 5875 8150 5875
Wire Notes Line
	8150 5875 8150 4250
Connection ~ 6975 3525
Connection ~ 6925 1875
Wire Wire Line
	8725 1925 8725 2125
Wire Wire Line
	8725 2400 8725 2600
Wire Wire Line
	8725 2875 8725 3075
Wire Wire Line
	8725 3350 8725 3550
Wire Wire Line
	9700 1150 10250 1150
Connection ~ 9825 1150
Connection ~ 10100 1150
Wire Wire Line
	9725 1450 10100 1450
Connection ~ 9825 1450
Wire Notes Line
	8050 625  10875 625 
Wire Notes Line
	10875 625  10875 4050
Wire Notes Line
	10875 4050 8050 4050
Wire Notes Line
	8050 4050 8050 625 
Wire Notes Line
	5225 1175 5225 1650
Wire Notes Line
	5225 1650 5475 1650
Wire Notes Line
	5475 1650 5475 1175
Wire Notes Line
	5475 1175 5225 1175
Wire Notes Line
	5225 2800 5225 3225
Wire Notes Line
	5225 3225 5500 3225
Wire Notes Line
	5500 3225 5500 2800
Wire Notes Line
	5500 2800 5225 2800
Wire Wire Line
	3350 6200 3350 6150
Wire Wire Line
	3350 6150 2925 6150
Wire Wire Line
	2925 6050 3125 6050
Wire Wire Line
	3125 6050 3125 6200
$EndSCHEMATC
