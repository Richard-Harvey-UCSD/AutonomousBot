/* Potentiometer Code UCSD ECE 5 Lab 4*/
 /* Declare Variables for Potentiometer */
   const int S_pin = A0; // proportional - analog pin 0
   const int P_pin = A1; // proportional - analog pin 1
   const int I_pin = A2; // integral - analog pin 2
   const int D_pin = A3; // derivative - analog pin 3
   int Sp = 0; // speed gain coeficient
   int kP = 0; // proportional gain coeficient
   int kI = 0; // integral gain coeficient
   int kD = 0; // derivative gain coeficient

void setup() {                /* Setup - runs once (when power is supplied or after reset) */

  Serial.begin(9600);        // For serial communication set up  
}

void loop() {                /* Loop - loops forever (until unpowered or reset) */
 
  ReadPotentiometers();     // Call on user-defined function to read Potentiometer values
 
  Print();                  // Call on user-defined function to print values from potentiometers

}

// ************************************************************************************************* //
// function to read and map values from potentiometers
void ReadPotentiometers()
{
      Sp = map(analogRead(S_pin),0,1023,0,100);
      kP = map(analogRead(P_pin),0,1023,0,100);
      kI = map(analogRead(I_pin),0,1023,0,100);
      kD = map(analogRead(D_pin),0,1023,0,100);
}  

// ************************************************************************************************* //
// function to print values of interest
void Print()
{

 
 Serial.print(Sp); Serial.print(" ");
 Serial.print(kP); Serial.print(" ");
 Serial.print(kI); Serial.print(" ");
 Serial.println(kD); 
 
 delay(200); //just here to slow down the output for easier reading if desired
 
}

