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
Sheet 9 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 725  1375 0    98   ~ 0
Rules for activating parkbrake\n1. It should always be possible to activate parkbrake, even when IGN off\n2. This node shall not have power without IGN OR when pressing and   holdning parkbrake button\n3. If speed is above 7km/h it should not lock wheels, activate parkbrake only when pressing and\nholdning parkbrake for at least 0.5sek and continue to hold button pressed
Text Notes 750  2225 0    98   ~ 0
Rules for releasing parkbrake:\n1. User must have IGN on AND must press brakepedal (brakelights lit)\n2. IGN, Brakepedal, parkbarke_non_inv nust be high as well as parkbrake_inv must be low to switch on latch to power up the H-bridge.\nThis is set in in hardware. Reset of latch is done by timeout of 30sec or by microcontroller
Text GLabel 1500 2550 2    60   Input ~ 0
+12V_ign_in
Text GLabel 1525 3800 2    60   Input ~ 0
BrakeLights
Text GLabel 1525 4800 2    60   Input ~ 0
ParkbrakeSwitch_non_invert
Text GLabel 1525 5800 2    60   Input ~ 0
~ParkbrakeSwitch_inverted
$Comp
L R R?
U 1 1 578EC776
P 1100 2750
F 0 "R?" V 1180 2750 50  0000 C CNN
F 1 "680" V 1100 2750 50  0000 C CNN
F 2 "" V 1030 2750 50  0000 C CNN
F 3 "" H 1100 2750 50  0000 C CNN
F 4 "MC01W08051680R" V 1100 2750 60  0001 C CNN "part"
F 5 "9333460" V 1100 2750 60  0001 C CNN "farnell"
	1    1100 2750
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 578EC797
P 1100 3075
F 0 "R?" V 1180 3075 50  0000 C CNN
F 1 "330" V 1100 3075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1030 3075 50  0001 C CNN
F 3 "" H 1100 3075 50  0000 C CNN
F 4 "MC01W08051330R" V 1100 3075 60  0001 C CNN "part"
F 5 "9333037" V 1100 3075 60  0001 C CNN "farnell"
	1    1100 3075
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 578EC7FC
P 850 3075
F 0 "D?" H 850 3175 50  0000 C CNN
F 1 "3.3V" H 850 2975 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 850 3075 50  0001 C CNN
F 3 "" H 850 3075 50  0000 C CNN
F 4 "MMSZ4684" H 850 3075 60  0001 C CNN "part"
F 5 "2454044" H 850 3075 60  0001 C CNN "farnell"
	1    850  3075
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 578EC84E
P 850 3225
F 0 "#PWR?" H 850 2975 50  0001 C CNN
F 1 "GND" H 850 3075 50  0000 C CNN
F 2 "" H 850 3225 50  0000 C CNN
F 3 "" H 850 3225 50  0000 C CNN
	1    850  3225
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  3225 1325 3225
Wire Wire Line
	850  3225 850  3175
Wire Wire Line
	1100 2900 1100 2925
Wire Wire Line
	850  2925 1325 2925
Wire Wire Line
	850  2925 850  2975
$Comp
L C C?
U 1 1 578EC8D5
P 1325 3075
F 0 "C?" H 1475 3100 50  0000 L CNN
F 1 "100n" H 1350 2975 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1363 2925 50  0001 C CNN
F 3 "" H 1325 3075 50  0000 C CNN
F 4 "2496940" H 1325 3075 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 1325 3075 60  0001 C CNN "part"
	1    1325 3075
	1    0    0    -1  
$EndComp
Connection ~ 1100 2925
Connection ~ 1100 3225
Text Notes 750  3600 0    60   ~ 0
cap adds about 15-25ms\nsmoothing delay
Text GLabel 1325 2925 2    60   Input ~ 0
IGN_ON_SIGNAL
$Comp
L R R?
U 1 1 578ECEFF
P 1100 4000
F 0 "R?" V 1180 4000 50  0000 C CNN
F 1 "680" V 1100 4000 50  0000 C CNN
F 2 "" V 1030 4000 50  0000 C CNN
F 3 "" H 1100 4000 50  0000 C CNN
F 4 "MC01W08051680R" V 1100 4000 60  0001 C CNN "part"
F 5 "9333460" V 1100 4000 60  0001 C CNN "farnell"
	1    1100 4000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 578ECF05
P 1100 4325
F 0 "R?" V 1180 4325 50  0000 C CNN
F 1 "330" V 1100 4325 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1030 4325 50  0001 C CNN
F 3 "" H 1100 4325 50  0000 C CNN
F 4 "9333037" V 1100 4325 60  0001 C CNN "farnell"
F 5 "MC01W08051330R" V 1100 4325 60  0001 C CNN "part"
	1    1100 4325
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 578ECF0B
P 850 4325
F 0 "D?" H 850 4425 50  0000 C CNN
F 1 "3.3V" H 850 4225 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 850 4325 50  0001 C CNN
F 3 "" H 850 4325 50  0000 C CNN
F 4 "MMSZ4684" H 850 4325 60  0001 C CNN "part"
F 5 "2454044" H 850 4325 60  0001 C CNN "farnell"
	1    850  4325
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 578ECF11
P 850 4475
F 0 "#PWR?" H 850 4225 50  0001 C CNN
F 1 "GND" H 850 4325 50  0000 C CNN
F 2 "" H 850 4475 50  0000 C CNN
F 3 "" H 850 4475 50  0000 C CNN
	1    850  4475
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  4475 1325 4475
Wire Wire Line
	850  4475 850  4425
Wire Wire Line
	1100 4150 1100 4175
Wire Wire Line
	850  4175 1325 4175
Wire Wire Line
	850  4175 850  4225
$Comp
L C C?
U 1 1 578ECF1C
P 1325 4325
F 0 "C?" H 1450 4325 50  0000 L CNN
F 1 "100n" H 1350 4225 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1363 4175 50  0001 C CNN
F 3 "" H 1325 4325 50  0000 C CNN
F 4 "2496940" H 1325 4325 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 1325 4325 60  0001 C CNN "part"
	1    1325 4325
	1    0    0    -1  
$EndComp
Connection ~ 1100 4175
Connection ~ 1100 4475
Text GLabel 1325 4175 2    60   Input ~ 0
BRAKELIGHTS_ON_SIGNAL
$Comp
L R R?
U 1 1 578ED090
P 1100 5000
F 0 "R?" V 1180 5000 50  0000 C CNN
F 1 "680" V 1100 5000 50  0000 C CNN
F 2 "" V 1030 5000 50  0000 C CNN
F 3 "" H 1100 5000 50  0000 C CNN
F 4 "MC01W08051680R" V 1100 5000 60  0001 C CNN "part"
F 5 "9333460" V 1100 5000 60  0001 C CNN "farnell"
	1    1100 5000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 578ED096
P 1100 5325
F 0 "R?" V 1180 5325 50  0000 C CNN
F 1 "330" V 1100 5325 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1030 5325 50  0001 C CNN
F 3 "" H 1100 5325 50  0000 C CNN
F 4 "9333037" V 1100 5325 60  0001 C CNN "farnell"
F 5 "MC01W08051330R" V 1100 5325 60  0001 C CNN "part"
	1    1100 5325
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 578ED09C
P 850 5325
F 0 "D?" H 850 5425 50  0000 C CNN
F 1 "3.3V" H 850 5225 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 850 5325 50  0001 C CNN
F 3 "" H 850 5325 50  0000 C CNN
F 4 "MMSZ4684" H 850 5325 60  0001 C CNN "part"
F 5 "2454044" H 850 5325 60  0001 C CNN "farnell"
	1    850  5325
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 578ED0A2
P 850 5475
F 0 "#PWR?" H 850 5225 50  0001 C CNN
F 1 "GND" H 850 5325 50  0000 C CNN
F 2 "" H 850 5475 50  0000 C CNN
F 3 "" H 850 5475 50  0000 C CNN
	1    850  5475
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  5475 1325 5475
Wire Wire Line
	850  5475 850  5425
Wire Wire Line
	1100 5150 1100 5175
Wire Wire Line
	850  5175 1325 5175
Wire Wire Line
	850  5175 850  5225
$Comp
L C C?
U 1 1 578ED0AD
P 1325 5325
F 0 "C?" H 1450 5325 50  0000 L CNN
F 1 "100n" H 1350 5225 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1363 5175 50  0001 C CNN
F 3 "" H 1325 5325 50  0000 C CNN
F 4 "2496940" H 1325 5325 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 1325 5325 60  0001 C CNN "part"
	1    1325 5325
	1    0    0    -1  
$EndComp
Connection ~ 1100 5175
Connection ~ 1100 5475
Text GLabel 1325 5175 2    60   Input ~ 0
PARKBRAKE_NON_INVERT_SIGNAL
$Comp
L R R?
U 1 1 578ED236
P 1100 6000
F 0 "R?" V 1180 6000 50  0000 C CNN
F 1 "680" V 1100 6000 50  0000 C CNN
F 2 "" V 1030 6000 50  0000 C CNN
F 3 "" H 1100 6000 50  0000 C CNN
F 4 "MC01W08051680R" V 1100 6000 60  0001 C CNN "part"
F 5 "9333460" V 1100 6000 60  0001 C CNN "farnell"
	1    1100 6000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 578ED23C
P 1100 6325
F 0 "R?" V 1180 6325 50  0000 C CNN
F 1 "330" V 1100 6325 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1030 6325 50  0001 C CNN
F 3 "" H 1100 6325 50  0000 C CNN
F 4 "9333037" V 1100 6325 60  0001 C CNN "farnell"
F 5 "MC01W08051330R" V 1100 6325 60  0001 C CNN "part"
	1    1100 6325
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D?
U 1 1 578ED242
P 850 6325
F 0 "D?" H 850 6425 50  0000 C CNN
F 1 "3.3V" H 850 6225 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 850 6325 50  0001 C CNN
F 3 "" H 850 6325 50  0000 C CNN
F 4 "MMSZ4684" H 850 6325 60  0001 C CNN "part"
F 5 "2454044" H 850 6325 60  0001 C CNN "farnell"
	1    850  6325
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 578ED248
P 850 6475
F 0 "#PWR?" H 850 6225 50  0001 C CNN
F 1 "GND" H 850 6325 50  0000 C CNN
F 2 "" H 850 6475 50  0000 C CNN
F 3 "" H 850 6475 50  0000 C CNN
	1    850  6475
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  6475 1325 6475
Wire Wire Line
	850  6475 850  6425
Wire Wire Line
	1100 6150 1100 6175
Wire Wire Line
	850  6175 1575 6175
Wire Wire Line
	850  6175 850  6225
$Comp
L C C?
U 1 1 578ED253
P 1325 6325
F 0 "C?" H 1350 6425 50  0000 L CNN
F 1 "100n" H 1350 6225 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1363 6175 50  0001 C CNN
F 3 "" H 1325 6325 50  0000 C CNN
F 4 "2496940" H 1325 6325 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 1325 6325 60  0001 C CNN "part"
	1    1325 6325
	1    0    0    -1  
$EndComp
Connection ~ 1100 6175
Connection ~ 1100 6475
Text GLabel 1575 6175 2    60   Input ~ 0
~PARKBRAKE_INVERT_SIGNAL
$Comp
L R R?
U 1 1 578EE014
P 1775 6775
F 0 "R?" V 1855 6775 50  0000 C CNN
F 1 "100k" V 1775 6775 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 1705 6775 50  0001 C CNN
F 3 "" H 1775 6775 50  0000 C CNN
F 4 "MC01W08051100K" V 1775 6775 60  0001 C CNN "part"
F 5 "9332405" V 1775 6775 60  0001 C CNN "farnell"
	1    1775 6775
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q?
U 1 1 578EE0C9
P 2025 6950
F 0 "Q?" H 2325 7000 50  0000 R CNN
F 1 "BSS308" H 2500 6875 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 2225 7050 50  0001 C CNN
F 3 "" H 2025 6950 50  0000 C CNN
F 4 "BSS308PE H6327" H 2025 6950 60  0001 C CNN "part"
F 5 "2432719" H 2025 6950 60  0001 C CNN "farnell"
	1    2025 6950
	1    0    0    1   
$EndComp
$Comp
L R R?
U 1 1 578EE1FC
P 2125 7350
F 0 "R?" V 2205 7350 50  0000 C CNN
F 1 "10k" V 2125 7350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2055 7350 50  0001 C CNN
F 3 "" H 2125 7350 50  0000 C CNN
F 4 "MC01W0805110K" V 2125 7350 60  0001 C CNN "part"
F 5 "9332391" V 2125 7350 60  0001 C CNN "farnell"
	1    2125 7350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 578EE3D9
P 2125 7500
F 0 "#PWR?" H 2125 7250 50  0001 C CNN
F 1 "GND" H 2125 7350 50  0000 C CNN
F 2 "" H 2125 7500 50  0000 C CNN
F 3 "" H 2125 7500 50  0000 C CNN
	1    2125 7500
	1    0    0    -1  
$EndComp
$Comp
L D D?
U 1 1 578EE4B3
P 1575 6775
F 0 "D?" H 1575 6875 50  0000 C CNN
F 1 "1n4148w" H 1575 6675 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 1575 6775 50  0001 C CNN
F 3 "" H 1575 6775 50  0000 C CNN
F 4 "1N4148W-E3-08" H 1575 6775 60  0001 C CNN "part"
F 5 "2433353" H 1575 6775 60  0001 C CNN "farnell"
	1    1575 6775
	0    1    1    0   
$EndComp
Wire Wire Line
	2125 7150 2125 7200
Wire Wire Line
	2125 7200 2225 7200
Wire Wire Line
	1575 6950 1825 6950
Wire Wire Line
	1775 6925 1775 6950
Connection ~ 1775 6950
Wire Wire Line
	2125 6750 2125 6600
Wire Wire Line
	2125 6600 1775 6600
Wire Wire Line
	1775 6600 1775 6625
Text GLabel 2125 6600 2    60   Input ~ 0
+3.3V
Text GLabel 2225 7200 2    60   Input ~ 0
PARKBRAKE_INVERT_CHECK_SIGNAL
Wire Wire Line
	1575 6925 1575 6950
Wire Wire Line
	1575 6175 1575 6625
Connection ~ 1325 6175
Text Notes 650  7450 0    60   ~ 0
Transistor circuit checks that\ninv cable isnt open\nMust ground 100k current
Text GLabel 4275 2875 0    60   Input ~ 0
IGN_ON_SIGNAL
Text GLabel 4275 3075 0    60   Input ~ 0
BRAKELIGHTS_ON_SIGNAL
Text GLabel 4275 3425 0    60   Input ~ 0
PARKBRAKE_NON_INV_SIGNAL
Text GLabel 4275 3650 0    60   Input ~ 0
PARKBRAKE_INVERT_CHECK_SIGNAL
$Comp
L D D?
U 1 1 578F186F
P 1350 2550
F 0 "D?" H 1350 2450 50  0000 C CNN
F 1 "1n4148w" H 1300 2675 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 1350 2550 50  0001 C CNN
F 3 "" H 1350 2550 50  0000 C CNN
F 4 "1N4148W-E3-08" H 1350 2550 60  0001 C CNN "part"
F 5 "2433353" H 1350 2550 60  0001 C CNN "farnell"
	1    1350 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 2550 1100 2550
Wire Wire Line
	1100 2550 1100 2600
$Comp
L D D?
U 1 1 578F1DCD
P 1375 3800
F 0 "D?" H 1350 3700 50  0000 C CNN
F 1 "1n4148w" H 1350 3900 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 1375 3800 50  0001 C CNN
F 3 "" H 1375 3800 50  0000 C CNN
F 4 "1N4148W-E3-08" H 1375 3800 60  0001 C CNN "part"
F 5 "2433353" H 1375 3800 60  0001 C CNN "farnell"
	1    1375 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1225 3800 1100 3800
Wire Wire Line
	1100 3800 1100 3850
$Comp
L D D?
U 1 1 578F2202
P 1375 4800
F 0 "D?" H 1375 4675 50  0000 C CNN
F 1 "1n4148w" H 1425 4925 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 1375 4800 50  0001 C CNN
F 3 "" H 1375 4800 50  0000 C CNN
F 4 "1N4148W-E3-08" H 1375 4800 60  0001 C CNN "part"
F 5 "2433353" H 1375 4800 60  0001 C CNN "farnell"
	1    1375 4800
	1    0    0    -1  
$EndComp
$Comp
L D D?
U 1 1 578F22F7
P 1375 5800
F 0 "D?" H 1375 5675 50  0000 C CNN
F 1 "1n4148w" H 1375 5925 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 1375 5800 50  0001 C CNN
F 3 "" H 1375 5800 50  0000 C CNN
F 4 "1N4148W-E3-08" H 1375 5800 60  0001 C CNN "part"
F 5 "2433353" H 1375 5800 60  0001 C CNN "farnell"
	1    1375 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1225 5800 1100 5800
Wire Wire Line
	1100 5800 1100 5850
Wire Wire Line
	1225 4800 1100 4800
Wire Wire Line
	1100 4800 1100 4850
$Comp
L R R?
U 1 1 578F351B
P 5975 3250
F 0 "R?" V 6055 3250 50  0000 C CNN
F 1 "1M" V 5975 3250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5905 3250 50  0001 C CNN
F 3 "" H 5975 3250 50  0000 C CNN
F 4 "MC01W080511M" V 5975 3250 60  0001 C CNN "part"
F 5 "9332413" V 5975 3250 60  0001 C CNN "farnell"
	1    5975 3250
	0    -1   -1   0   
$EndComp
$Comp
L D D?
U 1 1 578F3ABC
P 5975 3450
F 0 "D?" H 5975 3550 50  0000 C CNN
F 1 "1n4148" H 6025 3350 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 5975 3450 50  0001 C CNN
F 3 "" H 5975 3450 50  0000 C CNN
F 4 "2433353" H 5975 3450 60  0001 C CNN "farnell"
F 5 "1N4148W-E3-08" H 5975 3450 60  0001 C CNN "part"
	1    5975 3450
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 578F3B8F
P 6125 3100
F 0 "C?" H 6150 3200 50  0000 L CNN
F 1 "33uF" H 6150 3000 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_4x5.3" H 6163 2950 50  0001 C CNN
F 3 "" H 6125 3100 50  0000 C CNN
F 4 "2069144" H 6125 3100 60  0001 C CNN "farnell"
F 5 "EXV336M6R3A9BAA" H 6125 3100 60  0001 C CNN "part"
	1    6125 3100
	1    0    0    -1  
$EndComp
Text GLabel 7625 3350 2    60   Input ~ 0
SAFE_TO_RELEASE
Wire Wire Line
	4625 3200 4400 3200
Wire Wire Line
	4400 3200 4400 3075
Wire Wire Line
	4400 3075 4275 3075
Wire Wire Line
	4625 3300 4400 3300
Wire Wire Line
	4400 3300 4400 3425
Wire Wire Line
	4400 3425 4275 3425
Wire Wire Line
	4625 3400 4500 3400
Wire Wire Line
	4500 3400 4500 3650
Wire Wire Line
	4500 3650 4275 3650
Wire Wire Line
	4625 3100 4500 3100
Wire Wire Line
	4500 3100 4500 2875
Wire Wire Line
	4500 2875 4275 2875
$Comp
L 74LS132 U?
U 1 1 578F5AE6
P 7025 3350
F 0 "U?" H 7025 3450 50  0000 C CNN
F 1 "SN74HC132" H 7025 3250 50  0000 C CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 7025 3350 50  0001 C CNN
F 3 "" H 7025 3350 50  0000 C CNN
F 4 "SN74HC132PWR" H 7025 3350 60  0001 C CNN "part"
	1    7025 3350
	1    0    0    -1  
$EndComp
$Comp
L 74LS132 U?
U 2 1 578F5B9B
P 7075 4550
F 0 "U?" H 7075 4650 50  0000 C CNN
F 1 "SN74HC132" H 7075 4450 50  0000 C CNN
F 2 "" H 7075 4550 50  0000 C CNN
F 3 "" H 7075 4550 50  0000 C CNN
	2    7075 4550
	1    0    0    -1  
$EndComp
$Comp
L 74HCT02 U?
U 1 1 578F6954
P 5225 4400
F 0 "U?" H 5225 4450 50  0000 C CNN
F 1 "SN74HC02" H 5275 4350 50  0000 C CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 5225 4400 50  0001 C CNN
F 3 "" H 5225 4400 50  0000 C CNN
F 4 "SN74HC02PWR" H 5225 4400 60  0001 C CNN "part"
F 5 "9590927" H 5225 4400 60  0001 C CNN "farnell"
	1    5225 4400
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 578F6AA0
P 5975 4400
F 0 "R?" V 5875 4325 50  0000 C CNN
F 1 "1M" V 5975 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5905 4400 50  0001 C CNN
F 3 "" H 5975 4400 50  0000 C CNN
F 4 "MC01W080511M" V 5975 4400 60  0001 C CNN "part"
F 5 "9332413" V 5975 4400 60  0001 C CNN "farnell"
	1    5975 4400
	0    1    1    0   
$EndComp
$Comp
L CP C?
U 1 1 578F6B3F
P 6125 4250
F 0 "C?" H 6150 4350 50  0000 L CNN
F 1 "33uF" H 6150 4150 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_4x5.3" H 6163 4100 50  0001 C CNN
F 3 "" H 6125 4250 50  0000 C CNN
F 4 "EXV336M6R3A9BAA" H 6125 4250 60  0001 C CNN "part"
F 5 "2069144" H 6125 4250 60  0001 C CNN "farnell"
	1    6125 4250
	1    0    0    -1  
$EndComp
Text GLabel 4625 4300 0    60   Input ~ 0
PARKBRAKE_NON_INVERT_SIGNAL
Text GLabel 4625 4500 0    60   Input ~ 0
PARKBRAKE_INVERT_CHECK_SIGNAL
$Comp
L D D?
U 1 1 578F7186
P 5975 4650
F 0 "D?" H 5975 4750 50  0000 C CNN
F 1 "1n4148w" H 5975 4550 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 5975 4650 50  0001 C CNN
F 3 "" H 5975 4650 50  0000 C CNN
F 4 "2433353" H 5975 4650 60  0001 C CNN "farnell"
F 5 "1N4148W-E3-08" H 5975 4650 60  0001 C CNN "part"
	1    5975 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6125 4400 6125 4650
Wire Wire Line
	5825 4400 5825 4650
Text GLabel 6125 4100 1    60   Input ~ 0
3.3V
Wire Wire Line
	5825 3250 5825 3450
Wire Wire Line
	6125 3250 6125 3450
Text GLabel 6125 2950 1    60   Input ~ 0
3.3V
Wire Wire Line
	6425 3250 6425 3450
Wire Wire Line
	6425 3450 6125 3450
Wire Wire Line
	6475 4450 6475 4650
Wire Wire Line
	6475 4650 6125 4650
Text GLabel 7675 4225 2    60   Input ~ 0
TIGHTEN_CMD
Text Notes 6325 4025 0    60   ~ 0
Timing circuit gives around 30sec\nactivation after conditions stops
Text Notes 6375 2950 0    60   ~ 0
Timing circuit gives around 30sec activation\nafter conditions stops
$Comp
L 74HCT02 U?
U 2 1 578F935D
P 9350 4450
F 0 "U?" H 9350 4500 50  0000 C CNN
F 1 "SN74HC02" H 9400 4400 50  0000 C CNN
F 2 "" H 9350 4450 50  0000 C CNN
F 3 "" H 9350 4450 50  0000 C CNN
	2    9350 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7625 3350 7625 3700
Wire Wire Line
	7625 3700 8750 3700
Wire Wire Line
	8750 3700 8750 4350
Wire Wire Line
	8750 4550 7675 4550
Wire Wire Line
	7675 4550 7675 4225
$Comp
L 74HCT02 U?
U 3 1 578F98E6
P 10050 3850
F 0 "U?" H 10050 3900 50  0000 C CNN
F 1 "SN74HC02" H 10100 3800 50  0000 C CNN
F 2 "" H 10050 3850 50  0000 C CNN
F 3 "" H 10050 3850 50  0000 C CNN
	3    10050 3850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10150 4450 10150 4975
Wire Wire Line
	10150 4975 9950 4975
Text GLabel 10050 3250 1    60   Input ~ 0
Enable_Bridge_PWR
Text GLabel 7550 5150 0    60   Input ~ 0
~uC_SET_POWER
$Comp
L 74LS132 U?
U 3 1 578FA699
P 9350 4975
F 0 "U?" H 9350 5075 50  0000 C CNN
F 1 "SN74HC132" H 9350 4875 50  0000 C CNN
F 2 "" H 9350 4975 50  0000 C CNN
F 3 "" H 9350 4975 50  0000 C CNN
	3    9350 4975
	1    0    0    -1  
$EndComp
Text GLabel 8750 4875 0    60   Input ~ 0
12V_IGN_ON_SIGNAL
Text Notes 3450 5275 0    60   ~ 0
This complicated hardware logic safegards against \ncluesless idiotic software releases or tightens parkbrake\nwithout IGN on AND pressing button
$Comp
L 74LS132 U?
U 4 1 578FAEB9
P 8150 5250
F 0 "U?" H 8150 5350 50  0000 C CNN
F 1 "SN74HC132" H 8150 5150 50  0000 C CNN
F 2 "" H 8150 5250 50  0000 C CNN
F 3 "" H 8150 5250 50  0000 C CNN
	4    8150 5250
	1    0    0    -1  
$EndComp
Text GLabel 7550 5350 0    60   Input ~ 0
~PARKBRAKE_INVERT_SIGNAL
Wire Wire Line
	8750 5075 8750 5250
Text Notes 7775 5725 0    60   ~ 0
These 2 nands are there purly during diagnosis\nTo make activaet Test work (must press button and IGN on !)
$Comp
L C C?
U 1 1 578FCBAA
P 4125 6050
F 0 "C?" H 4150 6150 50  0000 L CNN
F 1 "100n" H 4150 5950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4163 5900 50  0001 C CNN
F 3 "" H 4125 6050 50  0000 C CNN
F 4 "2496940" H 4125 6050 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 4125 6050 60  0001 C CNN "part"
	1    4125 6050
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 578FCC2D
P 4400 6050
F 0 "C?" H 4425 6150 50  0000 L CNN
F 1 "100n" H 4425 5950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4438 5900 50  0001 C CNN
F 3 "" H 4400 6050 50  0000 C CNN
F 4 "2496940" H 4400 6050 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 4400 6050 60  0001 C CNN "part"
	1    4400 6050
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 578FCC92
P 4675 6050
F 0 "C?" H 4700 6150 50  0000 L CNN
F 1 "100n" H 4700 5950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4713 5900 50  0001 C CNN
F 3 "" H 4675 6050 50  0000 C CNN
F 4 "2496940" H 4675 6050 60  0001 C CNN "farnell"
F 5 "0805B104J500CT" H 4675 6050 60  0001 C CNN "part"
	1    4675 6050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 578FCD51
P 4400 6300
F 0 "#PWR?" H 4400 6050 50  0001 C CNN
F 1 "GND" H 4400 6150 50  0000 C CNN
F 2 "" H 4400 6300 50  0000 C CNN
F 3 "" H 4400 6300 50  0000 C CNN
	1    4400 6300
	1    0    0    -1  
$EndComp
Text GLabel 4400 5900 1    60   Input ~ 0
3.3V
Wire Wire Line
	4050 5900 4675 5900
Connection ~ 4400 5900
Connection ~ 4125 5900
Wire Wire Line
	4050 6200 4675 6200
Connection ~ 4400 6200
Wire Wire Line
	4400 6200 4400 6300
$Comp
L TEST_1P W?
U 1 1 578FEB95
P 6200 7200
F 0 "W?" H 6200 7470 50  0000 C CNN
F 1 "TP_Tighten_cmd" H 6200 7400 50  0000 C CNN
F 2 "" H 6400 7200 50  0000 C CNN
F 3 "" H 6400 7200 50  0000 C CNN
	1    6200 7200
	-1   0    0    1   
$EndComp
$Comp
L TEST_1P W?
U 1 1 578FED32
P 5300 7200
F 0 "W?" H 5300 7470 50  0000 C CNN
F 1 "TP_Safe_to_release" H 5300 7400 50  0000 C CNN
F 2 "" H 5500 7200 50  0000 C CNN
F 3 "" H 5500 7200 50  0000 C CNN
	1    5300 7200
	-1   0    0    1   
$EndComp
Text GLabel 5300 7200 1    60   Input ~ 0
SAFE_TO_RELEASE
Text GLabel 6200 7200 1    60   Input ~ 0
TIGHTEN_CMD
Text Label 4050 5900 2    60   ~ 0
VDD
Text Label 4050 6200 2    60   ~ 0
VSS
Connection ~ 4125 6200
$Comp
L 74LS20 U?
U 1 1 579080A4
P 5225 3250
F 0 "U?" H 5225 3350 50  0000 C CNN
F 1 "SN74HC20" H 5225 3150 50  0000 C CNN
F 2 "Housings_SSOP:TSSOP-14_4.4x5mm_Pitch0.65mm" H 5225 3250 50  0001 C CNN
F 3 "" H 5225 3250 50  0000 C CNN
F 4 "SN74HC20PW" H 5225 3250 60  0001 C CNN "part"
F 5 "1741207" H 5225 3250 60  0001 C CNN "farnell"
	1    5225 3250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
