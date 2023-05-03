# Distance Sensing Robot with Voice Control

**Team Members: Sufiyan Ahmed, Ashwin Kulkarni, Chinmay Galande, Anirudh Arunkumar**

**Georgia Insitute of Tehcnology**

Watch Presentation/Demo below: https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/video-20230503-164835-598cebf7.mov

Presentation: https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/presentation_slides_4180_final.pptx

Demo: add link

![](https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/robot.png)

## Table of Contents

* [Introduction](#Introduction)
* [Parts List](#Parts-List)
* [Schematics & Connection Guides](#Schematics--Connection-Guides)
* [Source Code](#Source-Code)
* [Conclusion & Future](#Conclusion--Future)

## Introduction

Over time, many different types of autonomous robots have entered the market, each with different uses and purposes. Some are just toys used purely for entertainment. Some have actual functions, such as a Roomba which uses sensors and cameras to detect its surroundings and navigate them whilst vacuuming. Using this as the primary motivation behind our project, we came up with the idea of designing a robot that will be able to autonomously navigate its surroundings and avoid obstacles, as well as be manually controlled using voice commands.

Click below to view video of a Roomba:

[![Roomba](https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/roomba.png)](https://www.youtube.com/watch?v=tZ0bq-jIg-o&ab_channel=iRobot "Roomba# ")

The robot can either be run in autonomous mode or take voice commands from the MIT App Inventor. In autonomous mode, it will be using the UltraSonic sensor to detect the distance between it and any objects in front of it. Once it detects an object within around 25 cm, it will change direction and then start moving in a straight line once again. It can also take basic voice commands such as "Forward" and "Reverse" from a voice control app developed on MIT app inventor which is sent over Bluetooth to the HC05 Bluetooth module, which then is sent to the Mbed and controls the movement of the robot.

Block Diagram:

![](https://github.com/sufiyanahmed024/ECE_4180_FinalProject/blob/main/4180_Final.png)

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
```
#include "Motor.h"
#include "mbed.h"
#include "rtos.h"
#include "ultrasonic.h"
#include <climits>
#include <ctype.h>
#include <time.h>

#include "uLCD_4DGL.h"
//#include "SongPlayer.h"
//
class Speaker
{
public:
    Speaker(PinName pin) : _pin(pin) {
// _pin(pin) means pass pin to the Speaker Constructor
    }
// class method to play a note based on PwmOut class
    void PlayNote(float frequency, float duration, float volume) {
        _pin.period(1.0/frequency);
        _pin = volume/2.0;
        wait(duration);
        _pin = 0.0;
    }

private:
// sets up specified pin for PWM using PwmOut class 
    PwmOut _pin;
};
//
Motor m1(p21, p6, p5); // pwm, fwd, rev
Motor m2(p22, p13, p14);
Serial pc(USBTX, USBRX);
Serial bt(p28, p27);
Speaker mySpeaker(p26);

uLCD_4DGL uLCD(p9,p10,p30); // tx, rx, rst

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

BusOut led(LED1, LED2, LED3, LED4);


float note[18]= {1568.0,1396.9,1244.5,1244.5,1396.9,1568.0,1568.0,1568.0,1396.9,
                 1244.5,1396.9,1568.0,1396.9,1244.5,1174.7,1244.5,1244.5, 0.0
                };
float duration[18]= {0.68,0.44,0.92,0.68,0.44,0.68,0.44,0.44,0.44,
                     0.44,0.44,0.44,0.44,0.68,0.44,0.68,0.68, 0.0
                    };

float s1 = 0.0;
float s2 = 0.0;
bool autonomous = false;
bool sound = false;
char display = 'u';


Thread motor_th;
Thread sonar_th;
Thread bt_th;
Thread display_th;

void display_func() {
    while(1) {
        
        switch(display) {
            case 'a':
                uLCD.cls();
                uLCD.text_width(1); //4X size text
    uLCD.text_height(2);

    uLCD.locate(1,2);
    uLCD.color(WHITE);
                uLCD.printf("\nAUTONOMOUS\n");
                display = '\0';
            break;
            case 'f':
            uLCD.cls();
            uLCD.text_width(2); //4X size text
    uLCD.text_height(2);

    uLCD.locate(1,1);
    uLCD.color(BLUE);
                uLCD.printf("\nBLUETOOTH\n");
                uLCD.color(WHITE);
                uLCD.printf("\nFORWARD\n");
                display = '\0';
            break;
            case 'b':
            uLCD.cls();
            uLCD.text_width(2); //4X size text
    uLCD.text_height(2);

    uLCD.locate(1,1);
    uLCD.color(BLUE);
                uLCD.printf("\nBLUETOOTH\n");
                uLCD.color(WHITE);
                uLCD.printf("\nBACKWARD\n");
                display = '\0';
            break;
            case 'u':
            uLCD.cls();
            uLCD.text_width(2); //4X size text
    uLCD.text_height(2);

    uLCD.locate(1,1);
    uLCD.color(BLUE);
                uLCD.printf("\nBLUETOOTH\n");
                display = '\0';
            break;
            case 'l':
            uLCD.cls();
            uLCD.text_width(2); //4X size text
    uLCD.text_height(2);

    uLCD.locate(1,1);
    uLCD.color(BLUE);
                uLCD.printf("\nBLUETOOTH\n");
                uLCD.color(WHITE);
                uLCD.printf("\nLEFT\n");
                display = '\0';
            break;
            case 'r':
            uLCD.cls();
            uLCD.text_width(2); //4X size text
    uLCD.text_height(2);

    uLCD.locate(1,1);
    uLCD.color(BLUE);
                uLCD.printf("\nBLUETOOTH\n");
                uLCD.color(WHITE);
                uLCD.printf("\nRIGHT\n");
                display = '\0';
            break;
            case 's':
            uLCD.cls();
            uLCD.text_width(2); //4X size text
    uLCD.text_height(2);

    uLCD.locate(1,1);
    uLCD.color(BLUE);
                uLCD.printf("\nBLUETOOTH\n");
                uLCD.color(WHITE);
                uLCD.printf("\nSTOP\n");
                display = '\0';
            break;
        }
    // basic printf demo = 16 by 18 characters on screen
    // uLCD.printf("\nHello uLCD World\n"); //Default Green on black text
    // uLCD.printf("\n  Starting Demo...");
    
    
    }
}

void dist(int distance) {
  // put code here to execute when the distance has changed
        
        
        if (autonomous) {

            printf("Distance %d mm\r\n", distance);

            if (distance < 300) {
                srand(time(0));
                int random_number = rand() % 2;
                
            printf("%d", random_number);
               // stops both motors if it detects an obstacle
                s1 = 0;
                s2 = 0;
                printf("s1 = 0, s2 = 0\r\n");
                wait(1.0);

                s1 = -0.5;
                s2 = -0.55;
                wait(1.0);
                
                if (random_number == 1) {
                    //right turn
                    s1 = 0;
                    s2 = 0.7;

                printf("s1 = 0, s2 = 0.5\r\n");
                    wait(1.0);
                    s1 = 0.7;
                } else {
                    //left turn
                    s1 = 0.7;
                    s2 = 0;

                printf("s1 = 0.5, s2 = 0\r\n");
                    wait(1.0);
                    s2 = 0.7;
                }
                
            } else {
                //move straight
                s1 = 0.7;
                s2 = 0.7;
            }

            
        }
  
        
}

// Thread test_th;
// void test_func() {
//     while(1) {
//         display = 'a';
//         // wait(3.00);
//         display = 'b';
//         // wait(3.00);
//         display = 'f';
//         // wait(3.00);
//         display = 'r';
//         // wait(3.00);
//         display = 'l';
//         // wait(3.00);
//         display = 's';
//         // wait(3.00);
//     }
// }



ultrasonic mu(p7, p8, .1, 1,
              &dist); // Set the trigger pin to D8 and the echo pin to D9
                      // have updates every .1 seconds and a timeout after 1
                      // second, and call dist when the distance changes

void sonar() {
    while (1) {
        mu.checkDistance(); 
        // call checkDistance() as much as possible, as this is
        // where the class checks if dist needs to be called.
    }
}

void update_speed() {
    while (1) {
        m1.speed(s1);
        m2.speed(s2);
        wait(0.01);
    }
}

void bluetooth_func() {
  while (true) {
    //printf("Autonomous: %s\r\n", autonomous ? "YES" : "NO");
    //   char *buf;
    //   bt.gets(buf, 20);
    //   printf("Got command: %s\r\n", buf);

    while (bt.readable()) {

      char ch = bt.getc();
      ch = tolower(ch);

      switch (ch) {
      case 'a': {
        // auto
        if (!autonomous) {
          char ch1 = bt.getc();
          ch1 = tolower(ch1);
          if (ch1 == 'u') {
            pc.printf("Auto\r\n");
            led = 0x0;
            display = 'a';
            autonomous = true;
            sound = false;
          }
        }
        break;
      }
      case 'f': {
        // forward
        if (!autonomous) {
          char ch1 = bt.getc();
          if (ch1 == 'o') {
            pc.printf("Forward\r\n");
            display = 'f';
            s1 = 0;
            s2 = 0;
            wait(0.5);
            led = 0xA;
            // m.speed(1);
            s1 = 0.7;
            s2 = 0.7;
            sound = false;
          }
        }
        break;
      }
      case 'b': {
        // bluetooth
        char ch1 = bt.getc();
        if (ch1 == 'l') {
          if (autonomous) {
            pc.printf("Bluetooth\r\n");
            display = 'u';
            s1 = 0;
            s2 = 0;
            led = 0x9;
            autonomous = false;
            sound = false;
          }
        } else if (ch1 == 'a') {
          // back
          if (!autonomous) {
            pc.printf("Backward\r\n");
            s1 = 0;
            s2 = 0;
            wait(0.5);
            display = 'b';
            s1 = -0.5;
            s2 = -0.55;
            
            sound = true;
          }
        }

      } break;
      case 'l': {
        // left
        if (!autonomous) {
          char ch1 = bt.getc();
          if (ch1 == 'e') {
            pc.printf("Left\r\n");
            display = 'l';
            led = 0x8;
            //sound flag
            sound = false;
            s1 = 0;
            s2 = 0;
            wait(0.5);
            s1 = 0.7;
            s2 = 0;
            wait(1.0);
            s2 = 0.7;
            wait(1.0);
            display = 'f';
          }
        }
      } break;
      case 'r': {
        // right
        if (!autonomous) {
          char ch1 = bt.getc();
          if (ch1 == 'i') {
            pc.printf("Right\r\n");
            display = 'r';
            led = 0x1;
            //sound flag
            sound = false;
            s1 = 0;
            s2 = 0;
            wait(0.5);
            s1 = 0;
            s2 = 0.7;
            wait(1.0);
            s1 = 0.7;
            wait(1.0);
            display = 'f';
          }
        }
      } break;
      case 's': {
        // stop
        if (!autonomous) {
          char ch1 = bt.getc();
          if (ch1 == 't') {
            pc.printf("Stop\r\n");
            display = 's';
            led = 0xF;
            //sound flag
            sound = false;
            // m.speed(0)
            s1 = 0;
            s2 = 0;
          }
        }
      } break;
      }
    }
  }
}


int main() {

    mu.startUpdates(); // start measuring the distance
    
    sonar_th.start(sonar);
    motor_th.start(update_speed);
    display_th.start(display_func);
    // test_th.start(test_func);
    bt_th.start(bluetooth_func);
    while(true)
    {
        if(sound == true)
        {
        mySpeaker.PlayNote(969.0, 0.3, 0.5);
        mySpeaker.PlayNote(800.0, 0.3, 0.5);
        }

    }
    
}
```

## Conclusion & Future
Through various inspirations, the idea of our robot was made and developed. It is able to be controlled via an app using voice commands or run autonomously using the ultrasonic sensor. It will also make sounds and have an LCD display that reacts based on its behavior, such as if it detects an object or which direction it is moving. For most cases this is sufficient, but since it only has one front sensor, it is possible that if the angle is off, the robot can approach a wall and collide before detection. This leads to some additional improvements that can be made in the future, such as:

* Adding Sensors to the sides for better autonomous functionality
* Add beeps when object detected
* Add Gyroscope for balancing
* Cliff Detection to ensure it doesnt fall of surfaces
