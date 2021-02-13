EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 11774 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	7800 3400 6300 3400
Wire Wire Line
	6300 3400 6300 2900
Wire Wire Line
	6300 2500 6300 2100
Wire Wire Line
	6300 2100 7500 2100
Wire Wire Line
	7500 2100 7500 2250
Wire Wire Line
	7800 2100 7500 2100
Connection ~ 7500 2100
Wire Wire Line
	7100 2800 7100 2300
Wire Wire Line
	7100 2300 6700 2300
Wire Wire Line
	6700 2500 6700 2300
Wire Wire Line
	5800 3500 5900 3500
Connection ~ 6700 2300
Wire Wire Line
	7100 3000 7100 3200
Wire Wire Line
	7100 3200 6700 3200
Wire Wire Line
	6700 3200 6700 2900
Connection ~ 6700 3200
Wire Wire Line
	4600 3700 5100 3700
Wire Wire Line
	4500 3700 4600 3700
Connection ~ 4600 3700
Wire Wire Line
	5800 3700 6000 3700
Wire Wire Line
	6400 3700 6500 3700
Wire Wire Line
	7500 2650 7500 2800
Wire Wire Line
	7500 2800 7800 2800
Wire Wire Line
	7800 3100 7500 3100
Wire Wire Line
	7500 3100 7500 2800
Connection ~ 7500 2800
Wire Wire Line
	4500 2700 6000 2700
Wire Wire Line
	4500 3500 4600 3500
Wire Wire Line
	5000 3500 5100 3500
$Comp
L relay_board_v1_0-rescue:G5LE-relays_board_v1_0-eagle-import K1
U 1 1 42D1CC17
P 6700 2700
F 0 "K1" H 6700 2700 50  0001 C CNN
F 1 "G5LE" H 6750 2815 59  0001 L BNN
F 2 "relays_board_v1_0:G5LE" H 6700 2700 50  0001 C CNN
F 3 "" H 6700 2700 50  0001 C CNN
	1    6700 2700
	-1   0    0    1   
$EndComp
$Comp
L relay_board_v1_0-rescue:G5LE-relays_board_v1_0-eagle-import K1
U 2 1 42D1CC1B
P 6100 2700
F 0 "K1" H 6100 2700 50  0001 C CNN
F 1 "G5LE" H 6150 2815 59  0001 L BNN
F 2 "relays_board_v1_0:G5LE" H 6100 2700 50  0001 C CNN
F 3 "" H 6100 2700 50  0001 C CNN
	2    6100 2700
	0    1    1    0   
$EndComp
$Comp
L relay_board_v1_0-rescue:A4L-LOC-relays_board_v1_0-eagle-import #FRAME1
U 1 1 7D0788B5
P 900 6700
F 0 "#FRAME1" H 900 6700 50  0001 C CNN
F 1 "A4L-LOC" H 900 6700 50  0001 C CNN
F 2 "" H 900 6700 50  0001 C CNN
F 3 "" H 900 6700 50  0001 C CNN
	1    900  6700
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:1N4007-relays_board_v1_0-eagle-import D1
U 1 1 DA5D11AB
P 7100 2900
F 0 "D1" H 7200 2919 59  0000 L BNN
F 1 "1N4007" H 7200 2809 59  0000 L BNN
F 2 "relays_board_v1_0:DO41-10" H 7100 2900 50  0001 C CNN
F 3 "" H 7100 2900 50  0001 C CNN
	1    7100 2900
	0    -1   -1   0   
$EndComp
$Comp
L relay_board_v1_0-rescue:10D470K-relays_board_v1_0-eagle-import R1
U 1 1 EC893FD4
P 7500 2450
F 0 "R1" H 7550 2304 59  0000 L BNN
F 1 "10D470K" H 7300 2200 59  0000 L BNN
F 2 "relays_board_v1_0:10D470K" H 7500 2450 50  0001 C CNN
F 3 "" H 7500 2450 50  0001 C CNN
	1    7500 2450
	0    -1   -1   0   
$EndComp
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import AC_N2_
U 2 1 13DBCEA2
P 7900 3100
F 0 "AC_N2_" H 8040 3075 59  0000 L BNN
F 1 "OSTTA040161" H 7900 3100 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 7900 3100 50  0001 C CNN
F 3 "" H 7900 3100 50  0001 C CNN
	2    7900 3100
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import AC_N1_
U 2 1 13DBCEAE
P 7900 2800
F 0 "AC_N1_" H 8040 2775 59  0000 L BNN
F 1 "OSTTA040161" H 7900 2800 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 7900 2800 50  0001 C CNN
F 3 "" H 7900 2800 50  0001 C CNN
	2    7900 2800
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import AC_OUT_ON_
U 2 1 13DBCEAA
P 7900 2100
F 0 "AC_OUT_ON_" H 8040 2075 59  0000 L BNN
F 1 "OSTTA040161" H 7900 2100 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 7900 2100 50  0001 C CNN
F 3 "" H 7900 2100 50  0001 C CNN
	2    7900 2100
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import AC_OUT_OFF_
U 2 1 13DBCEB6
P 7900 3400
F 0 "AC_OUT_OFF_" H 8040 3375 59  0000 L BNN
F 1 "OSTTA040161" H 7900 3400 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 7900 3400 50  0001 C CNN
F 3 "" H 7900 3400 50  0001 C CNN
	2    7900 3400
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import AC_IN_
U 1 1 97A34854
P 4400 2700
F 0 "AC_IN_" H 4540 2675 59  0000 L BNN
F 1 "OSTTA040161" H 4400 2700 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 4400 2700 50  0001 C CNN
F 3 "" H 4400 2700 50  0001 C CNN
	1    4400 2700
	-1   0    0    1   
$EndComp
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import CTRL_
U 1 1 97A34850
P 4400 3500
F 0 "CTRL_" H 4540 3475 59  0000 L BNN
F 1 "OSTTA040161" H 4400 3500 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 4400 3500 50  0001 C CNN
F 3 "" H 4400 3500 50  0001 C CNN
	1    4400 3500
	-1   0    0    1   
$EndComp
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import GND_
U 1 1 97A3484C
P 4400 3700
F 0 "GND_" H 4540 3675 59  0000 L BNN
F 1 "OSTTA040161" H 4400 3700 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 4400 3700 50  0001 C CNN
F 3 "" H 4400 3700 50  0001 C CNN
	1    4400 3700
	-1   0    0    1   
$EndComp
$Comp
L relay_board_v1_0-rescue:+5V-relays_board_v1_0-eagle-import #SUPPLY01
U 1 1 9BB82FD5
P 5900 1900
F 0 "#SUPPLY01" H 5900 1900 50  0001 C CNN
F 1 "+5V" H 5825 2025 59  0000 L BNN
F 2 "" H 5900 1900 50  0001 C CNN
F 3 "" H 5900 1900 50  0001 C CNN
	1    5900 1900
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:TLP621-relays_board_v1_0-eagle-import OK1
U 1 1 DA224995
P 5500 3600
F 0 "OK1" H 5225 3825 59  0000 L BNN
F 1 "TLP621" H 5225 3300 59  0000 L BNN
F 2 "relays_board_v1_0:DIL04" H 5500 3600 50  0001 C CNN
F 3 "" H 5500 3600 50  0001 C CNN
	1    5500 3600
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:2N3904-relays_board_v1_0-eagle-import T1
U 1 1 789B478A
P 6600 3700
F 0 "T1" H 6750 3750 59  0000 L BNN
F 1 "2N3904" H 6750 3600 59  0000 L BNN
F 2 "relays_board_v1_0:TO92" H 6600 3700 50  0001 C CNN
F 3 "" H 6600 3700 50  0001 C CNN
	1    6600 3700
	1    0    0    -1  
$EndComp
$Comp
L relay_board_v1_0-rescue:R-EU_0204_7-relays_board_v1_0-eagle-import R2
U 1 1 BDFB0F3E
P 6200 3700
F 0 "R2" H 6050 3759 59  0000 L BNN
F 1 "1k2" H 6050 3570 59  0000 L BNN
F 2 "relays_board_v1_0:0204_7" H 6200 3700 50  0001 C CNN
F 3 "" H 6200 3700 50  0001 C CNN
	1    6200 3700
	-1   0    0    1   
$EndComp
$Comp
L relay_board_v1_0-rescue:R-EU_0204_7-relays_board_v1_0-eagle-import R4
U 1 1 DFB4546F
P 4800 3500
F 0 "R4" H 4650 3559 59  0000 L BNN
F 1 "270" H 4650 3370 59  0000 L BNN
F 2 "relays_board_v1_0:0204_7" H 4800 3500 50  0001 C CNN
F 3 "" H 4800 3500 50  0001 C CNN
	1    4800 3500
	-1   0    0    1   
$EndComp
Wire Wire Line
	6700 3500 6700 3200
Wire Wire Line
	4600 3700 4600 4050
Wire Wire Line
	6700 3900 6700 4050
Wire Wire Line
	6700 4050 4600 4050
$Comp
L relay_board_v1_0-rescue:GND-relays_board_v1_0-eagle-import #SUPPLY02
U 1 1 7668A807
P 6700 4300
F 0 "#SUPPLY02" H 6700 4300 50  0001 C CNN
F 1 "GND" H 6625 4175 59  0000 L BNN
F 2 "" H 6700 4300 50  0001 C CNN
F 3 "" H 6700 4300 50  0001 C CNN
	1    6700 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3500 5900 2300
Wire Wire Line
	5900 2300 6700 2300
Wire Wire Line
	5900 2300 5900 2000
$Comp
L relay_board_v1_0-rescue:OSTTA040161-relays_board_v1_0-eagle-import DC_IN_
U 1 1 97A34858
P 4400 2300
F 0 "DC_IN_" H 4540 2275 59  0000 L BNN
F 1 "OSTTA040161" H 4400 2300 50  0001 C CNN
F 2 "relays_board_v1_0:040161" H 4400 2300 50  0001 C CNN
F 3 "" H 4400 2300 50  0001 C CNN
	1    4400 2300
	-1   0    0    1   
$EndComp
Connection ~ 5900 2300
Wire Wire Line
	4500 2300 5900 2300
Wire Wire Line
	6700 4200 6700 4050
Connection ~ 6700 4050
$EndSCHEMATC
