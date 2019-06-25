EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Doorkeeper wiring"
Date "2019-06-25"
Rev "V0.1"
Comp "Zeus WPI"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_UNO_R3 A?
U 1 1 5D12465A
P 5800 3950
F 0 "A?" H 5800 2669 50  0000 C CNN
F 1 "Arduino_UNO_R3" H 5800 2760 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 5950 2900 50  0001 L CNN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 5600 5000 50  0001 C CNN
	1    5800 3950
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5D124A56
P 5000 3100
F 0 "SW?" V 4954 3248 50  0000 L CNN
F 1 "SW_Push" V 5050 3250 50  0000 L CNN
F 2 "" H 5000 3300 50  0001 C CNN
F 3 "~" H 5000 3300 50  0001 C CNN
	1    5000 3100
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_Servo M?
U 1 1 5D124F67
P 5800 2750
F 0 "M?" V 5840 2581 50  0000 R CNN
F 1 "Motor_Servo" V 5749 2581 50  0000 R CNN
F 2 "" H 5800 2560 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 5800 2560 50  0001 C CNN
	1    5800 2750
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D125933
P 6800 4650
F 0 "#PWR?" H 6800 4500 50  0001 C CNN
F 1 "+5V" H 6815 4823 50  0000 C CNN
F 2 "" H 6800 4650 50  0001 C CNN
F 3 "" H 6800 4650 50  0001 C CNN
	1    6800 4650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D125D1C
P 6250 2650
F 0 "#PWR?" H 6250 2400 50  0001 C CNN
F 1 "GND" H 6255 2477 50  0000 C CNN
F 2 "" H 6250 2650 50  0001 C CNN
F 3 "" H 6250 2650 50  0001 C CNN
	1    6250 2650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6900 4650 6800 4650
$Comp
L power:+5V #PWR?
U 1 1 5D13BAD6
P 6250 2750
F 0 "#PWR?" H 6250 2600 50  0001 C CNN
F 1 "+5V" H 6265 2923 50  0000 C CNN
F 2 "" H 6250 2750 50  0001 C CNN
F 3 "" H 6250 2750 50  0001 C CNN
	1    6250 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	6100 2650 6250 2650
Wire Wire Line
	6100 2750 6250 2750
$Comp
L power:GND #PWR?
U 1 1 5D149A6B
P 4450 3850
F 0 "#PWR?" H 4450 3600 50  0001 C CNN
F 1 "GND" H 4455 3677 50  0000 C CNN
F 2 "" H 4450 3850 50  0001 C CNN
F 3 "" H 4450 3850 50  0001 C CNN
	1    4450 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 3850 4650 3850
Wire Wire Line
	4800 3100 4650 3100
Wire Wire Line
	4650 3100 4650 3850
Connection ~ 4650 3850
Wire Wire Line
	4650 3850 4450 3850
Wire Wire Line
	6200 3100 5200 3100
Wire Wire Line
	6200 3450 6200 3100
Wire Wire Line
	6300 2850 6300 3200
Wire Wire Line
	6300 3200 5500 3200
Wire Wire Line
	5500 3200 5500 3450
Wire Wire Line
	6100 2850 6300 2850
Wire Wire Line
	6900 3850 6800 3850
Wire Wire Line
	6900 4650 6900 3850
Wire Wire Line
	6800 4050 7000 4050
Wire Wire Line
	7000 4050 7000 3100
Wire Wire Line
	7000 3100 6200 3100
Connection ~ 6200 3100
$EndSCHEMATC
