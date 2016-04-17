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
Sheet 6 6
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
L R R?
U 1 1 570A4002
P 1950 2525
F 0 "R?" H 2020 2571 50  0000 L CNN
F 1 "10k" H 2020 2480 50  0000 L CNN
F 2 "" V 1880 2525 50  0000 C CNN
F 3 "" H 1950 2525 50  0000 C CNN
	1    1950 2525
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 570A4038
P 1625 2900
F 0 "C?" H 1740 2946 50  0000 L CNN
F 1 "100n" H 1740 2855 50  0000 L CNN
F 2 "" H 1663 2750 50  0000 C CNN
F 3 "" H 1625 2900 50  0000 C CNN
	1    1625 2900
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 570A407E
P 1950 2900
F 0 "R?" H 2020 2946 50  0000 L CNN
F 1 "1k" H 2020 2855 50  0000 L CNN
F 2 "" V 1880 2900 50  0000 C CNN
F 3 "" H 1950 2900 50  0000 C CNN
	1    1950 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 570A414C
P 1950 3125
F 0 "#PWR?" H 1950 2875 50  0001 C CNN
F 1 "GND" H 1958 2952 50  0001 C CNN
F 2 "" H 1950 3125 50  0000 C CNN
F 3 "" H 1950 3125 50  0000 C CNN
	1    1950 3125
	1    0    0    -1  
$EndComp
Text GLabel 1950 2375 1    60   Input ~ 0
+12V_power
Wire Wire Line
	1950 2675 1950 2750
Wire Wire Line
	1625 2750 2250 2750
Wire Wire Line
	1625 3050 2250 3050
Wire Wire Line
	1950 3050 1950 3125
$Comp
L ZENERsmall D?
U 1 1 570A42FD
P 2250 2900
F 0 "D?" V 2204 2969 50  0000 L CNN
F 1 "3.3V" V 2295 2969 50  0000 L CNN
F 2 "" H 2250 2900 50  0000 C CNN
F 3 "" H 2250 2900 50  0000 C CNN
	1    2250 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 2750 2250 2800
Connection ~ 1950 2750
Wire Wire Line
	2250 3050 2250 3000
Connection ~ 1950 3050
Text Label 2250 2750 0    60   ~ 0
bat_voltage_sense
Text Notes 2200 2475 0    60   ~ 0
12V->1.2V \n24V->2.4V\nmax around 30v
$EndSCHEMATC
