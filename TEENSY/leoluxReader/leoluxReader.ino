#define cTeensyId 1
#define cPinAmnt  8
#define cSensTresh 500
#define cTeensyAmnt 16

const byte cPinArray[cPinAmnt] = {A9,A7,A5,A3,A1,A0,A2,A4};

int  sensorData[cPinAmnt];
bool sensorState[2][cPinAmnt];
byte sData1 = 0;
byte sData2 = 0;

void setup() {

  //initialize
  for(int i = 0; i < cPinAmnt; i++){
    pinMode(cPinArray[i],INPUT);
    sensorData[i] = 0;
    sensorState[0][i] = 0;
    sensorState[1][i] = 0;    
  }//for
  
  //turn on onboard LED
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);  
  
  //start serial communication
  Serial1.begin(9600);

}//loop

void loop() {

  //loop trough pins
  for(int i = 0; i < cPinAmnt; i++){
    //read sensor data
    sensorData[i] = analogRead(cPinArray[i]);
    //divide sensordata between to arrays
    byte aDiv = i>>2;      //i>>2 == i/4 
    //invert becease of bite order
    byte aMod = 3 - (i&3); //i&3 == i%4

    //if magnet is in range
    if(sensorData[i] > cSensTresh){
      sensorState[aDiv][aMod] = 0;
    }else{
      sensorState[aDiv][aMod] = 1;
    }//if
    
  }//for
  
  /*convert array of sensorstates to one byte
  shift data to the right [10010000]>>4 == [00001001]
  this is done so the number is less then 128
  in max these bytes will be joined together again
  */
  sData1 =  bitsToByte(sensorState[0])>>4;
  sData2 =  bitsToByte(sensorState[1])>>4;  
  static byte oldData1 = sData1;
  static byte oldData2 = sData2;

  //if value changes send serial data
  if(sData1 != oldData1 || sData2 != oldData2){
    //send teensy id
    Serial1.write(200+cTeensyId);   
    //send data from this teensy
    Serial1.write(sData1);
    Serial1.write(sData2);
    oldData1 = sData1;
    oldData2 = sData2;
  }

  //check if other teensys have data to send
  checkOtherTeensys();
    
}//loop

void checkOtherTeensys(){
  //if there is serial data availible
  if (Serial1.available() > 0) {
    //pass data trough
    byte incomingbyte = Serial1.read();
    Serial1.write(incomingbyte);
  }//if
}//checkData

void fakeOtherTeensys(){
  for(int i = 0; i < cTeensyAmnt; i++){
    Serial1.write(i+200); 
    Serial1.write(0);
    Serial1.write(i);        
  }
}//void

//function for converting bytes to bits
void byteToBits(byte b, bool * bArray){

  for(int i = 0; i < 8; i++){
    bArray[i] = ((128 >> i) & b) >> (7-i);
  }//for

}//byteToBits

//function for converting an array of bits to a byte
byte bitsToByte(bool * bitArray){

  byte b = 0;
  for(int i = 0; i < 8; i++){
    b = (b << 1) | (bitArray[i] & 1);
  }//for

  return b;

}//bitsToByte

