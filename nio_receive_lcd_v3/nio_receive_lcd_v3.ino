/* 
 * rf12 pin configuuration
 * SDI - DIGITAL 11
 * N SEL - DIGITAL 10
 * N IRQ - DIGITAL 2
 * SCK - DIGITAL 13
 * SDO - DIGITAL 12
*/
/*Arduino pin configuration
 * Seainltet-DIGITAL 3
 * Analysis chamber(AC)-DIGITAL 4
 * Distilled water tank(DWT)-ANALOG 3
 * Drain water DIGITAL 1
 * Conductivity DIGITAL 0
 * Limit switch Lower(lowerSW) ANALOG 1
 * Limit switch Upper(upperSW) ANALOG 2
 * */
 /* A3967 STEPPER MOTOR CONFIGURATION
 * DIR_PIN-DIGITAL 6
 * STEP_PIN-DIGITAL 5
 * SLEEP-DIGITAL 7
 * MS1-DIGITAL 8
 * MS2-DIGITAL 9
 * ENABLE-ANALOG 0
 */
 /*Step mode  Table
  * MS1         MS2      STEPS
  *LOW         LOW       Full steps
  *LOW         HIGH      quater steps
  *HIGH        LOW       Half step
  *HIGH        HIGH      eighth steps
  *
  */
  
#include <Wire.h>
#include "rgb_lcd.h"
#include <JeeLib.h>
#define DIR_PIN 6                                            //declaring the  Direction pin as constant for clock wise and anticlock wise rotation
#define STEP_PIN 5                                           //declaring the  Step pin as constant to give number of steps
#define SLEEP 7                                              //declaring the Sleep pin as constant for motor stop
#define MS1 8                                                //declaring the ms1 pin for full steps,half steps and quater steps
#define MS2 9                                                //declaring the ms2 pin for full steps,half steps and quater steps 
#define ENABLE A0                                            //declaring the enable pin to start or stop the motor
#define rf_freq RF12_433MHZ                                  //sets the frequency of the rf12 module communication to 868MHZ
int Seainlet=3,AC=4,DWT=A3,Drain=1;                         //Declaring the pins the for the valves
int conductivity=0;                                         //using the analog pin 5 to read the conductivity probe sensor value   
int lowerSW=A1, upperSW=A2;                                          //declaring the Limit switch forward(lowerSW) and Limit switch backward(upperSW) to off the motor 
typedef struct{int Volume,Sensor;}                           //analog valuess to be sent
DataTxRx;                                                    //name of the structure
DataTxRx Packetin,Packetout;                                 //instance of the structure(payloadtx)
int node_id=10;                                              //sets the node id of the rf12 module to 10
const int networkgroup=135;                                  //sets the network group of the rf12 module to 33
int flag=0;                                                  //
int steps;                  //defining the steps variable which stores the no of steps required for the motor to get the water sample   
int switch1,switch2;        //defining the switch1,switch2 variable to read the limitswitch i.e lowerSW,upperSW                        
rgb_lcd lcd;
void setup () { 
    Serial.begin(57600);
    lcd.begin(16, 2);
    lcd.setRGB(255, 99, 71);                                     //
    lcd.clear();
    delay(1000);
    Serial.println("Receive");
    lcd.print("Receive");
    delay(1000);//   
    rf12_initialize(node_id,rf_freq, networkgroup);          //initializing the rf 12 module with its node id and its network group
    lcd.clear();
    lcd.setRGB(178, 255, 102);
    Serial.println("rf initialized"); 
    lcd.print("rf initialized");
    delay(1000);
    lcd.clear();
    lcd.print(node_id);
    delay(1000);
    lcd.clear();
    lcd.print(networkgroup);
    delay(1000);
    pinMode(Seainlet,OUTPUT);                               //Setting the type of pin
    pinMode(AC,OUTPUT);                                     //Setting the type of pin
    pinMode(DWT,OUTPUT);                                    //Setting the type of pin
    pinMode(Drain,OUTPUT);                                  //Setting the type of pin
    pinMode(DIR_PIN, OUTPUT);                               //Setting the type of pin
    pinMode(STEP_PIN, OUTPUT);                               //Setting the type of pin
    pinMode(SLEEP, OUTPUT);                                  //Setting the type of pin 
    pinMode(MS1, OUTPUT);                                    //Setting the type of pin
    pinMode(MS2, OUTPUT);                                    //Setting the type of pin  
    pinMode(ENABLE, OUTPUT);                                 //Setting the type of pin
    pinMode(lowerSW,INPUT);                                      //Setting the type of pin
    pinMode(upperSW,INPUT);                                      //Setting the type of pin
    pinMode(conductivity,INPUT);                             //Setting the type of pin
    digitalWrite(DIR_PIN, LOW);                              //set the directionpin to low
    digitalWrite(STEP_PIN, LOW);           //set the step pin to low
    digitalWrite(SLEEP, HIGH);             //set the sleep to high
    digitalWrite(MS1, LOW);                  // set the ms1 pin for fullsteps
    digitalWrite(MS2, LOW);                 //set the ms2 pin for full steps
    digitalWrite(ENABLE, HIGH);             // motor off
    digitalWrite(DWT,LOW);                 //set the distilled water valve to low 
    digitalWrite(Drain,LOW);
    digitalWrite(AC,LOW);                  //set the Analysis chamber valve to low
    digitalWrite(Seainlet,LOW);          //set the Seainlet valve to low  
    }

void loop () {
    lcd.clear();
    lcd.setRGB(178, 255, 102); 
     
    if (rf12_recvDone() && rf12_crc == 0){ 
        lcd.setRGB(0, 255, 0);   
        int node=(rf12_hdr & 0x1F);
        Serial.print(node);                     //prints the node id
        lcd.clear();
        lcd.print("Receiving from");
        lcd.setCursor(0, 1);
        lcd.print(node);
        delay(5000);
        lcd.setCursor(0, 0);
        if(node==5){                           //checking whether the nodes are matched
           Packetin=*(DataTxRx*)rf12_data;      //unfolding  the packet and storing in packet in 
           lcd.clear();
           lcd.print("Volume is : ");
           lcd.setCursor(0, 1);        
           lcd.print(Packetin.Volume);     //printing the volume that as been recived
           delay(5000);
           lcd.setCursor(0, 0);
           int a=Packetin.Volume;               //storing the recieved volume to a variable    
           calsteps(a);                        //passing the variable to the function which calculates number of steps required to the stepper motor to get the given volume  
           lcd.setRGB(0, 0, 255);     
           lcd.clear();        
           if(lowerSW!=HIGH)                   //checking whether the lowerSW is LOW 
           {
              check();
           }
           lcd.print("First Cleaning");
           clean();                                         //clean function cleans the complete water sample module internally
           lcd.clear();          
           lcd.print("Second Cleaning");
           clean();                                        //clean function cleans the complete water sample module internally
           lcd.clear();
           lcd.print("Taking sample");
           digitalWrite(Seainlet,HIGH);                  //opening the input sea water valve to collect sample
           delay(1000);                                 
           Inletsteppermotor();                         //calling the function which collects sample
           digitalWrite(Seainlet,LOW);                  //closing the sea water valve
           delay(1000);
           lcd.clear();
           lcd.print("To the reaction");
           digitalWrite(AC,HIGH);                       // opening the Analysis chamber vale
           delay(1000);                                 //delay of 1 sec  
           Outletsteppermotor();                       //the outerstepper motor function is called to send the sample to the analysis chamber from the syringe
           digitalWrite(AC,LOW);                       //closing the analysis chamber valve
           delay(1000);                                //delay of 1 sec
           lcd.clear();
           lcd.print("Sensor value is:");
           lcd.setCursor(0, 1);
           Packetout.Sensor=analogRead(A5);           //Reading the sensor value of the Conductivity probe
           lcd.print(Packetout.Sensor);
           delay(5000);
           lcd.setCursor(0, 0);
           lcd.clear();
           lcd.print("To drain");
           digitalWrite(Drain,HIGH);                  // opening the drain chamber valve
           delay(5000);                               //delay of 1 sec
           digitalWrite(Drain,LOW);                   //closing the drain chamber valve
           delay(1000);                               //delay of 1 sec
           lcd.clear();
           lcd.print("Final Cleaning");
           clean();                                  //clean function cleans the complete water sample module internally
           flag=1;
        }
    }
    if(flag==1){
     if (rf12_canSend()) {  //after sensor value is obtained place it in the same block
        lcd.clear();
        lcd.print("Sending Packet");
        lcd.setCursor(0, 1); 
        lcd.print(Packetout.Sensor); //printing the sensor value
        delay(5000);
        lcd.setCursor(0, 0);
        rf12_sendStart(0, &Packetout, sizeof Packetout);//sending the data
        lcd.clear();
        lcd.print("Packet Sent");
        delay(1000);
        flag=0;// otherwise led blinking isn't visible
    }
    }
}

void Inletsteppermotor()                  //method for pulling the syringe
{
   
    delay(1000);                            // wait for 2 seconds
    digitalWrite(DIR_PIN, LOW);            // Set the direction.
    fullStep();
    motorOn();                             //motor starts
    for (int i = 0; i<steps; i++){  // Iterate for steps microsteps.
      switch1=digitalRead(upperSW); //reading the value of upperSW and storing in switch1
      if(switch1!=HIGH)
      {  
      pulseOut(STEP_PIN);
      delay(5);
      }
      else
      break;
    }
    motorOff();                        //motor stops
    delay(1000);
}
void Outletsteppermotor()          //method for pushing the syringe
{
     delay(1000);                     // wait for 3 seconds  
    digitalWrite(DIR_PIN, HIGH);     // Set the direction.
    fullStep();
    motorOn();                      // motor starts
    for (int i = 0; i<steps; i++){// Iterate for steps  microsteps.
      switch2=digitalRead(lowerSW);// reading the value of lowerSW and storing in switch2
      if(switch2!=HIGH)
      {
    pulseOut(STEP_PIN);
    delay(5);
    }
    else
    break;
    }
    motorOff();                       // motor stops  
    delay(1000);                   // wait for 2 seconds 
}
void calsteps(int vol){        //calcsteps calculates the number of steps required to get the given sample volume
  steps=vol;                     //taking the volume 
}
void check()           // to check the position of the syringe and to bring back intial position of syringe
{
  delay(1000);                     // wait for 3 seconds  
    digitalWrite(DIR_PIN, HIGH);     // Set the direction.
    fullStep();
    motorOn();                      // motor starts
    for (int i = 0;; i++){// Iterate for steps  microsteps.
      switch2=digitalRead(lowerSW);
      if(switch2!=HIGH)
      {
    pulseOut(STEP_PIN);
    delay(5);
    }
    else
    break;
    }
    motorOff();                       // motor stops  
    delay(1000);                   // wait for 2 seconds 
}

void clean()
{
  digitalWrite(DWT,HIGH);                 //opening the distilled water valve to clean the syringe
  delay(1000);                            //delay of 1 sec
  Inletsteppermotor();                   //calling the Inletstepper motor function to take the distilled water
  digitalWrite(DWT,LOW);                 // closing the distilled water valve 
  delay(1000);                           //delay of 1 sec
  digitalWrite(AC,HIGH);                //opening the analysis chamber valve
  delay(1000);                           //delay of 1 sec
  digitalWrite(Drain,HIGH);             //openin the drain chamber valve to send the distilled water to drain
  Outletsteppermotor();                 //calling the outletstepper motor to send the distilled water to AC to clean it   
  digitalWrite(AC,LOW);                 //closing the analysis the chamber valve
   
  delay(5000);                          //delay of 5 sec  
  digitalWrite(Drain,LOW);              // closing the drain chamber valve
  delay(1000);                           //delay of 1 sec 
}

void pulseOut(byte x)   {            // inverts state of pin, delays, then reverts state back
  byte z = digitalRead(x);
  delayMicroseconds(10);
  z = !z;                           // reverse state
  digitalWrite(x, z);
  z = !z;                           // return to original state
  digitalWrite(x, z);
}                                  // end pulsout()

void fullStep(void)   {
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
}

void motorOff(void)   {
  digitalWrite(ENABLE, HIGH);              //enable is high the motor stops
}

void motorOn(void)   {
  digitalWrite(ENABLE, LOW);               //enable is low then motor starts 
}
