# Distance Sensing Robot with Voice Control

**Team Members: Sufiyan Ahmed, Ashwin Kulkarni, Chinmay Galande, Anirudh Arunkumar**

**Georgia Insitute of Tehcnology**

Watch Presentation/Demo below:

Presentation: add link

Demo: add link

**INSERT IMAGE OF ROBOT**

## Table of Contents

* [Introduction](#Introduction)
* [Parts List](#Parts-List)
* [Schematics & Connection Guides](#Schematics--Connection-Guides)
* [Source Code](#Source-Code)
* [Conclusion & Future](#Conclusion--Future)
* [References](##References)

## Introduction

Over time, many different types of autonomous robots have entered the market, each with different uses and purposes. Some are just toys used purely for entertainment. Some have actual functions, such as a Roomba which uses sensors and cameras to detect its surroundings and navigate them whilst vacuuming. Using this as the primary motivation behind our project, we came up with the idea of designing a robot that will be able to autonomously navigate its surroundings and avoid obstacles, as well as be manually controlled using voice commands.

Click below to view video of a Roomba:

[![Roomba](https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/roomba.png)](https://www.youtube.com/watch?v=tZ0bq-jIg-o&ab_channel=iRobot "Roomba# ")

The robot can either be run in autonomous mode or take voice commands from the MIT App Inventor. In autonomous mode, it will be using the UltraSonic sensor to detect the distance between it and any objects in front of it. Once it detects an object within around 25 cm, it will change direction and then start moving in a straight line once again. It can also take basic voice commands such as "Forward" and "Reverse" from a voice control app developed on MIT app inventor which is sent over Bluetooth to the HC05 Bluetooth module, which then is sent to the Mbed and controls the movement of the robot.

Block Diagram:

![](https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/4180_Final_Block.png)

## Parts List

* Mbed LPC1768: https://www.sparkfun.com/products/9564
* HC05 Bluetooth Module: https://a.co/d/0m9C4Wu
* Jumper Wires: https://www.sparkfun.com/products/124, https://www.sparkfun.com/products/11026, https://www.sparkfun.com/products/12794
* Ultrasonic Sensor (HC-SR04): https://www.digikey.com/en/products/detail/osepp-electronics-ltd/HC-SR04/11198533
* H-Bridge (SparkFun Motor Driver): https://www.sparkfun.com/products/14450
* uLCD-144-G2 128: https://www.sparkfun.com/products/11377
* Speaker: https://www.sparkfun.com/products/11089
* DC Motor (DG01D) x2: https://www.sparkfun.com/products/13302
* Rubber Wheels x2: https://www.sparkfun.com/products/13259
* Shadow Chassis: https://www.sparkfun.com/products/13301
* AAA Batteries x4: https://a.co/d/3kETYpp

## Schematics & Connection Guides

**Setup**

All Devices are connected to and controlled using the Mbed. 4 AAA batteries were used as the power supply for the Mbed and the motors. View the schematic below for more details:

**Schematic**

![](https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/final_bb.png)

**Connection Tables**

Mbed to Bluetooth

| Bluetooth | Mbed | Batteries |
| --- | --- | --- |
| GND | GND | GND |
| TX | (RX)P27 | |
| RX | (TX)P28 | |
| EN | | |
| STATE | | |
| VCC | VOUT | |

Mbed to Motor Driver and Motors

| H-Bridge | Mbed | Motor 1 | Motor 2 |  Batteries |
| --- | --- | --- | --- | --- |
| VM | | | | 9V |
| VCC | VOUT | | | |
| A01 | | + | | |
| A02 | | - | | |
| B01 | | | + | |
| B02 | | | - | |
| PWMA | P21 | | | |
| PWMB | P22 | | | |
| AI1 | P5 | | | |
| AI2 | P6 | | | |
| BI1 | P13 | | | |
| BI2 | P14 | | | |
| STBY | VOUT | | | |
| GND | GND | | | GND |


Mbed to Ultrasonic Sensor

| Ultrasonic Sensor | Mbed | Batteries |
| --- | --- | --- |
| VCC | VOUT | |
| GND | GND | GND |
| TRIG | P7 | |
| ECHO | P8 | |

Mbed to Class D Amp to Speaker

| Class D Amp | Mbed | Speaker | Batteries |
| --- | --- | --- | --- |
| PWR- | GND | | GND |
| PWR+ | VOUT | | |
| IN- | GND | | GND |
| IN+ | P26 | | |
| OUT- | | - | |
| OUT+ | | + | |
| S | PX(fill) | | |

Mbed to uLCD

| uLCD | Mbed | Batteries |
| --- | --- | --- |
| GND | GND | GND |
| 5V | VU | | 
| RX | (TX)P9 | |
| TX | (RX)P10 | |
| RST | P30 | |

## Source Code

main.cpp
[https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/main.cpp](https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/7ae27c835eb536f6b5e15d7121976144b436c701/main.cpp#L1-L414)
