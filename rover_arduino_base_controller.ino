/*
 ====================================================

 PINOUT (Arduino UNO R3)

 9		Servo 1
 10		Servo 2
 11		Servo 2

 5		Motor Left PWM
 6		Motor Right PWM

 14		Motor Left Direction F
 15		Motor Left Direction R
 16		Motor Right Direction F
 17		Motor Right Direction R


 0		RX (to rpi TX)
 1		TX (to rpi RX)

 ====================================================

 CONTROL DATA STRUCTURE

 BYTE 0	magic header 0xcd for normal 0xee for reset
 BYTE 1	Servo 1 degree (0-180)
 BYTE 2	Servo 2 degree (0-180)
 BYTE 3	Servo 3 degree (0-180)
 BYTE 4	Motor Direction (xxxxxxLR, 1=F 0=R)
 BYTE 5	Motor Left PWM (0-255)
 BYTE 6 Motor Right PWM (0-255)

 ====================================================

 NOTE

 This program will not verify control data value,
 make sure data from rpi are in correct range!!!

 ====================================================

*/

//soft_reset
#include <avr/wdt.h> //include watch dog library
void soft_reset() //soft reset by using WDT
{
        wdt_enable(WDTO_15MS);
        while (true);
}

#include <Servo.h> //include servo library
Servo servo_1; //create servo object 1
Servo servo_2; //create servo object 2
Servo servo_3; //create servo object 3

//define magic header for easy reading
#define RESET_MAGIC_HEADER 0xee
#define CONTROL_DATA_MAGIC_HEADER 0xcd
#define CONTROL_DATA_ARRAY_SIZE 7

uint8_t control_data[CONTROL_DATA_ARRAY_SIZE]; //define global array control_data

// the setup function runs once when you press reset or power the board
void setup()
{
        //set pin mode for motor controller
        //pinMode(3, OUTPUT);

        pinMode(5, OUTPUT);
        pinMode(6, OUTPUT);

        pinMode(14, OUTPUT);
        pinMode(15, OUTPUT);
        pinMode(16, OUTPUT);
        pinMode(17, OUTPUT);

        pinMode(9, OUTPUT);
        pinMode(10, OUTPUT);
        pinMode(11, OUTPUT);

        //set pin mode for system LED
        pinMode(13, OUTPUT);

        //attach servo PWM
        servo_1.attach(9);
        servo_2.attach(10);
        servo_3.attach(11);

        //serial port config
        Serial.begin(115200);
        Serial.setTimeout(60000);

        //send init done
        delay(50);
        Serial.write('i'); //0x69

        //move servo to default position
        servo_1.write(90);
        servo_2.write(180);
        servo_3.write(30);
}

// the loop function runs over and over again until power down or reset
void loop()
{
        //require control data
        Serial.write('r'); //0x72

        Serial.readBytes(control_data, CONTROL_DATA_ARRAY_SIZE);

        //magic header verify
        if (control_data[0] == RESET_MAGIC_HEADER)
        {
                //valid soft reset magic code
                //process control data
                digitalWrite(13, HIGH);
                digitalWrite(13, !(bool)digitalRead(13));
                soft_reset();
        }
        else if (control_data[0] == CONTROL_DATA_MAGIC_HEADER)
        {
                //valid control data magic code
                //process control data

                //servo motor
                servo_1.write(control_data[1]);
                servo_2.write(control_data[2]);
                servo_3.write(control_data[3]);

                //DC motor
                //control_data[4] &= 0x03;
                digitalWrite(14, (control_data[4] >> 1) & 0x01);
                digitalWrite(15, !(bool)((control_data[4] >> 1) & 0x01));
                digitalWrite(16, control_data[4] & 0x01);
                digitalWrite(17, !(bool)(control_data[4] & 0x01));

                //PWM for DC motor
                analogWrite(5, control_data[5]);
                analogWrite(6, control_data[6]);
        }
        else
        {
                Serial.write('f'); //0x66
                digitalWrite(13, HIGH);

                //read until buffer is empty
                while (Serial.available() > 0)
                {
                        Serial.read();
                }
                digitalWrite(13, LOW);
        }
}
