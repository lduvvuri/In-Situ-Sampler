/* 
 * rf12 pin configuuration
 * SDI - DIGITAL 11
 * N SEL - DIGITAL 10
 * N IRQ - DIGITAL 2
 * SCK - DIGITAL 13
 * SDO - DIGITAL 12
*/

#include <JeeLib.h>
//#include<SoftwareSerial.h>
#define rf_freq RF12_433MHZ                     //sets the frequency of the rf12 module communication to 868MHZ

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int volume;
Port leds (1);                                       //to use the Tx and Rx led 
MilliTimer sendTimer;                                //timer for sending the data
typedef struct{int Volume,Sensor;}              //analog valuess to be sent
DataTxRx;                                          //name of the structure
DataTxRx Packetin,Packetout;                                    //instance of the structure(payloadtx)
byte needToSend;                                    //this helps us to enable the rf12 module to send the signal
int node_id=5;                                     //sets the node id of the rf12 module to 10
const int networkgroup=135;                          //sets the network group of the rf12 module to 33


void setup () {
    Serial.begin(57600);
    Serial.println(57600);
    Serial.println("Send");    
    rf12_initialize(node_id,rf_freq, networkgroup);          //initializing the rf 12 module with its node id and its network group
    Serial.println("rf initialized");
    inputString.reserve(200);
}

void loop () {
    if (rf12_recvDone() && rf12_crc == 0) 
    {     
      Packetin=*(DataTxRx*)rf12_data;
        Serial.print(" Sensor value is : ");
        Serial.println(Packetin.Sensor);
    }
    serialEvent();

     if (stringComplete) {

     validate(inputString);
     Packetout.Volume = volume;
   if (rf12_canSend()) { 
        Serial.println("Sending Packet");
        rf12_sendStart(0, &Packetout, sizeof Packetout);//sending the data
        Serial.println("Packet Sent");
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
    
}

void serialEvent() {
  while (Serial.available()) {
   // Serial.println(stringComplete);
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == 'c') {
      stringComplete = true;
     // Serial.println(inputString);
    }
  }
}

boolean validate(String str){
   volume=0;
   for(int j=0;j<str.length();j++){
   
   char c =str.charAt(j); 
   int i;
   for(i=48;i<58;i++){
    if(i==(int)c){
      volume = (volume*10)+(i-48);
      break;
    }
   }
    if(i==58) return false;
   }
   
 return true;
} 


