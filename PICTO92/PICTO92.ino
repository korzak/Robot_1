/************************************************************************************************************************************************************************                                              
 * - Author           : BELKHIR Mohamed (BEE MB)     *                                               
 * - Profession       : (Developer) MEGA DAS owner   *                                                                                              
 * - Main purpose     : Robotic Application          *                                                                                  
 * - Copyright holder : No copyrights                *                                  
 * - Date             : 25/02/2017                   *
 * ***********************************************************************************************************************************************************************
                                           -----------------------------------------------------------------------
                                               P I C T O   9 2   T H E   L I N E   F O L L O W E R   R O B O T
                                           -----------------------------------------------------------------------
This project is established by MEGA DAS in order to perform some Robotic skills. PICTO92 is a mobile robot which has the capability to move around in its environment
folloa visual line painted or embedded in the floor or ceiling or an electrical wire in the floor. Most of these robots operated a simple "keep the line in 
the center sensor" algorithm, this means the robot is capable of navigating an uncontrolled environment without the need for physical or electro-mechanical guidance 
devices. In our robot we used 4 IR Transmitter and Receiver sensors (TCRT5000) in order to detect the line to follow, and we used a motor driver (l293D) to control 
two 9V DC motors placed in both sides of our device. The robot can detect objects and avoid it through a small servo motor and an Ultrasonic sensor. 
Th following connecting indicates the wiring technique to properly use the sensors and actuators.

                                                          ░░░░░░░░░░░░▄▄░░░░░░░░░
                                                          ░░░░░░░░░░░█░░█░░░░░░░░
                                                          ░░░░░░░░░░░█░░█░░░░░░░░
                                                          ░░░░░░░░░░█░░░█░░░░░░░░
                                                          ░░░░░░░░░█░░░░█░░░░░░░░
                                                          ███████▄▄█░░░░░██████▄░
                                                          ▓▓▓▓▓▓█░░░░░░░░░░░░░░█░
                                                          ▓▓▓▓▓▓█░░░░░░░░░░░░░░█░
                                                          ▓▓▓▓▓▓█░░░░░░░░░░░░░░█░
                                                          ▓▓▓▓▓▓█░░░░░░░░░░░░░░█░
                                                          ▓▓▓▓▓▓█░░░░░░░░░░░░░░█░
                                                          ▓▓▓▓▓▓█████░░░░░░░░░█░░
                                                          ██████▀░░░░▀▀██████▀░░░﻿ 


 /*I/O CONNECT:
 * MotorDriverIN1 to digital pin 7
 * MotorDrive to digital pin 6
 * MotorEN1 to digital pin 5
 * MotorEN2 to digital pin 3
 * MotorDriverIN3 to digital pin 4
 * MotorDriveIN4 to digital pin 2
 * SensorLeft to digital pin 8
 * SensorRight to digital pin 9
 * UltrasonicEco to digital pin 12
 * UltrasonicTrig to digital pin 10
 * ServoMotor to digital pin 11
 */
//------------------------------------------------------------------------- INCLUDES -----------------------------------------------------------------------------------

#include<Servo.h>                      //Use this library to control the servo motor

//------------------------------------------------------------------------- INSTANCES -----------------------------------------------------------------------------------

Servo myServo;
 
#define MotorDriverIN1 7    //  | This pin is used to control the direction of the first Motor
#define MotorDriverIN2 6    //  | This pin is used to control the direction of the first Motor
#define MotorEN1 5          //  | This pin is used to Enable or disable the rotation of the first Motor
#define MotorEN2 3          //  | This pin is used to Enable or disable the rotation of the second Motor
#define MotorDriverIN3 4    //  | This pin is used to control the direction of the second Motor
#define MotorDriverIN4 2    //  | This pin is used to control the direction of the second Motor
#define SensorLeft1 18      //  | This pin is used to Read the digital signal from the first Left IR sensor
#define SensorLeft2 8       //  | This pin is used to Read the digital signal from the second Left IR sensor
#define SensorRight1 17     //  | This pin is used to Read the digital signal from the first Right IR sensor
#define SensorRight2 9      //  | This pin is used to Read the digital signal from the second Right IR sensor
#define UltrasonicEco 12    //  | We provided this connector if you want to make and obstacle detector robot
#define UltrasonicTrig 10   //  | We provided this connector if you want to make and obstacle detector robot
#define ServoMotor 11       //  | We provided this connector if you want to make and obstacle detector robot
#define LedPower 13         //  | This pin is used to control the LED power
#define LedObstacle 14      //  | This pin is used to control the LED for Obstacle detection
#define LedRightMotor 15    //  | This pin is used to control the LED that indicates the Right motor rotation
#define LedLeftMotor 16     //  | This pin is used to control the LED that indicates the Left motor rotation

//------------------------------------------------------------------ STARTING THE SETUP FUNCTION -----------------------------------------------------------------------
void setup() 
{
                                      //************************************* I/O pins configuration **************************************************
  pinMode(MotorDriverIN1,OUTPUT);
  pinMode(MotorDriverIN2,OUTPUT);
  pinMode(MotorEN1,OUTPUT);
  pinMode(MotorEN2,OUTPUT);
  pinMode(MotorDriverIN3,OUTPUT);
  pinMode(MotorDriverIN4,OUTPUT);
  pinMode(ServoMotor,OUTPUT);

  pinMode(SensorLeft1,INPUT);
  pinMode(SensorLeft2,INPUT);
  pinMode(SensorRight1,INPUT);
  pinMode(SensorRight2,INPUT);
  pinMode(UltrasonicEco,INPUT);
  pinMode(UltrasonicTrig,OUTPUT);

  pinMode(LedPower,OUTPUT);
  pinMode(LedRightMotor,OUTPUT);
  pinMode(LedLeftMotor,OUTPUT);
  pinMode(LedObstacle,OUTPUT);

  myServo.attach(11);
                                    //**************************** Output pins initial value pins configuration ***************************************
  digitalWrite(MotorEN1,LOW);
  digitalWrite(MotorEN2,LOW);
  digitalWrite(MotorDriverIN1,HIGH);
  digitalWrite(MotorDriverIN2,LOW);
  digitalWrite(MotorDriverIN3,HIGH);
  digitalWrite(MotorDriverIN4,LOW);
  
  digitalWrite(LedPower,HIGH);
  digitalWrite(LedRightMotor,LOW);
  digitalWrite(LedLeftMotor,LOW);
  digitalWrite(LedObstacle,LOW);
  
  myServo.write(90);
}
//------------------------------------------------------------------- ENDING THE SETUP FUNCTION -----------------------------------------------------------------------

//------------------------------------------------------------------- STARTING THE LOOP FUNCTION ----------------------------------------------------------------------

void loop()
{
  if((digitalRead(SensorLeft1)==LOW && digitalRead(SensorLeft1)==LOW) && (digitalRead(SensorRight1)==HIGH || digitalRead(SensorRight2)==HIGH))
  {
    motorDrive("leftMotor","forward","fast");
    motorDrive("rightMotor","stop","slow");
  }
  if((digitalRead(SensorRight1)==LOW && digitalRead(SensorRight1)==LOW) && (digitalRead(SensorLeft1)==HIGH || digitalRead(SensorLeft1)==HIGH))
  {
    motorDrive("rightMotor","forward","fast");
    motorDrive("leftMotor","stop","slow");
  }
  if(digitalRead(SensorRight1)==LOW && digitalRead(SensorLeft1)==LOW && digitalRead(SensorRight2)==LOW && digitalRead(SensorLeft2)==LOW)
  {
    motorDrive("rightMotor","stop","fast");
    motorDrive("leftMotor","stop","fast");
  }
  if(digitalRead(SensorRight1)==HIGH && digitalRead(SensorLeft1)==HIGH)
  {
    motorDrive("rightMotor","forward","fast");
    motorDrive("leftMotor","forward","fast");
  }
}

//------------------------------------------------------------------- ENDING THE LOOP FUNCTION ------------------------------------------------------------------------------

//------------------------------------------------------------------- STARTING THE motorDrive FUNCTION ----------------------------------------------------------------------
/* This function will help you to drive your DC motors using L293D H-bridge IC.  
 * The function takes three arguments:
 * - motor: which can be leftMotor or rightMotor
 * - commande: to indicate the movement (you can selecte a forward movement; backward movement or stop)
 * - Speed: to set the motor speed (you can choose fast or slow speed)
 */
void motorDrive(String motor, String commande, String Speed)
{
  int drivePinEn=0;
  int drivePinIN1=0;
  int drivePinIN2=0;
  int LedPin=0;
  int motorSpeed;
  
  if(motor=="leftMotor")
  {
    drivePinEn=MotorEN1;
    drivePinIN1=MotorDriverIN1;
    drivePinIN2=MotorDriverIN2;
    LedPin=LedLeftMotor;
  }
  if(motor=="rightMotor")
  {
    drivePinEn=MotorEN2;
    drivePinIN1=MotorDriverIN3;
    drivePinIN2=MotorDriverIN4;
    LedPin=LedRightMotor;
  }  
  if(Speed=="fast")
  {
    motorSpeed=250;
  }
  else
  {
    motorSpeed=100;
  }
  if(commande=="forward")
  {
    analogWrite(drivePinEn,250);
    digitalWrite(drivePinIN1,HIGH);
    digitalWrite(drivePinIN2,LOW);
    digitalWrite(LedPin,HIGH);
  }
  if(commande=="backward")
  {
    analogWrite(drivePinEn,motorSpeed);
    digitalWrite(drivePinIN1,LOW);
    digitalWrite(drivePinIN2,HIGH);
    digitalWrite(LedPin,HIGH);
  }
  if(commande=="stop")
  {
    digitalWrite(drivePinEn,LOW);
    digitalWrite(LedPin,LOW);
  }
}
//------------------------------------------------------------------- ENDING THE motorDrive FUNCTION ------------------------------------------------------------------------------

