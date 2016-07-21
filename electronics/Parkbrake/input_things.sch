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
Sheet 8 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 5975 3100 0    60   ~ 0
This buffer prevents possibility for spurious \nsoftware to activate power to the bridge \nSecurity reasons
Text GLabel 7500 3625 0    60   Input ~ 0
BRAKELIGHTS_ON_SIGNAL
Text GLabel 7500 4000 0    60   Input ~ 0
IGN_ON_SIGNAL
Text GLabel 7500 3375 0    60   Input ~ 0
PARKBRAKE_NON_INVERT_SIGNAL
Text GLabel 7500 3500 0    60   Input ~ 0
PARKBRAKE_INVERT_SIGNAL
Text GLabel 7500 4125 0    60   Input ~ 0
TIGHTEN_CMD
Text GLabel 7500 3875 0    60   Input ~ 0
SAFE_TO_RELEASE
Text GLabel 7500 3750 0    60   Input ~ 0
Enable_bridge_PWR
$Comp
L C C?
U 1 1 5793D998
P 8400 3000
F 0 "C?" H 8425 3100 50  0000 L CNN
F 1 "1n" H 8425 2900 50  0000 L CNN
F 2 "" H 8438 2850 50  0000 C CNN
F 3 "" H 8400 3000 50  0000 C CNN
	1    8400 3000
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5793D9CB
P 8650 3000
F 0 "C?" H 8675 3100 50  0000 L CNN
F 1 "100n" H 8675 2900 50  0000 L CNN
F 2 "" H 8688 2850 50  0000 C CNN
F 3 "" H 8650 3000 50  0000 C CNN
	1    8650 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5793D9FE
P 8400 3150
F 0 "#PWR?" H 8400 2900 50  0001 C CNN
F 1 "GND" H 8400 3000 50  0001 C CNN
F 2 "" H 8400 3150 50  0000 C CNN
F 3 "" H 8400 3150 50  0000 C CNN
	1    8400 3150
	1    0    0    -1  
$EndComp
Text Label 8275 3150 2    60   ~ 0
VSS
Text Label 8300 2850 2    60   ~ 0
VDD
Text GLabel 8850 2850 2    60   Input ~ 0
+3.3V
$Comp
L 74LS241 U?
U 1 1 5793E341
P 8350 3950
F 0 "U?" H 8400 3750 50  0000 C CNN
F 1 "SN74LVC245ADBR" H 8400 3625 50  0000 C CNN
F 2 "Housings_SSOP:SSOP-20_5.3x7.2mm_Pitch0.65mm" H 8350 3950 50  0001 C CNN
F 3 "" H 8350 3950 50  0000 C CNN
F 4 "SN74LVC245ADBR" H 8350 3950 60  0001 C CNN "part"
F 5 "1595415" H 8350 3950 60  0001 C CNN "farnell"
	1    8350 3950
	1    0    0    -1  
$EndComp
Text Label 7650 4450 2    60   ~ 0
VDD
NoConn ~ 7650 4150
$Comp
L R R?
U 1 1 5793F16C
P 2925 3700
F 0 "R?" V 3005 3700 50  0000 C CNN
F 1 "10k" V 2925 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2855 3700 50  0001 C CNN
F 3 "" H 2925 3700 50  0000 C CNN
F 4 "MC01W0805110K" V 2925 3700 60  0001 C CNN "part"
F 5 "9332391" V 2925 3700 60  0001 C CNN "farnell"
	1    2925 3700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5793F18B
P 2925 4075
F 0 "R?" V 3005 4075 50  0000 C CNN
F 1 "1k" V 2925 4075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2855 4075 50  0001 C CNN
F 3 "" H 2925 4075 50  0000 C CNN
F 4 "MC01W080511K" V 2925 4075 60  0001 C CNN "part"
F 5 "9332383" V 2925 4075 60  0001 C CNN "farnell"
	1    2925 4075
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5793F1C0
P 2625 4075
F 0 "C?" H 2650 4175 50  0000 L CNN
F 1 "100n" H 2650 3975 50  0000 L CNN
F 2 "" H 2663 3925 50  0000 C CNN
F 3 "" H 2625 4075 50  0000 C CNN
	1    2625 4075
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 5793F24E
P 3225 4050
F 0 "D?" H 3225 4150 50  0000 C CNN
F 1 "3.3V" H 3225 3950 50  0000 C CNN
F 2 "" H 3225 4050 50  0000 C CNN
F 3 "" H 3225 4050 50  0000 C CNN
	1    3225 4050
	0    1    1    0   
$EndComp
Text GLabel 2925 3550 1    60   Input ~ 0
+12V_power
$Comp
L GND #PWR?
U 1 1 5793F3F6
P 2925 4225
F 0 "#PWR?" H 2925 3975 50  0001 C CNN
F 1 "GND" H 2925 4075 50  0000 C CNN
F 2 "" H 2925 4225 50  0000 C CNN
F 3 "" H 2925 4225 50  0000 C CNN
	1    2925 4225
	1    0    0    -1  
$EndComp
Text GLabel 3225 3850 2    60   Input ~ 0
PWR_voltsense
Wire Wire Line
	8275 3150 8650 3150
Wire Wire Line
	8300 2850 8850 2850
Wire Wire Line
	7650 4350 7650 4450
Wire Wire Line
	7650 3450 7650 3375
Wire Wire Line
	7650 3375 7500 3375
Wire Wire Line
	7650 3550 7600 3550
Wire Wire Line
	7600 3550 7600 3500
Wire Wire Line
	7600 3500 7500 3500
Wire Wire Line
	7650 3650 7650 3625
Wire Wire Line
	7650 3625 7500 3625
Wire Wire Line
	7650 3750 7500 3750
Wire Wire Line
	7650 3850 7650 3875
Wire Wire Line
	7650 3875 7500 3875
Wire Wire Line
	7650 3950 7600 3950
Wire Wire Line
	7600 3950 7600 4000
Wire Wire Line
	7600 4000 7500 4000
Wire Wire Line
	7650 4050 7575 4050
Wire Wire Line
	7575 4050 7575 4125
Wire Wire Line
	7575 4125 7500 4125
Connection ~ 8650 2850
Connection ~ 8400 2850
Connection ~ 8400 3150
Wire Wire Line
	2925 3850 2925 3925
Wire Wire Line
	2625 4225 3225 4225
Wire Wire Line
	3225 4225 3225 4150
Connection ~ 2925 4225
Wire Wire Line
	3225 3850 3225 3950
Wire Wire Line
	2625 3850 3225 3850
Wire Wire Line
	2625 3850 2625 3925
Connection ~ 2925 3850
Text Notes 1600 3100 0    60   ~ 0
Battery sense circuit\nNOTE! only measure when \n12V power is activated
Text Notes 3175 3650 0    60   ~ 0
12V->1.2V \n24V->2.4V\nmax around 30v
Wire Notes Line
	1525 2750 4375 2750
Wire Notes Line
	4375 2750 4375 4525
Wire Notes Line
	4375 4525 1525 4525
Wire Notes Line
	1525 4525 1525 2750
Text GLabel 9200 3375 2    60   Input ~ 0
BUTTON_SIG
Text GLabel 9200 3500 2    60   Input ~ 0
BUTTON_INV_SIG
Text GLabel 9200 3625 2    60   Input ~ 0
LIGHTS_ON_SIG
Text GLabel 9200 3750 2    60   Input ~ 0
PWR_ENANBLED_SIG
Text GLabel 9200 3875 2    60   Input ~ 0
RELEASE_CMD_SIG
Text GLabel 9200 4000 2    60   Input ~ 0
IGN_ON_SIG
Text GLabel 9200 4125 2    60   Input ~ 0
TIGHTEN_CMD_SIG
NoConn ~ 9050 4150
Wire Wire Line
	9050 3450 9050 3375
Wire Wire Line
	9050 3375 9200 3375
Wire Wire Line
	9050 3550 9100 3550
Wire Wire Line
	9100 3550 9100 3500
Wire Wire Line
	9100 3500 9200 3500
Wire Wire Line
	9050 3650 9050 3625
Wire Wire Line
	9050 3625 9200 3625
Wire Wire Line
	9050 3750 9200 3750
Wire Wire Line
	9050 3850 9050 3875
Wire Wire Line
	9050 3875 9200 3875
Wire Wire Line
	9050 3950 9150 3950
Wire Wire Line
	9150 3950 9150 4000
Wire Wire Line
	9150 4000 9200 4000
Wire Wire Line
	9050 4050 9125 4050
Wire Wire Line
	9125 4050 9125 4125
Wire Wire Line
	9125 4125 9200 4125
Wire Notes Line
	5875 2625 10175 2625
Wire Notes Line
	10175 2625 10175 4625
Wire Notes Line
	10175 4625 5875 4625
Wire Notes Line
	5875 4625 5875 2625
$Comp
L R R?
U 1 1 57945BE0
P 2900 6100
F 0 "R?" V 2980 6100 50  0000 C CNN
F 1 "10k" V 2900 6100 50  0000 C CNN
F 2 "" V 2830 6100 50  0000 C CNN
F 3 "" H 2900 6100 50  0000 C CNN
F 4 "MC01W0805110K" V 2900 6100 60  0001 C CNN "part"
F 5 "9332391" V 2900 6100 60  0001 C CNN "farnell"
	1    2900 6100
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57945BE6
P 2900 6475
F 0 "R?" V 2980 6475 50  0000 C CNN
F 1 "1k" V 2900 6475 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2830 6475 50  0001 C CNN
F 3 "" H 2900 6475 50  0000 C CNN
F 4 "9332383" V 2900 6475 60  0001 C CNN "farnell"
F 5 "MC01W080511K" V 2900 6475 60  0001 C CNN "part"
	1    2900 6475
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57945BEC
P 2600 6475
F 0 "C?" H 2625 6575 50  0000 L CNN
F 1 "100n" H 2625 6375 50  0000 L CNN
F 2 "" H 2638 6325 50  0000 C CNN
F 3 "" H 2600 6475 50  0000 C CNN
	1    2600 6475
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 57945BF2
P 3200 6450
F 0 "D?" H 3200 6550 50  0000 C CNN
F 1 "3.3V" H 3200 6350 50  0000 C CNN
F 2 "" H 3200 6450 50  0000 C CNN
F 3 "" H 3200 6450 50  0000 C CNN
	1    3200 6450
	0    1    1    0   
$EndComp
Text GLabel 2900 5950 1    60   Input ~ 0
+12V_feed
$Comp
L GND #PWR?
U 1 1 57945BF9
P 2900 6625
F 0 "#PWR?" H 2900 6375 50  0001 C CNN
F 1 "GND" H 2900 6475 50  0000 C CNN
F 2 "" H 2900 6625 50  0000 C CNN
F 3 "" H 2900 6625 50  0000 C CNN
	1    2900 6625
	1    0    0    -1  
$EndComp
Text GLabel 3200 6250 2    60   Input ~ 0
Feed_voltsense
Wire Wire Line
	2900 6250 2900 6325
Wire Wire Line
	2600 6625 3200 6625
Wire Wire Line
	3200 6625 3200 6550
Connection ~ 2900 6625
Wire Wire Line
	3200 6250 3200 6350
Wire Wire Line
	2600 6250 3200 6250
Wire Wire Line
	2600 6250 2600 6325
Connection ~ 2900 6250
Text Notes 1575 5325 0    60   ~ 0
IGN voltage sense circuit
Text Notes 3150 6050 0    60   ~ 0
12V->1.2V \n24V->2.4V\nmax around 30v
Wire Notes Line
	1500 5150 4350 5150
Wire Notes Line
	4350 5150 4350 6925
Wire Notes Line
	4350 6925 1500 6925
Wire Notes Line
	1500 6925 1500 5150
$EndSCHEMATC
