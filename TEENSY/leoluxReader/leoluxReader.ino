#define cTeensyId 15
#define cPinAmnt  8
#define cSensTresh 500
#define cTeensyAmnt 16
#define cByteAmnt 32

const byte cPinArray[cPinAmnt] = {A9,A7,A5,A3,A1,A0,A2,A4};
byte serialReceive[30];

int  sensorData[cPinAmnt];
bool sensorState[2][cPinAmnt];

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

  for(int i = 0; i < 30; i++){
    serialReceive[i] = 56;
  }

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
  in the master code these bytes will be joined together again
  */
  byte sData1 =  bitsToByte(sensorState[0])>>4;
  byte sData2 =  bitsToByte(sensorState[1])>>4;  

  //send first bit bit
  Serial1.write(255);
     
  //send data from this teensy
  Serial1.write(sData1);
  Serial1.write(sData2);
    
}//loop

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

void testSerial(){
  static int x;
  Serial1.write(255);
  for(int i = 0; i < cByteAmnt; i++){
    Serial1.write(i+x%128);
  }//for  

  x=x+1%128;
}//testSerial

