/* Calibration Code UCSD ECE 5 Lab 4 */
// Variables and Libaries for Motor

  // Variables for Light Sensors
    int LDR_Pin[7] = {A8,A9,A10,A11,A12,A13,A14};   // Arrays are used top simplify the code
    int LDR[7];                                     // these are variables that have multiple elements to each variable name
                                                    // LDR_Pin hold 7 values and A8 is the 0th element and A11 is the 4th element 
  // Calibration Variables
    int led_Pin = 53;      /// This pin is for a led built into the Arduino that indicates what part of the calibration you are on 
                  /// You can use any digital pin like digital pin 31 with an LED connected for better visibility  
    float Mn[7];
    float Mx[7];
    float LDRf[7] = {0.,0.,0.,0.,0.,0.,0.};

    int   MxRead;
    int   MxIndex;
    float AveRead;
    int   CriteriaForMax;
    float WeightedAve;
    int   ii;
    int   im0,im1,im2;
    float error;
    
// ************************************************************************************************* //
// setup - runs once
void setup()
{
 
      Serial.begin(9600);        // For serial communication set up  
      pinMode(led_Pin, OUTPUT);  // Note that all analog pins used are INPUTs by default so don't need pinMode
    
      Calibrate();               // Calibrate black and white sensing
                 
}  // end setup()

// ************************************************************************************************* //
// loop - runs/loops forever
void loop()
{
  
      ReadPhotoResistors(); // Read photoresistors and map to 0-100 based on calibration
    
      CalcError();
            
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
 }  // end CalcError()

 // ************************************************************************************************* //
// function to print values of interest
void Print()
{
      
        Serial.print(LDR[0]); Serial.print(" ");  // Each photo resistor value is shown
        Serial.print(LDR[1]); Serial.print(" ");
        Serial.print(LDR[2]); Serial.print(" ");
        Serial.print(LDR[3]); Serial.print(" ");
        Serial.print(LDR[4]); Serial.print(" ");
        Serial.print(LDR[5]); Serial.print(" ");
        Serial.print(LDR[6]); Serial.print("    ");  
        
        Serial.print(MxRead); Serial.print(" ");    // the maximum value from the photo resistors is shown again
        Serial.print(MxIndex);Serial.print("    "); // this is the index of that maximum (0 through 6) (aka which element in LDR)
        Serial.print(error); Serial.println("    ");  // this will show the calculated error (-3 through 3) 
               
        delay(100); //just here to slow down the output for easier reading if wanted 
                     
}  // end Print()

