/* Photoresistor Code UCSD ECE 5 Lab 4 */
  /* Variables for Light Sensors*/
   int LDR_Pin0 = A8 ; // analog pin  8
   int LDR_Pin1 = A9 ; // analog pin  9
   int LDR_Pin2 = A10; // analog pin 10
   int LDR_Pin3 = A11; // analog pin 11
   int LDR_Pin4 = A12; // analog pin 12
   int LDR_Pin5 = A13; // analog pin 13
   int LDR_Pin6 = A14; // analog pin 14

   // Initialize Photo Resistor Variables
   int LDR0 = 0, LDR1 = 0, LDR2 = 0, LDR3 = 0, LDR4 = 0, LDR5 = 0, LDR6 = 0; 
   
void setup() {

   Serial.begin(9600);        // For serial communication set up  

}

void loop() {

   ReadPhotoResistors(); // Read photoresistors and map to 0-100 based on calibration

   Print();              // Print values to serial monitor
}

// ************************************************************************************************* //
// function to read photo resistors
void ReadPhotoResistors()
{
 
 LDR0 = analogRead(LDR_Pin0);
   delay(2);
 LDR1 = analogRead(LDR_Pin1);
   delay(2);
 LDR2 = analogRead(LDR_Pin2);
   delay(2);
 LDR3 = analogRead(LDR_Pin3);
   delay(2);
 LDR4 = analogRead(LDR_Pin4);
   delay(2);
 LDR5 = analogRead(LDR_Pin5);
   delay(2);
 LDR6 = analogRead(LDR_Pin6);
   delay(2); 
}

// ************************************************************************************************* //
// function to print values of interest
void Print()
{

 Serial.print(LDR0); Serial.print(" ");
 Serial.print(LDR1); Serial.print(" ");
 Serial.print(LDR2); Serial.print(" ");
 Serial.print(LDR3); Serial.print(" ");
 Serial.print(LDR4); Serial.print(" ");
 Serial.print(LDR5); Serial.print(" ");
 Serial.println(LDR6);  
 
 delay(200); //just here to slow down the output for easier reading if desired
 
}

