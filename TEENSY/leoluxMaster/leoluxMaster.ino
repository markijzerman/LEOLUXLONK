#define panelAmnt 20
#define sensorAmnt 128
#define cPinAmnt  8
#define cTeensyAmnt 16
#define cByteAmnt 32

byte nth[sensorAmnt];
byte ccValues[panelAmnt];
byte rawSerial[cByteAmnt];
byte teensyData[cTeensyAmnt];

bool serialState = 0;
byte sCount = 0;

void setup() {
  Serial1.begin(9600);
  
  //turn on onboard LED
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);    

  for(int i = 0; i < sizeof(nth); i++){
    nth[i] = i;
  }//for
    
}

void loop() {  

  //if there is Serial data availible 
  if (Serial1.available() > 0) {

    byte incomingbyte = Serial1.read();

    //if first bit is detected
    if(serialState == 0){
      if(incomingbyte == 255){
        serialState = 1;
      }//if
    }else{
      if(incomingbyte > -1 && incomingbyte <= 128){
        //if uneven << 4
        rawSerial[sCount] = incomingbyte << ((sCount & 1) << 2);
        //number of incoming data
        sCount++;
      }else{
        serialState = 0;  
        sCount = 0;        
      }//if
    }//if
    
  }//if

  //join bytes
  for(int i = 0; i < cTeensyAmnt; i++){
    teensyData[i] = rawSerial[i<<1] | rawSerial[i+1];
    Serial.print(teensyData[i]);
    Serial.print(" ");
  }//for
  Serial.println();
  
  for(int i = 0; i < panelAmnt; i++){
    usbMIDI.sendControlChange(i, ccValues[i], 0);
  }//for
  
}//loop

//function for converting bytes to bits
bool * byteToBits(byte b){

  static bool bArray[cPinAmnt];
  for(int i = 0; i < 8; i++){
    bArray[i] = ((128 >> i) & b) >> (7-i);
  }//for

  return bArray;
}//byteToBits

//function for converting an array of bits to a byte
byte bitsToByte(bool * bitArray){

  byte b = 0;
  for(int i = 0; i < 8; i++){
    b = (b << 1) | (bitArray[i] & 1);
  }//for

  return b;

}//bitsToByte

