/*------------------------------------
leolux master teensy code
Code for receiving data from the leolux reader teensys
The data received is passed trough as midi to a max/msp patch
------------------------------------*/

#define cByteAmnt 2
#define cTeensyAmnt 16
#define cTimerSpeed 1000

#include <Metro.h> 

Metro updateMetro = Metro(cTimerSpeed); //timer for update data

byte rawSerial[cByteAmnt];
byte oldData[cTeensyAmnt][cByteAmnt];

bool serialState = 0;
byte sCount = 0;
byte ccId = 0;

void setup() {
  //start serial communications with other teenys
  Serial1.begin(9600);

  for(int i = 0; i < cTeensyAmnt; i++){
    oldData[i][0] = 0;
    oldData[i][1] = 0;    
  }
  
  //turn on onboard LED
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);    
}

void loop() {  

  //if there is Serial data availible 
  if (Serial1.available() > 0) {
    //read serial data
    byte incomingbyte = Serial1.read();
    //Serial.println(incomingbyte);
    //if id byte is detected
    if(incomingbyte >= 200){
      ccId = incomingbyte-200;
      sCount=0;
    }else if(incomingbyte > -1){
      rawSerial[sCount] = incomingbyte;
      //number of incoming data
      sCount++;          
    }//if
    
    //send midi if paramaters have changed
    if(rawSerial[0] != oldData[ccId][0] || rawSerial[1] != oldData[ccId][1]){
      //first send the first bytes then the last bytes to max
      //order of the data that is send is important here!
      usbMIDI.sendControlChange(ccId, rawSerial[1], 2);  
      usbMIDI.sendControlChange(ccId, rawSerial[0], 1);
      oldData[ccId][0] = rawSerial[0];
      oldData[ccId][1] = rawSerial[1];   
    }//if

  //if there is no serial data availible 
  //send all data that is stored is send every second
  }else if(updateMetro.check() == 1) { 
    updateData();
  }//if
    
}//loop

void updateData(){
  for(int i = 0; i < cTeensyAmnt; i++){ 
    usbMIDI.sendControlChange(i, oldData[i][1], 2);  
    usbMIDI.sendControlChange(i, oldData[i][0], 1);    
  }//for
  
}

