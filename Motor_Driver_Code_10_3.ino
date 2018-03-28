/* Motor Driver Code UCSD ECE 5 Lab 4*/
// Libraries for Motor
#include <Wire.h>
#include <Adafruit_MotorShield.h>  // Must add libary - see MotorShield Manual
//https://cdn-learn.adafruit.com/downloads/pdf/adafruit-motor-shield-v2-for-arduino.pdf

// Initialize Motors
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Motor1 = AFMS.getMotor(1); // Motors can be switched here (1) <--> (2)
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);

//Set Initial Speed of Motors
int M1Sp = 120; // initial speed may vary and later can be increased with Sp potentiometer
int M2Sp = 120;

//Set LED Pin
int led_Pin = 53;  // can change to another digital pin and connect extra LED to me more easily seen


// setup - runs once
void setup(){
 Serial.begin(9600);
 AFMS.begin(); //for Motor
 
 pinMode(led_Pin, OUTPUT); // set pin mode to output voltage
      // Gives you a moment before tank actually moves
   for (int waitii = 0; waitii < 20; waitii++) { 
      digitalWrite(led_Pin, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(100);            // wait for 100 milliseconds
         digitalWrite(led_Pin, LOW);  // turn the LED off by making the voltage LOW
         delay(100);            // wait for 100 milliseconds
       }        
 }


// loop - loops forever
void loop(){ 

    // Start Motors in forward direction
  Motor1->setSpeed(M1Sp);
  Motor1->run(FORWARD);
  Motor2->setSpeed(M2Sp);
  Motor2->run(FORWARD);
     delay(3000);             // let run forward for 3 seconds

    // Start Motors in backward direction
  Motor1->setSpeed(M1Sp);
  Motor1->run(BACKWARD);
  Motor2->setSpeed(M2Sp);
  Motor2->run(BACKWARD);
     delay(3000);             // let run backward for 3 seconds  

    // Stop Motors
  Motor1->setSpeed(M1Sp);
  Motor1->run(RELEASE);
  Motor2->setSpeed(M2Sp);
  Motor2->run(RELEASE);
     delay(3000);           // stop for 3 seconds 

     // Turn Sharp Right
  Motor1->setSpeed(M1Sp);
  Motor1->run(FORWARD);
  Motor2->setSpeed(M2Sp);
  Motor2->run(BACKWARD);
     delay(3000);             // let run forward for 3 seconds

     // Turn Sharp Left
  Motor1->setSpeed(M1Sp);
  Motor1->run(BACKWARD);
  Motor2->setSpeed(M2Sp);
  Motor2->run(FORWARD);
     delay(3000);             // let run forward for 3 seconds
     
     // Stop Motors
  Motor1->setSpeed(M1Sp);
  Motor1->run(RELEASE);
  Motor2->setSpeed(M2Sp);
  Motor2->run(RELEASE);
     delay(3000);           // stop for 3 seconds 

     // Turn Long Right
  Motor1->setSpeed(M1Sp);
  Motor1->run(FORWARD);
  Motor2->setSpeed(M2Sp);
  Motor2->run(RELEASE);
     delay(3000);             // let run forward for 3 seconds

     // Turn Long Left
  Motor1->setSpeed(M1Sp);
  Motor1->run(RELEASE);
  Motor2->setSpeed(M2Sp);
  Motor2->run(FORWARD);
     delay(3000);             // let run forward for 3 seconds

     // Stop Motors
  Motor1->setSpeed(M1Sp);
  Motor1->run(RELEASE);
  Motor2->setSpeed(M2Sp);
  Motor2->run(RELEASE);
     delay(3000);           // stop for 3 seconds 


 }

