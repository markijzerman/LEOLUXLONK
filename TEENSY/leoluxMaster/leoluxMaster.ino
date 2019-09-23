#define panelAmnt 20
#define sensorAmnt 128
#define cPinAmnt  8
#define cTeensyAmnt 16
#define cByteAmnt 2

byte nth[sensorAmnt];
byte ccValues[panelAmnt];
byte rawSerial[cByteAmnt];
bool teensyState[cTeensyAmnt][cPinAmnt];

bool serialState = 0;
byte sCount = 0;
byte ccId = 0;

void setup() {
  Serial1.begin(9600);
  
  //turn on onboard LED
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);    
}

void loop() {  

  //if there is Serial data availible 
  if (Serial1.available() > 0) {

    byte incomingbyte = Serial1.read();
    //if first bit is detected
    if(incomingbyte >= 200){
      ccId = incomingbyte-200;
      sCount=0;
    }else if(incomingbyte > -1){
      rawSerial[sCount] = incomingbyte;
      //number of incoming data
      sCount++;
    }
  }//if
    
  static byte oldSerial1 = rawSerial[0]; 
  static byte oldSerial2 = rawSerial[1]; 

  //if serial values change
  if(oldSerial1 != rawSerial[0] || oldSerial2 != rawSerial[1]){
    //first send the first bytes then the last bytes
    //order of the midi that is sent is important here!
    usbMIDI.sendControlChange(ccId, rawSerial[1], 2);  
    usbMIDI.sendControlChange(ccId, rawSerial[0], 1);
    oldSerial1 = rawSerial[0]; 
    oldSerial2 = rawSerial[1]; 
  }//if

}//loop
