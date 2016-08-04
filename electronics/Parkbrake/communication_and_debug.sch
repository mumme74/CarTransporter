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
Sheet 7 10
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
L SN65HVD1050D U26
U 1 1 5791E80E
P 2925 4025
F 0 "U26" H 2975 4375 60  0000 C CNN
F 1 "SN65HVD1050D" H 2925 3625 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 3025 3875 60  0001 C CNN
F 3 "" H 3025 3875 60  0000 C CNN
F 4 "SN65HVD1050D" H 2925 4025 60  0001 C CNN "part"
F 5 "1220984" H 2925 4025 60  0001 C CNN "farnell"
	1    2925 4025
	1    0    0    -1  
$EndComp
$Comp
L R R88
U 1 1 5791E8BD
P 3275 3550
F 0 "R88" V 3355 3550 50  0000 C CNN
F 1 "10k" V 3275 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3205 3550 50  0001 C CNN
F 3 "" H 3275 3550 50  0000 C CNN
F 4 "MC01W0805110K" V 3275 3550 60  0001 C CNN "part"
F 5 "9332391" V 3275 3550 60  0001 C CNN "farnell"
	1    3275 3550
	0    -1   -1   0   
$EndComp
$Comp
L Jumper_NO_Small JP2
U 1 1 5791E904
P 3525 3550
F 0 "JP2" H 3525 3630 50  0000 C CNN
F 1 "Jumper_NO_Small" H 3535 3490 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 3525 3550 50  0001 C CNN
F 3 "" H 3525 3550 50  0000 C CNN
F 4 "RC1206JR-070RL" H 3525 3550 60  0001 C CNN "part"
F 5 "9240276" H 3525 3550 60  0001 C CNN "farnell"
	1    3525 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR085
U 1 1 5791E9C0
P 3125 3550
F 0 "#PWR085" H 3125 3300 50  0001 C CNN
F 1 "GND" H 3125 3400 50  0001 C CNN
F 2 "" H 3125 3550 50  0000 C CNN
F 3 "" H 3125 3550 50  0000 C CNN
	1    3125 3550
	1    0    0    -1  
$EndComp
$Comp
L C C57
U 1 1 5791E9E8
P 2450 3450
F 0 "C57" H 2475 3550 50  0000 L CNN
F 1 "100n" H 2475 3350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 2488 3300 50  0001 C CNN
F 3 "" H 2450 3450 50  0000 C CNN
F 4 "2496940" H 2450 3450 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 2450 3450 60  0001 C CNN "part"
	1    2450 3450
	1    0    0    -1  
$EndComp
$Comp
L C C58
U 1 1 5791EA0D
P 2700 3450
F 0 "C58" H 2725 3550 50  0000 L CNN
F 1 "1n" H 2725 3350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 2738 3300 50  0001 C CNN
F 3 "" H 2700 3450 50  0000 C CNN
F 4 "2494247" H 2700 3450 60  0001 C CNN "farnell"
F 5 "GRM2165C1H102JA01D" H 2700 3450 60  0001 C CNN "part"
	1    2700 3450
	1    0    0    -1  
$EndComp
Text GLabel 2450 3300 0    60   Input ~ 0
+5V
Wire Wire Line
	2700 3300 2450 3300
Wire Wire Line
	2700 3600 2450 3600
$Comp
L GND #PWR086
U 1 1 5791EA76
P 2450 3600
F 0 "#PWR086" H 2450 3350 50  0001 C CNN
F 1 "GND" H 2450 3450 50  0001 C CNN
F 2 "" H 2450 3600 50  0000 C CNN
F 3 "" H 2450 3600 50  0000 C CNN
	1    2450 3600
	1    0    0    -1  
$EndComp
Text GLabel 2300 4125 0    60   Input ~ 0
+5V
$Comp
L GND #PWR087
U 1 1 5791EABC
P 2400 3975
F 0 "#PWR087" H 2400 3725 50  0001 C CNN
F 1 "GND" H 2400 3825 50  0001 C CNN
F 2 "" H 2400 3975 50  0000 C CNN
F 3 "" H 2400 3975 50  0000 C CNN
	1    2400 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	2475 3975 2400 3975
Wire Wire Line
	2475 4125 2300 4125
$Comp
L C C59
U 1 1 5791EB00
P 3425 4425
F 0 "C59" H 3450 4525 50  0000 L CNN
F 1 "100n" H 3450 4325 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3463 4275 50  0001 C CNN
F 3 "" H 3425 4425 50  0000 C CNN
F 4 "2496940" H 3425 4425 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 3425 4425 60  0001 C CNN "part"
	1    3425 4425
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR088
U 1 1 5791EB6A
P 3425 4575
F 0 "#PWR088" H 3425 4325 50  0001 C CNN
F 1 "GND" H 3425 4425 50  0001 C CNN
F 2 "" H 3425 4575 50  0000 C CNN
F 3 "" H 3425 4575 50  0000 C CNN
	1    3425 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 3825 3425 3550
Text GLabel 3625 3550 2    60   Input ~ 0
+5V
Text GLabel 2475 4275 0    60   Input ~ 0
CAN_RX
Text GLabel 2475 3825 0    60   Input ~ 0
CAN_TX
Text GLabel 3675 3900 2    60   Input ~ 0
CAN_HI
Text GLabel 3675 4200 2    60   Input ~ 0
CAN_LO
Wire Notes Line
	1775 3075 4650 3075
Wire Notes Line
	4650 3075 4650 4725
Wire Notes Line
	4650 4725 1775 4725
Wire Notes Line
	1775 4725 1775 3075
Text Notes 1825 3175 0    60   ~ 0
CAN interface
Text Notes 2925 3375 0    60   ~ 0
bridge to run silent mode
$Comp
L R R89
U 1 1 5791F232
P 3575 4050
F 0 "R89" V 3655 4050 50  0000 C CNN
F 1 "120" V 3575 4050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3505 4050 50  0001 C CNN
F 3 "" H 3575 4050 50  0000 C CNN
F 4 "MC01W08051120R" V 3575 4050 60  0001 C CNN "part"
F 5 "9332480" V 3575 4050 60  0001 C CNN "farnell"
	1    3575 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3475 4200 3675 4200
Wire Wire Line
	3475 4200 3475 4125
Wire Wire Line
	3475 4125 3425 4125
Connection ~ 3575 4200
Wire Wire Line
	3425 3975 3475 3975
Wire Wire Line
	3475 3975 3475 3900
Wire Wire Line
	3475 3900 3675 3900
Connection ~ 3575 3900
$Comp
L CONN_01X03 P3
U 1 1 57920C75
P 6900 2875
F 0 "P3" H 6900 3075 50  0000 C CNN
F 1 "UART" V 7000 2875 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03" H 6900 2875 50  0001 C CNN
F 3 "" H 6900 2875 50  0000 C CNN
F 4 "826629-3" H 6900 2875 60  0001 C CNN "part"
F 5 "3418297" H 6900 2875 60  0001 C CNN "farnell"
	1    6900 2875
	-1   0    0    1   
$EndComp
$Comp
L R R92
U 1 1 57920D82
P 7250 2775
F 0 "R92" V 7330 2775 50  0000 C CNN
F 1 "330" V 7250 2775 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7180 2775 50  0001 C CNN
F 3 "" H 7250 2775 50  0000 C CNN
F 4 "MC01W08051330R" V 7250 2775 60  0001 C CNN "part"
F 5 "9333037" V 7250 2775 60  0001 C CNN "farnell"
	1    7250 2775
	0    -1   -1   0   
$EndComp
$Comp
L R R93
U 1 1 57920DB5
P 7250 2875
F 0 "R93" V 7330 2875 50  0000 C CNN
F 1 "330" V 7250 2875 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7180 2875 50  0001 C CNN
F 3 "" H 7250 2875 50  0000 C CNN
F 4 "MC01W08051330R" V 7250 2875 60  0001 C CNN "part"
F 5 "9333037" V 7250 2875 60  0001 C CNN "farnell"
	1    7250 2875
	0    1    1    0   
$EndComp
$Comp
L GND #PWR089
U 1 1 57920F20
P 7100 2975
F 0 "#PWR089" H 7100 2725 50  0001 C CNN
F 1 "GND" H 7100 2825 50  0000 C CNN
F 2 "" H 7100 2975 50  0000 C CNN
F 3 "" H 7100 2975 50  0000 C CNN
	1    7100 2975
	1    0    0    -1  
$EndComp
Text GLabel 7400 2875 2    60   Input ~ 0
USART_TX
Text GLabel 7400 2775 2    60   Input ~ 0
USART_RX
Text Notes 6575 3375 0    60   ~ 0
Uses usart1 pins 42-3 because \nthey're 5v tolerant
Text Notes 6600 2550 0    60   ~ 0
UART for debugging 
Wire Notes Line
	6475 2425 8450 2425
Wire Notes Line
	8450 2425 8450 3425
Wire Notes Line
	8450 3425 6475 3425
Wire Notes Line
	6475 3425 6475 2425
$Comp
L GND #PWR090
U 1 1 579225C8
P 6875 4250
F 0 "#PWR090" H 6875 4000 50  0001 C CNN
F 1 "GND" H 6875 4100 50  0001 C CNN
F 2 "" H 6875 4250 50  0000 C CNN
F 3 "" H 6875 4250 50  0000 C CNN
	1    6875 4250
	1    0    0    -1  
$EndComp
Text GLabel 7950 4100 2    60   Input ~ 0
+3.3V
$Comp
L CONN_01X08 P2
U 1 1 57922780
P 6550 4600
F 0 "P2" H 6550 5050 50  0000 C CNN
F 1 "JTAG" V 6650 4600 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x08" H 6550 4600 50  0001 C CNN
F 3 "" H 6550 4600 50  0000 C CNN
F 4 "61300811121" H 6550 4600 60  0001 C CNN "part"
F 5 "2356160" H 6550 4600 60  0001 C CNN "farnell"
	1    6550 4600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6750 4450 7975 4450
Wire Wire Line
	6750 4550 7975 4550
Wire Wire Line
	6750 4650 7975 4650
Wire Wire Line
	6750 4750 7975 4750
Wire Wire Line
	6750 4850 7975 4850
Wire Wire Line
	6750 4950 7975 4950
Text GLabel 7975 4450 2    60   Input ~ 0
JTCLK
Text GLabel 7975 4550 2    60   Input ~ 0
JTDO
Text GLabel 7975 4650 2    60   Input ~ 0
JTDI
Text GLabel 7975 4750 2    60   Input ~ 0
JTMS
Text GLabel 7975 4850 2    60   Input ~ 0
JTNRST
Text GLabel 7975 4950 2    60   Input ~ 0
NRST
Wire Notes Line
	6275 3825 8400 3825
Wire Notes Line
	8400 3825 8400 5450
Wire Notes Line
	8400 5450 6275 5450
Wire Notes Line
	6275 5450 6275 3825
Text Notes 6375 3975 0    60   ~ 0
JTAG port
$Comp
L R R90
U 1 1 5792478C
P 7000 4250
F 0 "R90" V 7080 4250 50  0000 C CNN
F 1 "10k" V 7000 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 6930 4250 50  0001 C CNN
F 3 "" H 7000 4250 50  0000 C CNN
F 4 "MC01W0805110K" V 7000 4250 60  0001 C CNN "part"
F 5 "9332391" V 7000 4250 60  0001 C CNN "farnell"
	1    7000 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 4250 6750 4100
Wire Wire Line
	6750 4100 7950 4100
Wire Wire Line
	6750 4350 6800 4350
Wire Wire Line
	6800 4350 6800 4250
Wire Wire Line
	6800 4250 6875 4250
$Comp
L R R91
U 1 1 57924D13
P 7175 4250
F 0 "R91" V 7255 4250 50  0000 C CNN
F 1 "10k" V 7175 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7105 4250 50  0001 C CNN
F 3 "" H 7175 4250 50  0000 C CNN
F 4 "MC01W0805110K" V 7175 4250 60  0001 C CNN "part"
F 5 "9332391" V 7175 4250 60  0001 C CNN "farnell"
	1    7175 4250
	1    0    0    -1  
$EndComp
$Comp
L R R94
U 1 1 57924D64
P 7350 4250
F 0 "R94" V 7430 4250 50  0000 C CNN
F 1 "10k" V 7350 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7280 4250 50  0001 C CNN
F 3 "" H 7350 4250 50  0000 C CNN
F 4 "MC01W0805110K" V 7350 4250 60  0001 C CNN "part"
F 5 "9332391" V 7350 4250 60  0001 C CNN "farnell"
	1    7350 4250
	1    0    0    -1  
$EndComp
$Comp
L R R95
U 1 1 57924DA8
P 7525 4250
F 0 "R95" V 7605 4250 50  0000 C CNN
F 1 "10k" V 7525 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7455 4250 50  0001 C CNN
F 3 "" H 7525 4250 50  0000 C CNN
F 4 "MC01W0805110K" V 7525 4250 60  0001 C CNN "part"
F 5 "9332391" V 7525 4250 60  0001 C CNN "farnell"
	1    7525 4250
	1    0    0    -1  
$EndComp
$Comp
L R R96
U 1 1 57924E03
P 7700 4250
F 0 "R96" V 7780 4250 50  0000 C CNN
F 1 "10k" V 7700 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7630 4250 50  0001 C CNN
F 3 "" H 7700 4250 50  0000 C CNN
F 4 "MC01W0805110K" V 7700 4250 60  0001 C CNN "part"
F 5 "9332391" V 7700 4250 60  0001 C CNN "farnell"
	1    7700 4250
	1    0    0    -1  
$EndComp
Connection ~ 7000 4100
Connection ~ 7175 4100
Connection ~ 7350 4100
Connection ~ 7525 4100
Connection ~ 7700 4100
Wire Wire Line
	7000 4400 7000 4450
Connection ~ 7000 4450
Wire Wire Line
	7175 4400 7175 4550
Connection ~ 7175 4550
Wire Wire Line
	7350 4400 7350 4650
Connection ~ 7350 4650
Wire Wire Line
	7525 4400 7525 4750
Connection ~ 7525 4750
Wire Wire Line
	7700 4400 7700 4850
Connection ~ 7700 4850
$Comp
L R R97
U 1 1 5792546C
P 7875 4250
F 0 "R97" V 7955 4250 50  0000 C CNN
F 1 "10k" V 7875 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7805 4250 50  0001 C CNN
F 3 "" H 7875 4250 50  0000 C CNN
F 4 "MC01W0805110K" V 7875 4250 60  0001 C CNN "part"
F 5 "9332391" V 7875 4250 60  0001 C CNN "farnell"
	1    7875 4250
	1    0    0    -1  
$EndComp
Connection ~ 7875 4100
$Comp
L C C60
U 1 1 579264A6
P 7875 5100
F 0 "C60" H 7900 5200 50  0000 L CNN
F 1 "100n" H 7900 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7913 4950 50  0001 C CNN
F 3 "" H 7875 5100 50  0000 C CNN
F 4 "2496940" H 7875 5100 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 7875 5100 60  0001 C CNN "part"
	1    7875 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR091
U 1 1 579264FD
P 7875 5250
F 0 "#PWR091" H 7875 5000 50  0001 C CNN
F 1 "GND" H 7875 5100 50  0001 C CNN
F 2 "" H 7875 5250 50  0000 C CNN
F 3 "" H 7875 5250 50  0000 C CNN
	1    7875 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7875 4950 7875 4400
Connection ~ 7875 4950
Text Notes 3650 4475 0    60   ~ 0
Only termination \nresistor on endnodes
$EndSCHEMATC
