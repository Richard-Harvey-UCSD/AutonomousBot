/* ************************************************************************************************* */
                       // * UCSD ECE 5 Lab 4 Code: Line Following Robot with PID * //
/* ************************************************************************************************* */

//   This is code for your PID controlled line following robot.  
//
//
//
//
// Code Table of Contents
    //  1)  Declare Variables     - declares many variables as global variables so each variable can be accessed from every function
    //  2)  Setup (Main)          - runs once at beginning when you press button on arduino or motor drive or when you open serial monitor
    //  3)  Loop  (Main)          - loops forever calling on a series of functions   
    //  4)  Calibration           - makes white = 0 and black = 100 (a few seconds to prep, a few seconds on white, a few seconds to move to black, a few seconds of black)
    //  5)  Read Potentiometers   - reads each potentiometer
    //  6)  Run Motors            - runs motors 
    //  7)  Read Photoresistors   - reads each photoresistor
    //  8)  Calculate Error       - calculate error from photoresistor readings
    //  9)  PID Turn              - takes the error and implements PID control
    //  10) Print                 - used for debugging but should comment out when not debugging because it slows down program

// ************************************************************************************************* //
// Declare Variables


// Variables and Libaries for Motor
    #include <Wire.h>
    #include <Adafruit_MotorShield.h>
  
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);  // you may switch 1 with 2 if needed (if you see motors responding to error in the opposite way they should be.
    Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);  // you you may use 3 and 4 instead of 1 and 2 if you use 2 other motor outlets
  
    //int M1Sp = 20, M2Sp = 20;   // this is the nominal speed for the motors when not using potentiometer  
    int M1Sp = 50, M2Sp = 50;
    int M1SpeedtoMotor, M2SpeedtoMotor;

  // Variables for Potentiometer
    const int S_pin = A0; //proportional control
    const int P_pin = A1; //proportional control
    const int I_pin = A2; //integral control
    const int D_pin = A3; //derivative control
    int       SpRead = 0; int Sp; //Speed Increase 
    int       kPRead = 0; //proportional gain
    int       kIRead = 0; //integral gain
    int       kDRead = 0; //derivative gain

  // Variables for Light Sensors
    int LDR_Pin[7] = {A8,A9,A10,A11,A12,A13,A14};   // Many arrays are used in this code to simplify it
    int LDR[7];                                     // these are variables that have multiple elements to each variable name
                                                    // LDR_Pin hold 7 values and A8 is the 0th element and A11 is the 4th element
   
  // Calibration Variables
    int   led_Pin = 53;      /// This is a led set up to indicate what part of the calibration you are on.  
    float Mn[7];             ///     You could use pin 13 instead which is a built in LED to Arduino
    float Mx[7];
    float LDRf[7] = {0.,0.,0.,0.,0.,0.,0.};


    int   MxRead;
    int   MxIndex;
    float AveRead;
    int   CriteriaForMax;
    float WeightedAve;
    int   ii;
    int   im0,im1,im2;

  // For Motor/Control
    int   Turn, M1P = 0, M2P = 0;
    float error, lasterror = 0, sumerror = 0;
    float kP,kI,kD;

    const int LeftOvershootLED = 51;
    const int RightOvershootLED = 52;
    const int CenteredLED = led_Pin;


// ************************************************************************************************* //
// setup - runs once
void setup()
{
 
      Serial.begin(9600);        // For serial communication set up  
      AFMS.begin();              // For motor setup
      pinMode(led_Pin, OUTPUT);  // Note that all analog pins used are INPUTs by default so don't need pinMode
      pinMode(LeftOvershootLED, OUTPUT);
      pinMode(RightOvershootLED, OUTPUT);
      //pinMode(CenteredLED, OUTPUT);
    
      Calibrate();               // Calibrate black and white sensing
    
      ReadPotentiometers();      // Read potentiometer values (Sp, P, I, & D)
    
      delay(2000);
      
      RunMotors();               // Starts motors forward and strait depending on Sp (Speed from potentiometer) and M1Sp/M2Sp (Nominal values)
                    
}  // end setup()

// ************************************************************************************************* //
// loop - runs/loops forever
void loop()
{
  
      ReadPotentiometers(); // Only if you want to see Potentiometers working in set up as you run the line following
    
      ReadPhotoResistors(); // Read photoresistors and map to 0-100 based on calibration
    
      CalcError();
      
      PID_Turn();         // PID Control and Output to motors to turn

      RunMotors();        // Uses info from
    
      Print();          // Print values to serial monitor  //currently commented out but could be good for debugging =)
                        
}  // end loop()

// ************************************************************************************************* //
// function to calibrate
void Calibrate()
 { 
  // wait to make sure in position
     for (int calii = 0; calii < 4; calii++) 
      {
        digitalWrite(led_Pin, HIGH);   // turn the LED on 
        delay(100);                    // wait for 0.1 seconds
        digitalWrite(led_Pin, LOW);    // turn the LED off
        delay(900);                    // wait for 0.9 seconds
      }
 
   // Calibration
    // White Calibration
    int numMeas = 40;
    for (int calii = 0; calii < numMeas; calii++) 
       {  
              digitalWrite(led_Pin, HIGH);   // turn the LED on 
              delay(100);                    // wait for 0.1 seconds
              digitalWrite(led_Pin, LOW);    // turn the LED off
              delay(100);                    // wait for 0.1 seconds

            for ( int ci = 0; ci < 7; ci++ )
               {
                  LDRf[ci] = LDRf[ci] + (float) analogRead(LDR_Pin[ci]);
                    delay(2);            
               } 
       }  

       for ( int cm = 0; cm < 7; cm++ )
          {
              Mn[cm] = round(LDRf[cm]/(float)numMeas);  // take average  
              LDRf[cm]=0.;  
          }
                  
    // Time to move from White to Black Surface
    for (int calii = 0; calii < 10; calii++) 
      {
        digitalWrite(led_Pin, HIGH);    
        delay(100);                   
        digitalWrite(led_Pin, LOW);    
        delay(900);                  
      }
        
      // Black Calibration
      for (int calii = 0; calii < numMeas; calii++) 
         {      
              digitalWrite(led_Pin, HIGH);   
              delay(100);                   
              digitalWrite(led_Pin, LOW);    
              delay(100);                   
                  
              for ( int ci = 0; ci < 7; ci++ )
                 {
                    LDRf[ci] = LDRf[ci] + (float) analogRead(LDR_Pin[ci]);
                      delay(2);            
                 }                  
        }  
       for ( int cm = 0; cm < 7; cm++ )
          {
              Mx[cm] = round(LDRf[cm]/(float)numMeas);  // take average  
              LDRf[cm]=0.;  
          }
       
  }  // end Calibrate()

// ************************************************************************************************* //
// function to read and map values from potentiometers
void ReadPotentiometers()
{
       SpRead = map(analogRead(S_pin),0,1023,0,50); Sp=SpRead;
       kPRead = map(analogRead(P_pin),0,1023,0,10);
       kIRead = map(analogRead(I_pin),0,1023,0,5);
       kDRead = map(analogRead(D_pin),0,1023,0,10);
}    // end ReadPotentiometers()
  
// ************************************************************************************************* //
// function to start motors using nominal speed + speed addition from potentiometer
void RunMotors()
{     
       M1SpeedtoMotor = min(M1Sp+Sp+M1P,255); // limits speed to 255     
       M2SpeedtoMotor = min(M2Sp+Sp+M2P,255); // remember M1Sp & M2Sp is defined at beginning of code (default 60)
        
       Motor1->setSpeed(abs(M1SpeedtoMotor)); 
       Motor2->setSpeed(abs(M2SpeedtoMotor));
       
       if (M1SpeedtoMotor > 0) 
         {
             Motor1->run(FORWARD);
         }
       else 
         {
             Motor1->run(BACKWARD);
         }
       
       if (M2SpeedtoMotor > 0) 
         {
             Motor2->run(FORWARD);
         }
       else 
         {
             Motor2->run(BACKWARD);
         }
            
}  // end RunMotors()

// ************************************************************************************************* //
// function to read photo resistors, map from 0 to 100, and find darkest photo resitor (MxIndex)
void ReadPhotoResistors()
 {
     for (int Li = 0; Li < 7; Li++)
        {
            LDR[Li] = map(analogRead(LDR_Pin[Li]), Mn[Li], Mx[Li], 0, 100);
              delay(2);            
        }
 }   // end ReadPhotoResistors()

// ************************************************************************************************* //
// Calculate error from photoresistor readings
void CalcError()
 {
        MxRead = -99;
        AveRead = 0.0;
        for (int ii=0;ii<7;ii++)
            {
              if (MxRead < LDR[ii])
                {
                  MxRead = LDR[ii];
                  MxIndex = -1*(ii-3);
                  im1 = (float)ii;
                }
              AveRead = AveRead+(float)LDR[ii]/7.;
            }       
      CriteriaForMax = 2;  // max should be at least twice as big as the other values     
      if (MxRead > CriteriaForMax*AveRead)
         {
            if (im1!=0 && im1!=6)
              {
               im0 = im1-1;
               im2 = im1+1;
               WeightedAve = ((float)(LDR[im0]*im0 + LDR[im1]*im1 + LDR[im2]*im2))/((float)(LDR[im0]+LDR[im1]+LDR[im2]));
               error = -1*(WeightedAve - 3);
              }
            else if (im1 == 0)
               {
                im2 = im1+1;
                WeightedAve = ((float)(LDR[im1]*im1 + LDR[im2]*im2))/((float)(LDR[im1]+LDR[im2]));
                error = -1*(WeightedAve - 3);
               }
             else if (im1 == 6)
               {
                im0 = im1-1;
                WeightedAve = ((float)(LDR[im0]*im0 + LDR[im1]*im1))/((float)(LDR[im0]+LDR[im1]));
                error = -1*(WeightedAve - 3);
               }           
         }    

      if (error > 0.6){                     // Cart is far left off track
        digitalWrite(CenteredLED, LOW);
        digitalWrite(LeftOvershootLED, HIGH);
        digitalWrite(RightOvershootLED, LOW);
      }
      else if (error < -0.6){               // Cart is far right off track
        digitalWrite(CenteredLED, LOW);
        digitalWrite(LeftOvershootLED, LOW);
        digitalWrite(RightOvershootLED, HIGH);
      }
      else {
        digitalWrite(CenteredLED, HIGH);    // Cart is centered on track
        digitalWrite(LeftOvershootLED, LOW);
        digitalWrite(RightOvershootLED, LOW);
      }
      
 }  // end CalcError()


// ************************************************************************************************* //
// function to make a turn ( a basic P controller)
void PID_Turn()
{
       // *Read values are between 0 and 100, scale to become PID Constants
       // ****!!!! These values you will most likely want to change - Every robot is different !!!!
       //kP = (float)kPRead/1.;      // each of these scaling factors can change depending on how influential you want them to be
       kP = (float)kPRead*5;
       kI = (float)kIRead/100.;    // the potentiometers will also scale them
       kD = (float)kDRead/1.;
  // error holds values from -3 to 3 ( 0 is your target / middle sensor)
  
  Turn = error*kP + sumerror*kI + (error - lasterror)*kD; //PID!!!!!
       
       sumerror = sumerror + error;
       if (sumerror > 5) {sumerror = 5;} // prevents integrator wind-up
       else if (sumerror < -5) {sumerror = -5;}
       
       lasterror = error;
       
  
       if      (Turn < 0) { M1P = Turn; M2P = -Turn;}   //One motor becomes slower and the other faster
       else if (Turn > 0) {M1P = Turn ; M2P = -Turn;} 
       else               {M1P = 0    ; M2P = 0;}                 
}  // end PID_Turn()


// ************************************************************************************************* //
// function to print values of interest
void Print()
{
        Serial.print(SpRead); Serial.print(" ");  // Initial Speed addition from potentiometer
        Serial.print(kP); Serial.print(" ");      // PID values from potentiometers after scaling 
        Serial.print(kI); Serial.print(" ");
        Serial.print(kD); Serial.print("    ");
      
        Serial.print(LDR[0]); Serial.print(" ");  // Each photo resistor value is shown
        Serial.print(LDR[1]); Serial.print(" ");
        Serial.print(LDR[2]); Serial.print(" ");
        Serial.print(LDR[3]); Serial.print(" ");
        Serial.print(LDR[4]); Serial.print(" ");
        Serial.print(LDR[5]); Serial.print(" ");
        Serial.print(LDR[6]); Serial.print("    ");  
        
        Serial.print(MxRead); Serial.print(" ");    // the maximum value from the photo resistors is shown again
        Serial.print(MxIndex);Serial.print("    "); // this is the index of that maximum (0 through 6) (aka which element in LDR)
        Serial.print(error); Serial.print("    ");  // this will show the calculated error (-3 through 3) 
        
        Serial.print(M1SpeedtoMotor); Serial.print(" "); // This prints the arduino output to each motor so you can see what the values (0-255)
        Serial.println(M2SpeedtoMotor);                  //  that are sent to the motors would be without actually needing to power/run the motors
        
       // delay(100); //just here to slow down the output for easier reading if wanted 
                     // ensure delay is commented when actually running your robot or this will slow down sampling too much
}  // end Print()

