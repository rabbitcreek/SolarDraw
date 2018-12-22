/* 
This program operates the Solardraw.  The setup section will draw a figure placed into a digital Matrix and the
loop function will operate the joystick.  If you want to go immediatly to joystic control just eliminate  dotMatrix.
*/


#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Servo myservo;  
int tick=0;
int tock=0;
int upDown=0;
int leftRight=0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//This setup uses two stepper motors each with 20 steps per revolution--18 degrees
Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(20, 1);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(20, 2);
/*dotMatrix contains the drawing parameters.  Each drawing step consists of a new row with six parameters (columns).  The first two elements are if x and
 * y are moving--1 or a decimal or 0.  Next two  are direction x and y (1 is forward, 0 is backward). Next element is how many turns and last element is 
 * open (one) or closed (zero) shutter.  With this you can draw lines of different slopes and different stop and start points.  The Burning Man
 * logo is included.  There is no reset start position and I program it to return to its initial spot or do it manually with joystick.
 * If you do a new design I usually use graph paper and count squares--the operating size is about 3 inches  and each inch represents 40 turns of the 
 * stepper-screw
 * 
 */
int dotMatrix[13][6]={
  {1,1,0,0,10,1},
  {0,1,0,0,10,1},
  {1,1,1,0,10,1},
  {1,0,0,0,30,0},
  {1,1,1,1,10,1},
  {0,1,0,1,10,1},
  {1,1,0,1,10,1},
  {1,0,1,0,14,0},
  {1,1,0,0,5,1},
  {1,1,1,0,5,1},
  {1,1,1,1,5,1},
  {1,1,0,1,5,1},
  {1,0,1,0,16,0}
};


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  myservo.attach(9); `

  AFMS.begin();  // create with the default frequency 1.6KHz
  //I have not had much luck moving these steppers faster--you can reduce speed by using 
  //INTERLEAVE OR MICROSTEP
  
  myMotor1->setSpeed(10);
  myMotor2->setSpeed(10);  // 10 rpm  
  //This delay is built in so you can reprogram without having it start running another burn partially
  delay(10000); 
  for(int i=0;i<13;i++)
{ 
   if(dotMatrix[i][5])myservo.write(0);//opens shutter
    tick=dotMatrix[i][0];
    tock=dotMatrix[i][1];
    
    for(int turns=0;turns<(dotMatrix[i][4]*2);turns++)
    {
    if(tick)
    {
    if(dotMatrix[i][2]) myMotor2->step(20*tick*2,FORWARD,DOUBLE);
     else myMotor2->step(20*tick*2,BACKWARD,DOUBLE);
    }
     
     
     if(tock)
     {
     if(dotMatrix[i][3])myMotor1->step(20*tock*2,FORWARD,DOUBLE);
     else myMotor1->step(20*tock*2,BACKWARD,DOUBLE);
     }
    }
     myservo.write(80);//closes shutter
     //Serial.println(tick,tock);
     myMotor1->release();
     myMotor2->release();
     delay(100);
}
}

void loop() {
/*
 * The loop function connects the Solardraw to the Joystick for control.  I originallhy programmend it for speed/resistance function but
 * found it not very helpful.  This version operates it at full speed and just controls direction.  The shutter opens and closes with each press.
 */




  
  tick=0;
  upDown=analogRead(A0);
  leftRight=analogRead(A1);
  if(upDown<450)tick=1;
    
  else if(upDown>550)tick=2;
    
  else if(leftRight<450) tick=3;
    
  else if(leftRight>550)tick=4;
   
  switch(tick){
    //Serial.print("tick=");
    //Serial.println(tick);
    case 1://myservo.write(0) opens shutter
    myservo.write(0);
    myMotor1->step(20,BACKWARD,DOUBLE);
    myMotor1->release();
    break;
    case 2:
    myservo.write(0);
    myMotor1->step(20, FORWARD, DOUBLE);
    myMotor1->release();
    break;
    case 3:
    myservo.write(0);
    myMotor2->step(20,BACKWARD, DOUBLE);
    myMotor2->release();
    break;
    case 4:
    myservo.write(0);
    myMotor2->step(20,FORWARD,DOUBLE);
    myMotor2->release();
    break;

   

    
    default:
    myservo.write(80); //closes shutter
    myMotor1->release();//Shuts down motor
    myMotor2->release();//Shuts down motor
    
    
    
    
  }


 
   
  }
  
  
