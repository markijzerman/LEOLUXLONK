/*------------------------------------
leolux master teensy code
Code for receiving data from the leolux reader teensys
The data received is passed trough as midi to a max/msp patch
------------------------------------*/

#define cByteAmnt 2
#define cTeensyAmnt 16

byte rawSerial[cByteAmnt];
byte oldData[cTeensyAmnt][cByteAmnt];

bool serialState = 0;
byte sCount = 0;
byte ccId = 0;

void setup() {
  //start serial communications with other teenys
  Serial1.begin(9600);
  
  //turn on onboard LED
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);    
}

void loop() {  

  //if there is Serial data availible 
  if (Serial1.available() > 0) {
    //read serial data
    byte incomingbyte = Serial1.read();

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
    }
       
  }//if
    
}//loop


