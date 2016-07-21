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
Text Notes 5975 1950 0    60   ~ 0
This buffer prevents possibility for spurious \nsoftware to activate power to the bridge \nSecurity reasons
Text GLabel 7500 2475 0    60   Input ~ 0
BRAKELIGHTS_ON_SIGNAL
Text GLabel 7500 2850 0    60   Input ~ 0
IGN_ON_SIGNAL
Text GLabel 7500 2225 0    60   Input ~ 0
PARKBRAKE_NON_INVERT_SIGNAL
Text GLabel 7500 2350 0    60   Input ~ 0
PARKBRAKE_INVERT_SIGNAL
Text GLabel 7500 2975 0    60   Input ~ 0
TIGHTEN_CMD
Text GLabel 7500 2725 0    60   Input ~ 0
SAFE_TO_RELEASE
Text GLabel 7500 2600 0    60   Input ~ 0
Enable_bridge_PWR
$Comp
L C C?
U 1 1 5793D998
P 8400 1850
F 0 "C?" H 8425 1950 50  0000 L CNN
F 1 "1n" H 8425 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 8438 1700 50  0001 C CNN
F 3 "" H 8400 1850 50  0000 C CNN
F 4 "2494247" H 8400 1850 60  0001 C CNN "farnell"
F 5 "GRM2165C1H102JA01D" H 8400 1850 60  0001 C CNN "part"
	1    8400 1850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5793D9CB
P 8650 1850
F 0 "C?" H 8675 1950 50  0000 L CNN
F 1 "100n" H 8675 1750 50  0000 L CNN
F 2 "" H 8688 1700 50  0000 C CNN
F 3 "" H 8650 1850 50  0000 C CNN
F 4 "0805B104J500CT" H 8650 1850 60  0001 C CNN "part"
F 5 "2496940" H 8650 1850 60  0001 C CNN "farnell"
	1    8650 1850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5793D9FE
P 8400 2000
F 0 "#PWR?" H 8400 1750 50  0001 C CNN
F 1 "GND" H 8400 1850 50  0001 C CNN
F 2 "" H 8400 2000 50  0000 C CNN
F 3 "" H 8400 2000 50  0000 C CNN
	1    8400 2000
	1    0    0    -1  
$EndComp
Text Label 8275 2000 2    60   ~ 0
VSS
Text Label 8300 1700 2    60   ~ 0
VDD
Text GLabel 8850 1700 2    60   Input ~ 0
+3.3V
$Comp
L 74LS241 U?
U 1 1 5793E341
P 8350 2800
F 0 "U?" H 8400 2600 50  0000 C CNN
F 1 "SN74LVC245ADBR" H 8400 2475 50  0000 C CNN
F 2 "Housings_SSOP:SSOP-20_5.3x7.2mm_Pitch0.65mm" H 8350 2800 50  0001 C CNN
F 3 "" H 8350 2800 50  0000 C CNN
F 4 "SN74LVC245ADBR" H 8350 2800 60  0001 C CNN "part"
F 5 "1595415" H 8350 2800 60  0001 C CNN "farnell"
	1    8350 2800
	1    0    0    -1  
$EndComp
Text Label 7650 3300 2    60   ~ 0
VDD
NoConn ~ 7650 3000
$Comp
L R R?
U 1 1 5793F16C
P 2625 2500
F 0 "R?" V 2705 2500 50  0000 C CNN
F 1 "10k" V 2625 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2555 2500 50  0001 C CNN
F 3 "" H 2625 2500 50  0000 C CNN
F 4 "MC01W0805110K" V 2625 2500 60  0001 C CNN "part"
F 5 "9332391" V 2625 2500 60  0001 C CNN "farnell"
	1    2625 2500
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5793F18B
P 2625 2875
F 0 "R?" V 2705 2875 50  0000 C CNN
F 1 "1k" V 2625 2875 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2555 2875 50  0001 C CNN
F 3 "" H 2625 2875 50  0000 C CNN
F 4 "MC01W080511K" V 2625 2875 60  0001 C CNN "part"
F 5 "9332383" V 2625 2875 60  0001 C CNN "farnell"
	1    2625 2875
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5793F1C0
P 2325 2875
F 0 "C?" H 2350 2975 50  0000 L CNN
F 1 "100n" H 2350 2775 50  0000 L CNN
F 2 "" H 2363 2725 50  0000 C CNN
F 3 "" H 2325 2875 50  0000 C CNN
F 4 "0805B104J500CT" H 2325 2875 60  0001 C CNN "part"
F 5 "2496940" H 2325 2875 60  0001 C CNN "farnell"
	1    2325 2875
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 5793F24E
P 2925 2850
F 0 "D?" H 2925 2950 50  0000 C CNN
F 1 "3.3V" H 2925 2750 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 2925 2850 50  0001 C CNN
F 3 "" H 2925 2850 50  0000 C CNN
F 4 "MMSZ4684" H 2925 2850 60  0001 C CNN "part"
F 5 "2454044" H 2925 2850 60  0001 C CNN "farnell"
	1    2925 2850
	0    1    1    0   
$EndComp
Text GLabel 2625 2350 1    60   Input ~ 0
+12V_power
$Comp
L GND #PWR?
U 1 1 5793F3F6
P 2625 3025
F 0 "#PWR?" H 2625 2775 50  0001 C CNN
F 1 "GND" H 2625 2875 50  0000 C CNN
F 2 "" H 2625 3025 50  0000 C CNN
F 3 "" H 2625 3025 50  0000 C CNN
	1    2625 3025
	1    0    0    -1  
$EndComp
Text GLabel 2925 2650 2    60   Input ~ 0
PWR_voltsense
Wire Wire Line
	8275 2000 8650 2000
Wire Wire Line
	8300 1700 8850 1700
Wire Wire Line
	7650 3200 7650 3300
Wire Wire Line
	7650 2300 7650 2225
Wire Wire Line
	7650 2225 7500 2225
Wire Wire Line
	7650 2400 7600 2400
Wire Wire Line
	7600 2400 7600 2350
Wire Wire Line
	7600 2350 7500 2350
Wire Wire Line
	7650 2500 7650 2475
Wire Wire Line
	7650 2475 7500 2475
Wire Wire Line
	7650 2600 7500 2600
Wire Wire Line
	7650 2700 7650 2725
Wire Wire Line
	7650 2725 7500 2725
Wire Wire Line
	7650 2800 7600 2800
Wire Wire Line
	7600 2800 7600 2850
Wire Wire Line
	7600 2850 7500 2850
Wire Wire Line
	7650 2900 7575 2900
Wire Wire Line
	7575 2900 7575 2975
Wire Wire Line
	7575 2975 7500 2975
Connection ~ 8650 1700
Connection ~ 8400 1700
Connection ~ 8400 2000
Wire Wire Line
	2625 2650 2625 2725
Wire Wire Line
	2325 3025 2925 3025
Wire Wire Line
	2925 3025 2925 2950
Connection ~ 2625 3025
Wire Wire Line
	2925 2650 2925 2750
Wire Wire Line
	2325 2650 2925 2650
Wire Wire Line
	2325 2650 2325 2725
Connection ~ 2625 2650
Text Notes 1300 1900 0    60   ~ 0
Battery sense circuit\nNOTE! only measure when \n12V power is activated
Text Notes 2875 2450 0    60   ~ 0
12V->1.2V \n24V->2.4V\nmax around 30v
Wire Notes Line
	1225 1550 4075 1550
Wire Notes Line
	4075 1550 4075 3325
Wire Notes Line
	4075 3325 1225 3325
Wire Notes Line
	1225 3325 1225 1550
Text GLabel 9200 2225 2    60   Input ~ 0
BUTTON_SIG
Text GLabel 9200 2350 2    60   Input ~ 0
BUTTON_INV_SIG
Text GLabel 9200 2475 2    60   Input ~ 0
LIGHTS_ON_SIG
Text GLabel 9200 2600 2    60   Input ~ 0
PWR_ENANBLED_SIG
Text GLabel 9200 2725 2    60   Input ~ 0
RELEASE_CMD_SIG
Text GLabel 9200 2850 2    60   Input ~ 0
IGN_ON_SIG
Text GLabel 9200 2975 2    60   Input ~ 0
TIGHTEN_CMD_SIG
NoConn ~ 9050 3000
Wire Wire Line
	9050 2300 9050 2225
Wire Wire Line
	9050 2225 9200 2225
Wire Wire Line
	9050 2400 9100 2400
Wire Wire Line
	9100 2400 9100 2350
Wire Wire Line
	9100 2350 9200 2350
Wire Wire Line
	9050 2500 9050 2475
Wire Wire Line
	9050 2475 9200 2475
Wire Wire Line
	9050 2600 9200 2600
Wire Wire Line
	9050 2700 9050 2725
Wire Wire Line
	9050 2725 9200 2725
Wire Wire Line
	9050 2800 9150 2800
Wire Wire Line
	9150 2800 9150 2850
Wire Wire Line
	9150 2850 9200 2850
Wire Wire Line
	9050 2900 9125 2900
Wire Wire Line
	9125 2900 9125 2975
Wire Wire Line
	9125 2975 9200 2975
Wire Notes Line
	5875 1475 10175 1475
Wire Notes Line
	10175 1475 10175 3475
Wire Notes Line
	10175 3475 5875 3475
Wire Notes Line
	5875 3475 5875 1475
$Comp
L R R?
U 1 1 57945BE0
P 2600 4900
F 0 "R?" V 2680 4900 50  0000 C CNN
F 1 "10k" V 2600 4900 50  0000 C CNN
F 2 "" V 2530 4900 50  0000 C CNN
F 3 "" H 2600 4900 50  0000 C CNN
F 4 "MC01W0805110K" V 2600 4900 60  0001 C CNN "part"
F 5 "9332391" V 2600 4900 60  0001 C CNN "farnell"
	1    2600 4900
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57945BE6
P 2600 5275
F 0 "R?" V 2680 5275 50  0000 C CNN
F 1 "1k" V 2600 5275 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2530 5275 50  0001 C CNN
F 3 "" H 2600 5275 50  0000 C CNN
F 4 "9332383" V 2600 5275 60  0001 C CNN "farnell"
F 5 "MC01W080511K" V 2600 5275 60  0001 C CNN "part"
	1    2600 5275
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57945BEC
P 2300 5275
F 0 "C?" H 2325 5375 50  0000 L CNN
F 1 "100n" H 2325 5175 50  0000 L CNN
F 2 "" H 2338 5125 50  0000 C CNN
F 3 "" H 2300 5275 50  0000 C CNN
F 4 "0805B104J500CT" H 2300 5275 60  0001 C CNN "part"
F 5 "2496940" H 2300 5275 60  0001 C CNN "farnell"
	1    2300 5275
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 57945BF2
P 2900 5250
F 0 "D?" H 2900 5350 50  0000 C CNN
F 1 "3.3V" H 2900 5150 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 2900 5250 50  0001 C CNN
F 3 "" H 2900 5250 50  0000 C CNN
F 4 "2454044" H 2900 5250 60  0001 C CNN "farnell"
F 5 "MMSZ4684" H 2900 5250 60  0001 C CNN "part"
	1    2900 5250
	0    1    1    0   
$EndComp
Text GLabel 2600 4750 1    60   Input ~ 0
+12V_feed
$Comp
L GND #PWR?
U 1 1 57945BF9
P 2600 5425
F 0 "#PWR?" H 2600 5175 50  0001 C CNN
F 1 "GND" H 2600 5275 50  0000 C CNN
F 2 "" H 2600 5425 50  0000 C CNN
F 3 "" H 2600 5425 50  0000 C CNN
	1    2600 5425
	1    0    0    -1  
$EndComp
Text GLabel 2900 5050 2    60   Input ~ 0
Feed_voltsense
Wire Wire Line
	2600 5050 2600 5125
Wire Wire Line
	2300 5425 2900 5425
Wire Wire Line
	2900 5425 2900 5350
Connection ~ 2600 5425
Wire Wire Line
	2900 5050 2900 5150
Wire Wire Line
	2300 5050 2900 5050
Wire Wire Line
	2300 5050 2300 5125
Connection ~ 2600 5050
Text Notes 1275 4125 0    60   ~ 0
IGN voltage sense circuit
Text Notes 2850 4850 0    60   ~ 0
12V->1.2V \n24V->2.4V\nmax around 30v
Wire Notes Line
	1200 3950 4050 3950
Wire Notes Line
	4050 3950 4050 5725
Wire Notes Line
	4050 5725 1200 5725
Wire Notes Line
	1200 5725 1200 3950
$Comp
L R R?
U 1 1 579422D1
P 7725 4825
F 0 "R?" V 7805 4825 50  0000 C CNN
F 1 "560" V 7725 4825 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 7655 4825 50  0001 C CNN
F 3 "" H 7725 4825 50  0000 C CNN
F 4 "MC01W08051560R" V 7725 4825 60  0001 C CNN "part"
F 5 "9333363" V 7725 4825 60  0001 C CNN "farnell"
	1    7725 4825
	1    0    0    -1  
$EndComp
$Comp
L BSS138 Q?
U 1 1 57942372
P 7625 5175
F 0 "Q?" H 7825 5250 50  0000 L CNN
F 1 "2N7002" H 7825 5175 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7825 5100 50  0001 L CIN
F 3 "" H 7625 5175 50  0000 L CNN
F 4 "2N7002" H 7625 5175 60  0001 C CNN "part"
F 5 "1510761" H 7625 5175 60  0001 C CNN "farnell"
	1    7625 5175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57942417
P 7725 5375
F 0 "#PWR?" H 7725 5125 50  0001 C CNN
F 1 "GND" H 7725 5225 50  0000 C CNN
F 2 "" H 7725 5375 50  0000 C CNN
F 3 "" H 7725 5375 50  0000 C CNN
	1    7725 5375
	1    0    0    -1  
$EndComp
Text GLabel 7725 4675 0    60   Input ~ 0
Parkbrake_indicator_lamp
Text GLabel 7425 5225 0    60   Input ~ 0
INDICATOR_ON
Text Notes 6375 4450 0    60   ~ 0
Turns on (grounds) indicator LED
Wire Notes Line
	6275 4275 8450 4275
Wire Notes Line
	8450 4275 8450 5675
Wire Notes Line
	8450 5675 6275 5675
Wire Notes Line
	6275 5675 6275 4275
$Comp
L D D?
U 1 1 5794388E
P 7400 4975
F 0 "D?" H 7400 5075 50  0000 C CNN
F 1 "D" H 7400 4875 50  0000 C CNN
F 2 "" H 7400 4975 50  0000 C CNN
F 3 "" H 7400 4975 50  0000 C CNN
	1    7400 4975
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 4975 7725 4975
Text GLabel 7250 4975 0    60   Input ~ 0
+12V_feed
Text Notes 6375 5475 0    60   ~ 0
Diode is for ESD protection
$EndSCHEMATC
