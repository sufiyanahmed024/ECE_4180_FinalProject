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