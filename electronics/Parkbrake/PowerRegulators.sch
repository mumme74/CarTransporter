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
Sheet 5 10
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
L MIC2619 U21
U 1 1 570107B3
P 2125 6150
F 0 "U21" H 2125 6587 60  0000 C CNN
F 1 "MIC2619" H 2125 6481 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6" H 2125 6150 60  0001 C CNN
F 3 "" H 2125 6150 60  0000 C CNN
F 4 "MIC2619YD6-TR" H 2125 6150 60  0001 C CNN "part"
F 5 "2509963" H 2125 6150 60  0001 C CNN "farnell"
	1    2125 6150
	1    0    0    -1  
$EndComp
Text GLabel 1675 5550 0    60   Input ~ 0
+5V
$Comp
L C C33
U 1 1 570109D3
P 1425 6150
F 0 "C33" H 1540 6196 50  0000 L CNN
F 1 "1u" H 1540 6105 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 1463 6000 50  0001 C CNN
F 3 "" H 1425 6150 50  0000 C CNN
F 4 "2362126" H 1425 6150 60  0001 C CNN "farnell"
F 5 "GRM31MR71E105JA01L" H 1425 6150 60  0001 C CNN "part"
	1    1425 6150
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR_SMALL L1
U 1 1 57010A1F
P 2125 5550
F 0 "L1" H 2125 5765 50  0000 C CNN
F 1 "10uH" H 2125 5674 50  0000 C CNN
F 2 "myfootprints:inductor_sd54" H 2125 5550 50  0001 C CNN
F 3 "" H 2125 5550 50  0000 C CNN
F 4 "LQH32CN100K33L" H 2125 5550 60  0001 C CNN "part"
F 5 "9522204" H 2125 5550 60  0001 C CNN "farnell"
	1    2125 5550
	1    0    0    -1  
$EndComp
$Comp
L CP C36
U 1 1 57010AEF
P 3725 6150
F 0 "C36" V 3473 6150 50  0000 C CNN
F 1 "10u" V 3564 6150 50  0000 C CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeC_EIA-6032_Wave" H 3763 6000 50  0001 C CNN
F 3 "" H 3725 6150 50  0000 C CNN
F 4 "T491C106K035AT" V 3725 6150 60  0001 C CNN "part"
F 5 "1457464" V 3725 6150 60  0001 C CNN "farnell"
	1    3725 6150
	-1   0    0    1   
$EndComp
$Comp
L GNDPWR #PWR59
U 1 1 57010C16
P 1675 6425
F 0 "#PWR59" H 1675 6225 50  0001 C CNN
F 1 "GNDPWR" H 1675 6200 50  0001 C CNN
F 2 "" H 1675 6375 50  0000 C CNN
F 3 "" H 1675 6375 50  0000 C CNN
	1    1675 6425
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky_Small D26
U 1 1 57010E55
P 2775 6000
F 0 "D26" H 2775 5795 50  0000 C CNN
F 1 "min 40V 1A" H 2775 5886 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" V 2775 6000 50  0001 C CNN
F 3 "" V 2775 6000 50  0000 C CNN
F 4 "NTS260SFT1G" H 2775 6000 60  0001 C CNN "part"
F 5 "2473419" H 2775 6000 60  0001 C CNN "farnell"
	1    2775 6000
	-1   0    0    1   
$EndComp
$Comp
L R R80
U 1 1 57010F19
P 3075 6150
F 0 "R80" H 3145 6196 50  0000 L CNN
F 1 "220k" H 3145 6105 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 3005 6150 50  0001 C CNN
F 3 "" H 3075 6150 50  0000 C CNN
F 4 "MC01W08051220K" H 3075 6150 60  0001 C CNN "part"
F 5 "9332839" H 3075 6150 60  0001 C CNN "farnell"
	1    3075 6150
	1    0    0    -1  
$EndComp
$Comp
L R R81
U 1 1 57010F4F
P 3075 6525
F 0 "R81" H 3145 6571 50  0000 L CNN
F 1 "12k" H 3145 6480 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 3005 6525 50  0001 C CNN
F 3 "" H 3075 6525 50  0000 C CNN
F 4 "MC01W0805112K" H 3075 6525 60  0001 C CNN "part"
F 5 "9332502" H 3075 6525 60  0001 C CNN "farnell"
	1    3075 6525
	1    0    0    -1  
$EndComp
$Comp
L R R82
U 1 1 57011141
P 3350 6150
F 0 "R82" H 3420 6196 50  0000 L CNN
F 1 "220k" H 3420 6105 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 3280 6150 50  0001 C CNN
F 3 "" H 3350 6150 50  0000 C CNN
F 4 "MC01W08051220K" H 3350 6150 60  0001 C CNN "part"
F 5 "9332839" H 3350 6150 60  0001 C CNN "farnell"
	1    3350 6150
	1    0    0    -1  
$EndComp
$Comp
L R R83
U 1 1 57011177
P 3350 6525
F 0 "R83" H 3420 6571 50  0000 L CNN
F 1 "10k" H 3420 6480 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 3280 6525 50  0001 C CNN
F 3 "" H 3350 6525 50  0000 C CNN
F 4 "MC01W0805110K" H 3350 6525 60  0001 C CNN "part"
F 5 "9332391" H 3350 6525 60  0001 C CNN "farnell"
	1    3350 6525
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR61
U 1 1 57011519
P 3350 6700
F 0 "#PWR61" H 3350 6500 50  0001 C CNN
F 1 "GNDPWR" H 3357 6774 50  0001 C CNN
F 2 "" H 3350 6650 50  0000 C CNN
F 3 "" H 3350 6650 50  0000 C CNN
	1    3350 6700
	1    0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR63
U 1 1 57011573
P 3950 6300
F 0 "#PWR63" H 3950 6100 50  0001 C CNN
F 1 "GNDPWR" H 3957 6374 50  0001 C CNN
F 2 "" H 3950 6250 50  0000 C CNN
F 3 "" H 3950 6250 50  0000 C CNN
	1    3950 6300
	1    0    0    -1  
$EndComp
Text Notes 2750 5650 0    60   ~ 0
Continous gate highside switch\nRegulated =24.3V\nOvervolt => 29V
$Comp
L C C37
U 1 1 57010B62
P 4150 6150
F 0 "C37" V 3898 6150 50  0000 C CNN
F 1 "100n" V 3989 6150 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H 4188 6000 50  0001 C CNN
F 3 "" H 4150 6150 50  0000 C CNN
F 4 "0805B104J500CT" V 4150 6150 60  0001 C CNN "part"
F 5 "2496940" V 4150 6150 60  0001 C CNN "farnell"
	1    4150 6150
	-1   0    0    1   
$EndComp
Text GLabel 4500 6000 2    60   Input ~ 0
gate_drive_24V
$Comp
L MC34063-RESCUE-Parkbrake U22
U 1 1 57089F7E
P 3675 2425
F 0 "U22" H 3675 2992 50  0000 C CNN
F 1 "MC34063" H 3675 2901 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 3675 2901 50  0001 C CNN
F 3 "" H 4175 2425 50  0000 C CNN
F 4 "MC34063ABD-TR" H 3675 2425 60  0001 C CNN "part"
F 5 "1087205" H 3675 2425 60  0001 C CNN "farnell"
	1    3675 2425
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR62
U 1 1 5708A002
P 3675 3150
F 0 "#PWR62" H 3675 2900 50  0001 C CNN
F 1 "GND" H 3683 2977 50  0000 C CNN
F 2 "" H 3675 3150 50  0000 C CNN
F 3 "" H 3675 3150 50  0000 C CNN
	1    3675 3150
	1    0    0    -1  
$EndComp
$Comp
L C C35
U 1 1 5708A058
P 3125 3000
F 0 "C35" H 3240 3046 50  0000 L CNN
F 1 "470p" H 3240 2955 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3163 2850 50  0001 C CNN
F 3 "" H 3125 3000 50  0000 C CNN
F 4 "0805N471J500CT" H 3125 3000 60  0001 C CNN "part"
F 5 "2497037" H 3125 3000 60  0001 C CNN "farnell"
	1    3125 3000
	1    0    0    -1  
$EndComp
$Comp
L R R84
U 1 1 5708A132
P 4075 3000
F 0 "R84" H 4145 3046 50  0000 L CNN
F 1 "1k" H 4145 2955 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 4005 3000 50  0001 C CNN
F 3 "" H 4075 3000 50  0000 C CNN
F 4 "MC01W080511K" H 4075 3000 60  0001 C CNN "part"
F 5 "9332383" H 4075 3000 60  0001 C CNN "farnell"
	1    4075 3000
	1    0    0    -1  
$EndComp
$Comp
L R R85
U 1 1 5708A176
P 4625 2725
F 0 "R85" V 4550 2725 50  0000 C CNN
F 1 "3k" V 4625 2725 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4555 2725 50  0001 C CNN
F 3 "" H 4625 2725 50  0000 C CNN
F 4 "MC01W080513K" V 4625 2725 60  0001 C CNN "part"
F 5 "9332995" V 4625 2725 60  0001 C CNN "farnell"
	1    4625 2725
	0    1    1    0   
$EndComp
$Comp
L R R79
U 1 1 5708A1CB
P 2975 2175
F 0 "R79" H 3045 2221 50  0000 L CNN
F 1 "0.150 1/2W" V 2875 2125 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 2905 2175 50  0001 C CNN
F 3 "" H 2975 2175 50  0000 C CNN
F 4 "WW12PR150FTL" H 2975 2175 60  0001 C CNN "part"
F 5 "2503066" H 2975 2175 60  0001 C CNN "farnell"
	1    2975 2175
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR_SMALL L2
U 1 1 5708A258
P 4600 2525
F 0 "L2" H 4600 2740 50  0000 C CNN
F 1 "33uH" H 4600 2649 50  0000 C CNN
F 2 "myfootprints:inductor_sd54" H 4600 2525 50  0001 C CNN
F 3 "" H 4600 2525 50  0000 C CNN
F 4 "SD54-333MLB" H 4600 2525 60  0001 C CNN "part"
F 5 "2526983" H 4600 2525 60  0001 C CNN "farnell"
	1    4600 2525
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D27
U 1 1 5708A2E4
P 4350 3000
F 0 "D27" V 4304 3079 50  0000 L CNN
F 1 "min 1.5A" V 4395 3079 50  0000 L CNN
F 2 "Diodes_SMD:SOD-123" H 4350 3000 50  0001 C CNN
F 3 "" H 4350 3000 50  0000 C CNN
F 4 "NTS260SFT1G" V 4350 3000 60  0001 C CNN "part"
F 5 "2473419" V 4350 3000 60  0001 C CNN "farnell"
	1    4350 3000
	0    1    1    0   
$EndComp
$Comp
L CP C38
U 1 1 5708AB53
P 5075 2675
F 0 "C38" H 5193 2721 50  0000 L CNN
F 1 "680u" H 5193 2630 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeD_EIA-7343_Wave" H 5113 2525 50  0001 C CNN
F 3 "" H 5075 2675 50  0000 C CNN
F 4 "TPSE687K006R0060" H 5075 2675 60  0001 C CNN "part"
F 5 "2283882" H 5075 2675 60  0001 C CNN "farnell"
	1    5075 2675
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR64
U 1 1 5708ABA3
P 5300 2850
F 0 "#PWR64" H 5300 2600 50  0001 C CNN
F 1 "GND" H 5308 2677 50  0000 C CNN
F 2 "" H 5300 2850 50  0000 C CNN
F 3 "" H 5300 2850 50  0000 C CNN
	1    5300 2850
	1    0    0    -1  
$EndComp
$Comp
L C C39
U 1 1 5708ABDB
P 5475 2675
F 0 "C39" H 5590 2721 50  0000 L CNN
F 1 "100n" H 5590 2630 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5513 2525 50  0001 C CNN
F 3 "" H 5475 2675 50  0000 C CNN
F 4 "0805B104J500CT" H 5475 2675 60  0001 C CNN "part"
F 5 "2496940" H 5475 2675 60  0001 C CNN "farnell"
	1    5475 2675
	1    0    0    -1  
$EndComp
$Comp
L C C34
U 1 1 5708B842
P 2975 2475
F 0 "C34" H 3090 2521 50  0000 L CNN
F 1 "100n" H 3090 2430 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3013 2325 50  0001 C CNN
F 3 "" H 2975 2475 50  0000 C CNN
F 4 "2496940" H 2975 2475 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 2975 2475 60  0001 C CNN "part"
	1    2975 2475
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR60
U 1 1 5708B974
P 2975 2625
F 0 "#PWR60" H 2975 2375 50  0001 C CNN
F 1 "GND" H 2983 2452 50  0000 C CNN
F 2 "" H 2975 2625 50  0000 C CNN
F 3 "" H 2975 2625 50  0000 C CNN
	1    2975 2625
	1    0    0    -1  
$EndComp
Text GLabel 2800 2325 0    60   Input ~ 0
+12V_Feed
Text GLabel 5700 2525 2    60   Input ~ 0
+5V
Text Notes 4500 2100 0    60   ~ 0
5V 700mA max
$Comp
L LD1117S33CTR U23
U 1 1 5708D2D2
P 8450 2350
F 0 "U23" H 8450 2758 50  0000 C CNN
F 1 "LD1117S33CTR" H 8450 2667 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-223" H 8450 2576 50  0001 C CNN
F 3 "" H 8450 2350 50  0000 C CNN
F 4 "LD1117S33CTR" H 8450 2350 60  0001 C CNN "part"
F 5 "1467779" H 8450 2350 60  0001 C CNN "farnell"
	1    8450 2350
	1    0    0    -1  
$EndComp
$Comp
L CP C40
U 1 1 5708D4A0
P 7875 2450
F 0 "C40" H 7990 2496 50  0000 L CNN
F 1 "10u" H 7990 2405 50  0000 L CNN
F 2 "myfootprints:CP_0805" H 7913 2300 50  0001 C CNN
F 3 "" H 7875 2450 50  0000 C CNN
F 4 "TMCP0J106KTRF" H 7875 2450 60  0001 C CNN "part"
F 5 "2491430" H 7875 2450 60  0001 C CNN "farnell"
	1    7875 2450
	1    0    0    -1  
$EndComp
$Comp
L CP C41
U 1 1 5708D4FA
P 9025 2450
F 0 "C41" H 9140 2496 50  0000 L CNN
F 1 "10u" H 9140 2405 50  0000 L CNN
F 2 "myfootprints:CP_0805" H 9063 2300 50  0001 C CNN
F 3 "" H 9025 2450 50  0000 C CNN
F 4 "TMCP0J106KTRF" H 9025 2450 60  0001 C CNN "part"
F 5 "2491430" H 9025 2450 60  0001 C CNN "farnell"
	1    9025 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR65
U 1 1 5708D552
P 8450 2775
F 0 "#PWR65" H 8450 2525 50  0001 C CNN
F 1 "GND" H 8458 2602 50  0000 C CNN
F 2 "" H 8450 2775 50  0000 C CNN
F 3 "" H 8450 2775 50  0000 C CNN
	1    8450 2775
	1    0    0    -1  
$EndComp
Wire Wire Line
	1675 5550 1675 6150
Wire Wire Line
	1675 6300 1675 6425
Wire Wire Line
	1675 6300 1425 6300
Wire Wire Line
	1675 6000 1425 6000
Wire Wire Line
	1675 5550 1875 5550
Connection ~ 1675 6000
Wire Wire Line
	2375 5550 2575 5550
Wire Wire Line
	2575 5550 2575 6000
Wire Wire Line
	3075 6300 3075 6375
Wire Wire Line
	3350 6300 3350 6375
Connection ~ 3075 6000
Connection ~ 3350 6000
Wire Wire Line
	3075 6300 2800 6300
Wire Wire Line
	2800 6300 2800 6150
Wire Wire Line
	2800 6150 2575 6150
Wire Wire Line
	3350 6350 2575 6350
Wire Wire Line
	2575 6350 2575 6300
Connection ~ 3350 6350
Wire Wire Line
	2575 6000 2675 6000
Wire Wire Line
	3350 6700 3350 6675
Wire Wire Line
	3350 6675 3075 6675
Wire Wire Line
	2875 6000 4500 6000
Connection ~ 3725 6000
Connection ~ 4150 6000
Wire Wire Line
	3725 6300 4150 6300
Connection ~ 3950 6300
Wire Wire Line
	3125 3150 4350 3150
Connection ~ 4075 3150
Wire Wire Line
	3125 2850 3125 2725
Wire Wire Line
	3125 2725 3275 2725
Wire Wire Line
	4075 2725 4075 2850
Wire Wire Line
	4075 2725 4475 2725
Wire Wire Line
	3675 2925 3675 3150
Connection ~ 3675 3150
Wire Wire Line
	4350 2525 4075 2525
Connection ~ 5075 2525
Connection ~ 5475 2525
Wire Wire Line
	5075 2825 5475 2825
Wire Wire Line
	5300 2825 5300 2850
Connection ~ 5300 2825
Wire Wire Line
	4100 2325 4075 2325
Wire Wire Line
	4100 1750 4100 2325
Wire Wire Line
	4100 2125 4075 2125
Wire Wire Line
	4100 1750 2975 1750
Wire Wire Line
	2975 1750 2975 2025
Connection ~ 4100 2125
Wire Wire Line
	3275 2125 3275 1950
Wire Wire Line
	3275 1950 2975 1950
Connection ~ 2975 1950
Wire Wire Line
	2800 2325 3275 2325
Connection ~ 2975 2325
Wire Wire Line
	4850 2725 4850 2525
Wire Wire Line
	4850 2725 4775 2725
Wire Wire Line
	4350 2850 4350 2525
Wire Wire Line
	4850 2525 5700 2525
Wire Wire Line
	7750 2300 8050 2300
Connection ~ 7875 2300
Wire Wire Line
	8850 2300 9250 2300
Wire Wire Line
	8450 2600 8450 2775
Wire Wire Line
	9025 2650 9025 2600
Wire Wire Line
	7875 2650 9025 2650
Connection ~ 8450 2650
Wire Wire Line
	7875 2650 7875 2600
Text GLabel 7750 2300 0    60   Input ~ 0
+5V
Text GLabel 9250 2300 2    60   Input ~ 0
+3.3V
$Comp
L PWR_FLAG #FLG11
U 1 1 57980F2B
P 9025 2300
F 0 "#FLG11" H 9025 2395 50  0001 C CNN
F 1 "PWR_FLAG" H 9025 2480 50  0001 C CNN
F 2 "" H 9025 2300 50  0000 C CNN
F 3 "" H 9025 2300 50  0000 C CNN
	1    9025 2300
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG10
U 1 1 5798551D
P 5075 2525
F 0 "#FLG10" H 5075 2620 50  0001 C CNN
F 1 "PWR_FLAG" H 5075 2705 50  0001 C CNN
F 2 "" H 5075 2525 50  0000 C CNN
F 3 "" H 5075 2525 50  0000 C CNN
	1    5075 2525
	1    0    0    -1  
$EndComp
$EndSCHEMATC
