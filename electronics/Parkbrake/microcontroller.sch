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
Sheet 6 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 7875 5975 1    60   Input ~ 0
3.3V
$Comp
L C C?
U 1 1 57915417
P 7875 6175
F 0 "C?" H 7900 6275 50  0000 L CNN
F 1 "100n" H 7900 6075 50  0000 L CNN
F 2 "" H 7913 6025 50  0000 C CNN
F 3 "" H 7875 6175 50  0000 C CNN
	1    7875 6175
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57915474
P 8125 6175
F 0 "C?" H 8150 6275 50  0000 L CNN
F 1 "100n" H 8150 6075 50  0000 L CNN
F 2 "" H 8163 6025 50  0000 C CNN
F 3 "" H 8125 6175 50  0000 C CNN
	1    8125 6175
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579154AB
P 7600 6175
F 0 "C?" H 7625 6275 50  0000 L CNN
F 1 "4.7u" H 7625 6075 50  0000 L CNN
F 2 "" H 7638 6025 50  0000 C CNN
F 3 "" H 7600 6175 50  0000 C CNN
	1    7600 6175
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579154E4
P 8675 6175
F 0 "C?" H 8700 6275 50  0000 L CNN
F 1 "10n" H 8700 6075 50  0000 L CNN
F 2 "" H 8713 6025 50  0000 C CNN
F 3 "" H 8675 6175 50  0000 C CNN
	1    8675 6175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57915825
P 7875 6325
F 0 "#PWR?" H 7875 6075 50  0001 C CNN
F 1 "GND" H 7875 6175 50  0001 C CNN
F 2 "" H 7875 6325 50  0000 C CNN
F 3 "" H 7875 6325 50  0000 C CNN
	1    7875 6325
	1    0    0    -1  
$EndComp
Connection ~ 7600 6025
$Comp
L C C?
U 1 1 57915C80
P 8925 6175
F 0 "C?" H 8950 6275 50  0000 L CNN
F 1 "1u" H 8950 6075 50  0000 L CNN
F 2 "" H 8963 6025 50  0000 C CNN
F 3 "" H 8925 6175 50  0000 C CNN
	1    8925 6175
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57915CD6
P 9450 6175
F 0 "C?" H 9475 6275 50  0000 L CNN
F 1 "10n" H 9475 6075 50  0000 L CNN
F 2 "" H 9488 6025 50  0000 C CNN
F 3 "" H 9450 6175 50  0000 C CNN
	1    9450 6175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57915D33
P 8925 6325
F 0 "#PWR?" H 8925 6075 50  0001 C CNN
F 1 "GND" H 8925 6175 50  0001 C CNN
F 2 "" H 8925 6325 50  0000 C CNN
F 3 "" H 8925 6325 50  0000 C CNN
	1    8925 6325
	1    0    0    -1  
$EndComp
Connection ~ 8925 6325
Wire Wire Line
	8925 6325 8675 6325
$Comp
L C C?
U 1 1 5791614E
P 9700 6175
F 0 "C?" H 9725 6275 50  0000 L CNN
F 1 "1u" H 9725 6075 50  0000 L CNN
F 2 "" H 9738 6025 50  0000 C CNN
F 3 "" H 9700 6175 50  0000 C CNN
	1    9700 6175
	1    0    0    -1  
$EndComp
Text Label 5725 800  1    60   ~ 0
VDD_2
Text Label 5825 800  1    60   ~ 0
VDD_1
Text Label 5925 800  1    60   ~ 0
VDDA
Text Label 6025 800  1    60   ~ 0
VDDSD
Text Label 5825 5200 3    60   ~ 0
VSS_1
Text Label 7500 6325 2    60   ~ 0
VSS_1
Text Label 7500 6025 2    60   ~ 0
VDD_1
Wire Wire Line
	7500 6325 8125 6325
Connection ~ 7875 6325
Connection ~ 7600 6325
Text Notes 7175 6500 0    60   ~ 0
4.7u near pin 47-48
Wire Wire Line
	7500 6025 7875 6025
Text Label 8125 5950 1    60   ~ 0
VDD_2
Wire Wire Line
	7875 6025 7875 5975
Wire Wire Line
	7875 5975 8125 5975
Wire Wire Line
	8125 5975 8125 6025
Text Label 5925 5200 3    60   ~ 0
VSSA
Text Label 6025 5200 3    60   ~ 0
VSSSD
Wire Wire Line
	8675 6025 8925 6025
Text Label 8675 6025 2    60   ~ 0
VDDSD
Text Label 8675 6325 2    60   ~ 0
VSSSD
Text GLabel 8925 6025 1    60   Input ~ 0
3.3V
$Comp
L GND #PWR?
U 1 1 57918611
P 9700 6325
F 0 "#PWR?" H 9700 6075 50  0001 C CNN
F 1 "GND" H 9700 6175 50  0001 C CNN
F 2 "" H 9700 6325 50  0000 C CNN
F 3 "" H 9700 6325 50  0000 C CNN
	1    9700 6325
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 6325 9450 6325
Wire Wire Line
	9700 6025 9450 6025
Text GLabel 9700 6025 1    60   Input ~ 0
3.3V
Text Label 9450 6025 1    60   ~ 0
VREFSD+
Text Label 9450 6325 2    60   ~ 0
VSSSD
$Comp
L C C?
U 1 1 5791910A
P 10725 6175
F 0 "C?" H 10750 6275 50  0000 L CNN
F 1 "10n" H 10750 6075 50  0000 L CNN
F 2 "" H 10763 6025 50  0000 C CNN
F 3 "" H 10725 6175 50  0000 C CNN
	1    10725 6175
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57919110
P 10975 6175
F 0 "C?" H 11000 6275 50  0000 L CNN
F 1 "1u" H 11000 6075 50  0000 L CNN
F 2 "" H 11013 6025 50  0000 C CNN
F 3 "" H 10975 6175 50  0000 C CNN
	1    10975 6175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57919116
P 10975 6325
F 0 "#PWR?" H 10975 6075 50  0001 C CNN
F 1 "GND" H 10975 6175 50  0001 C CNN
F 2 "" H 10975 6325 50  0000 C CNN
F 3 "" H 10975 6325 50  0000 C CNN
	1    10975 6325
	1    0    0    -1  
$EndComp
Wire Wire Line
	10975 6325 10725 6325
Wire Wire Line
	10975 6025 10725 6025
Text GLabel 10975 6025 1    60   Input ~ 0
3.3V
Text Label 10725 6025 1    60   ~ 0
VDDA
Text Label 10725 6325 2    60   ~ 0
VSSA
Text Label 1375 1800 2    60   ~ 0
VREFSD+
$Comp
L C C?
U 1 1 5791A5CD
P 6000 7375
F 0 "C?" H 6025 7475 50  0000 L CNN
F 1 "26p" H 6025 7275 50  0000 L CNN
F 2 "" H 6038 7225 50  0000 C CNN
F 3 "" H 6000 7375 50  0000 C CNN
	1    6000 7375
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5791A6BF
P 6400 7375
F 0 "C?" H 6425 7475 50  0000 L CNN
F 1 "26p" H 6425 7275 50  0000 L CNN
F 2 "" H 6438 7225 50  0000 C CNN
F 3 "" H 6400 7375 50  0000 C CNN
	1    6400 7375
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5791A808
P 6000 7525
F 0 "#PWR?" H 6000 7275 50  0001 C CNN
F 1 "GND" H 6000 7375 50  0001 C CNN
F 2 "" H 6000 7525 50  0000 C CNN
F 3 "" H 6000 7525 50  0000 C CNN
	1    6000 7525
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 7525 6000 7525
Wire Wire Line
	6000 7075 6000 7225
Wire Wire Line
	6400 7075 6400 7225
$Comp
L Crystal Y?
U 1 1 5791AC33
P 6200 7075
F 0 "Y?" H 6200 7225 50  0000 C CNN
F 1 "24Mhz" H 6200 6925 50  0000 C CNN
F 2 "Crystals:Crystal_HC49-SD_SMD" H 6200 7075 50  0001 C CNN
F 3 "" H 6200 7075 50  0000 C CNN
F 4 "ABLS2-24.000MHZ-D4YF-T" H 6200 7075 60  0001 C CNN "part"
F 5 "2467732" H 6200 7075 60  0001 C CNN "farnell"
	1    6200 7075
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5791AD48
P 6400 6925
F 0 "R?" V 6480 6925 50  0000 C CNN
F 1 "255" V 6400 6925 50  0000 C CNN
F 2 "" V 6330 6925 50  0000 C CNN
F 3 "" H 6400 6925 50  0000 C CNN
	1    6400 6925
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 7075 6350 7075
Wire Wire Line
	6050 7075 6000 7075
Text Label 6000 7075 1    60   ~ 0
OSC_IN
Text Label 1375 2000 2    60   ~ 0
OSC_IN
Text Label 1375 2100 2    60   ~ 0
OSC_OUT
Text Label 6400 6775 1    60   ~ 0
OSC_OUT
$Comp
L R R?
U 1 1 5791C2AA
P 2825 7075
F 0 "R?" V 2905 7075 50  0000 C CNN
F 1 "10k" V 2825 7075 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 2755 7075 50  0001 C CNN
F 3 "" H 2825 7075 50  0000 C CNN
F 4 "MC01W0805110K" V 2825 7075 60  0001 C CNN "part"
F 5 "9332391" V 2825 7075 60  0001 C CNN "farnell"
	1    2825 7075
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP?
U 1 1 5791C31D
P 2825 7325
F 0 "JP?" H 2825 7405 50  0000 C CNN
F 1 "Jumper_NO_Small" H 2835 7265 50  0001 C CNN
F 2 "" H 2825 7325 50  0000 C CNN
F 3 "" H 2825 7325 50  0000 C CNN
	1    2825 7325
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 7225 2825 7225
$Comp
L GND #PWR?
U 1 1 5791C4BD
P 2825 7425
F 0 "#PWR?" H 2825 7175 50  0001 C CNN
F 1 "GND" H 2825 7275 50  0001 C CNN
F 2 "" H 2825 7425 50  0000 C CNN
F 3 "" H 2825 7425 50  0000 C CNN
	1    2825 7425
	1    0    0    -1  
$EndComp
Text GLabel 2825 6925 1    60   Input ~ 0
3.3V
$Sheet
S 700  7100 1625 500 
U 5791CEF6
F0 "Communication_and_debug" 60
F1 "communication_and_debug.sch" 60
$EndSheet
Wire Notes Line
	5800 6300 6625 6300
Wire Notes Line
	6625 6300 6625 7675
Wire Notes Line
	6625 7675 5800 7675
Wire Notes Line
	5800 7675 5800 6300
Text Notes 5850 6400 0    60   ~ 0
Oscillator
Wire Notes Line
	7075 5600 11150 5600
Wire Notes Line
	11150 5600 11150 6525
Wire Notes Line
	11125 6500 7050 6500
Wire Notes Line
	7075 6525 7075 5600
Text Notes 7125 5700 0    60   ~ 0
Decoupling
Text GLabel 10350 3000 2    60   Input ~ 0
CAN_RX
Text GLabel 10375 3100 2    60   Input ~ 0
CAN_TX
Text Label 2900 7225 0    60   ~ 0
BOOT0
Text Label 1375 1400 2    60   ~ 0
BOOT0
Text GLabel 10375 4300 2    60   Input ~ 0
USART_RX
Text GLabel 10375 4200 2    60   Input ~ 0
USART_TX
Text GLabel 10375 3400 2    60   Input ~ 0
JTDI
Text GLabel 10375 3900 2    60   Input ~ 0
JTDO
Text GLabel 10375 3300 2    60   Input ~ 0
JTCLK
Text GLabel 10375 3200 2    60   Input ~ 0
JTMS
Text GLabel 10375 4000 2    60   Input ~ 0
JTNRST
Text GLabel 1375 1200 0    60   Input ~ 0
NRST
Text Notes 2700 6625 0    60   ~ 0
Boot selection\nBridge for built\nin bootloader
Wire Notes Line
	2625 6300 3450 6300
Wire Notes Line
	3450 6300 3450 7675
Wire Notes Line
	3450 7675 2625 7675
Wire Notes Line
	2625 7675 2625 6300
Text GLabel 10375 1900 2    60   Input ~ 0
LeftRear_CS
Text GLabel 10375 2000 2    60   Input ~ 0
RightRear_CS
Text GLabel 10375 2100 2    60   Input ~ 0
LeftFront_CS
Text GLabel 10375 2200 2    60   Input ~ 0
RightFront_CS
Text GLabel 10375 3700 2    60   Input ~ 0
LeftFront_DIAG
Text GLabel 10375 3800 2    60   Input ~ 0
RightFront_DIAG
Text GLabel 10375 2700 2    60   Input ~ 0
LeftRear_DIAG
Text GLabel 10375 2800 2    60   Input ~ 0
RightRear_DIAG
$Comp
L CAT250_EEPROM U?
U 1 1 579296D6
P 4675 7200
F 0 "U?" H 4375 7450 50  0000 L CNN
F 1 "CAT250_EEPROM" H 4725 7450 50  0000 L CNN
F 2 "Housings_SSOP:TSSOP-8_4.4x3mm_Pitch0.65mm" H 4325 7150 50  0001 C CNN
F 3 "" H 4325 7150 50  0000 C CNN
F 4 "CAT25160YI-GT3" H 4675 7200 60  0001 C CNN "part"
F 5 "2534647" H 4675 7200 60  0001 C CNN "farnell"
	1    4675 7200
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579298BC
P 4275 6725
F 0 "C?" H 4300 6825 50  0000 L CNN
F 1 "1n" H 4300 6625 50  0000 L CNN
F 2 "" H 4313 6575 50  0000 C CNN
F 3 "" H 4275 6725 50  0000 C CNN
	1    4275 6725
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5792991D
P 4025 6725
F 0 "C?" H 4050 6825 50  0000 L CNN
F 1 "100n" H 4050 6625 50  0000 L CNN
F 2 "" H 4063 6575 50  0000 C CNN
F 3 "" H 4025 6725 50  0000 C CNN
	1    4025 6725
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 579299FE
P 4025 6875
F 0 "#PWR?" H 4025 6625 50  0001 C CNN
F 1 "GND" H 4025 6725 50  0001 C CNN
F 2 "" H 4025 6875 50  0000 C CNN
F 3 "" H 4025 6875 50  0000 C CNN
	1    4025 6875
	1    0    0    -1  
$EndComp
Wire Wire Line
	4275 6875 4025 6875
Wire Wire Line
	4275 6575 4025 6575
Text GLabel 4025 6575 0    60   Input ~ 0
+3.3V
Text GLabel 4675 6900 1    60   Input ~ 0
+3.3V
$Comp
L GND #PWR?
U 1 1 57929D36
P 4675 7500
F 0 "#PWR?" H 4675 7250 50  0001 C CNN
F 1 "GND" H 4675 7350 50  0001 C CNN
F 2 "" H 4675 7500 50  0000 C CNN
F 3 "" H 4675 7500 50  0000 C CNN
	1    4675 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4275 7100 4275 7200
Text GLabel 4275 7100 0    60   Input ~ 0
+3.3V
Text Label 4275 7300 2    60   ~ 0
SPI_CS
Text Label 5075 7200 0    60   ~ 0
SPI_MOSI
Text Label 5075 7300 0    60   ~ 0
SPI_MISO
Text Label 5075 7100 0    60   ~ 0
SPI_SCK
Text Notes 3750 6400 0    60   ~ 0
EEPROM
Wire Notes Line
	3675 6300 5525 6300
Wire Notes Line
	5525 6300 5525 7675
Wire Notes Line
	5525 7675 3675 7675
Wire Notes Line
	3675 7675 3675 6300
Text Label 1300 3850 2    60   ~ 0
SPI_SCK
Text Label 1300 3750 2    60   ~ 0
SPI_CS
Text Label 1300 3950 2    60   ~ 0
SPI_MISO
Text Label 1300 4050 2    60   ~ 0
SPI_MOSI
Text GLabel 1375 3300 0    60   Input ~ 0
LeftFront_Tighten
Text GLabel 1375 3400 0    60   Input ~ 0
LeftFront_Loosen
Text GLabel 1375 3500 0    60   Input ~ 0
RightFront_Tighten
Text GLabel 1375 3600 0    60   Input ~ 0
RightFront_Loosen
Text GLabel 1375 4100 0    60   Input ~ 0
LeftRear_Tighten
Text GLabel 1375 4200 0    60   Input ~ 0
LeftRear_Loosen
Text GLabel 1375 4300 0    60   Input ~ 0
RightRear_Tighten
Text GLabel 1375 4400 0    60   Input ~ 0
RightRear_Loosen
Text GLabel 10375 4100 2    60   Input ~ 0
Bridge_Disable
Text GLabel 10375 2300 2    60   Input ~ 0
LR_speed_in
Text GLabel 10375 2400 2    60   Input ~ 0
RR_speed_in
Text GLabel 10375 2500 2    60   Input ~ 0
LF_speed_in
Text GLabel 10375 2600 2    60   Input ~ 0
RF_speed_in
$Comp
L STM32F373RBTx U?
U 1 1 57934AF3
P 5925 3050
F 0 "U?" H 1525 5250 50  0000 L BNN
F 1 "STM32F373RBTx" H 10275 5225 50  0000 R BNN
F 2 "Housings_QFP:LQFP-64_10x10mm_Pitch0.5mm" H 10275 5175 50  0001 R TNN
F 3 "" H 4875 3100 50  0000 C CNN
F 4 "STM32F373RBT6" H 5925 3050 60  0001 C CNN "part"
F 5 "2333277" H 5925 3050 60  0001 C CNN "farnell"
	1    5925 3050
	1    0    0    -1  
$EndComp
Text Label 1375 1700 2    60   ~ 0
VREF+
$Comp
L C C?
U 1 1 579360EF
P 10100 6175
F 0 "C?" H 10125 6275 50  0000 L CNN
F 1 "10n" H 10125 6075 50  0000 L CNN
F 2 "" H 10138 6025 50  0000 C CNN
F 3 "" H 10100 6175 50  0000 C CNN
	1    10100 6175
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 579360F5
P 10350 6175
F 0 "C?" H 10375 6275 50  0000 L CNN
F 1 "1u" H 10375 6075 50  0000 L CNN
F 2 "" H 10388 6025 50  0000 C CNN
F 3 "" H 10350 6175 50  0000 C CNN
	1    10350 6175
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 579360FB
P 10350 6325
F 0 "#PWR?" H 10350 6075 50  0001 C CNN
F 1 "GND" H 10350 6175 50  0001 C CNN
F 2 "" H 10350 6325 50  0000 C CNN
F 3 "" H 10350 6325 50  0000 C CNN
	1    10350 6325
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 6325 10100 6325
Wire Wire Line
	10350 6025 10100 6025
Text GLabel 10350 6025 1    60   Input ~ 0
3.3V
Text Label 10100 6025 1    60   ~ 0
VREF+
Text Label 10100 6325 2    60   ~ 0
VSSA
Text GLabel 10375 4400 2    60   Input ~ 0
L_speed_out+
Text GLabel 10375 4500 2    60   Input ~ 0
L_speed_out-
Text GLabel 10375 4600 2    60   Input ~ 0
R_speed_out+
Text GLabel 10375 4700 2    60   Input ~ 0
R_speed_out-
Text GLabel 1375 4500 0    60   Input ~ 0
~uC_SET_POWER
$Sheet
S 700  6300 1625 550 
U 5793D1EA
F0 "Input_things" 60
F1 "input_things.sch" 60
$EndSheet
Text GLabel 1375 2200 0    60   Input ~ 0
TIGHTEN_CMD_SIG
Text GLabel 1375 2300 0    60   Input ~ 0
RELEASE_CMD_SIG
Text GLabel 10375 3600 2    60   Input ~ 0
PWR_voltsense
Text GLabel 1375 2500 0    60   Input ~ 0
BUTTON_SIG
Text GLabel 1375 2600 0    60   Input ~ 0
BUTTON_INV_SIG
Text GLabel 1375 2800 0    60   Input ~ 0
IGN_ON_SIG
Text GLabel 1375 2900 0    60   Input ~ 0
LIGHTS_ON_SIG
Text GLabel 10375 2900 2    60   Input ~ 0
PWR_ENABLED_SIG
Text GLabel 1375 4600 0    60   Input ~ 0
VR_diag_test
Text GLabel 1375 3100 0    60   Input ~ 0
Feed_voltsense
Wire Wire Line
	1375 3700 1350 3700
Wire Wire Line
	1350 3700 1350 3750
Wire Wire Line
	1350 3750 1300 3750
Wire Wire Line
	1375 3800 1350 3800
Wire Wire Line
	1350 3800 1350 3850
Wire Wire Line
	1350 3850 1300 3850
Wire Wire Line
	1375 3900 1350 3900
Wire Wire Line
	1350 3900 1350 3950
Wire Wire Line
	1350 3950 1300 3950
Wire Wire Line
	1375 4000 1350 4000
Wire Wire Line
	1350 4000 1350 4050
Wire Wire Line
	1350 4050 1300 4050
NoConn ~ 1375 3200
$EndSCHEMATC
