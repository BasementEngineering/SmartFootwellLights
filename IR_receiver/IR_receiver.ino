//#define DEBUG
#define RECEIVER_PIN 2

#define REQUEST_PIN 0
#define DATA_PIN 1

#include <JBus.h>

JBus jBus(REQUEST_PIN, DATA_PIN);

//functions
byte getInput(void);
void printBinaryValue(byte value);

void sendData(int data);

void setup() {
  pinMode(RECEIVER_PIN, INPUT);
  
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("ready");
#endif
}

void loop() {
  
  int input = getInput();

  if (input) {
#ifdef DEBUG
    Serial.println(input);
#endif
    sendData(input);
  }

}

byte decodeNECMessage(){
  byte byteValue = 0;
  byte inversedByteValue = 0;
  for (int i = 0; i < 32; i++) {
      unsigned long bitLength = pulseIn(RECEIVER_PIN, HIGH);

      if (bitLength > 2000) {//if there is only a start Bit and no further bits follow the button is held down
        return 100;
      }
      //reading the first command Byte
      if (i > 15 && i < 24) {
        if (bitLength > 1000) {
          byteValue = byteValue + (1 << (i - 16));
        }
      }
      //reading the inversed command Byte
      else if (i > 23 && i < 32) {
        if (bitLength > 1000) {
          inversedByteValue = inversedByteValue + (1 << (i - 24));
        }
      }
}
   //Validating the received Signal by comparing the two Bytes
    if (byteValue == ~inversedByteValue) {
      return byteValue;
    }
    return -1;
}

//Receiving IR Signals from the IR Remote
byte getInput(void) {

  byte result = -1;

  unsigned long startBitLength = pulseIn(RECEIVER_PIN, LOW);

  if (startBitLength > 8800) {
    result = decodeNECMessage();
    }
  return (result + 1);
}

void sendData(int data){
  if(data > 0){
    jBus.write(data);
  }
}

/*void printBinaryValue(byte value) {
  for (int i = 0; i < 8; i++) {
    byte binaryMask = B00000000;
    binaryMask |= ((byte)(1 << i));
    byte isolatedDigit = value & ((byte)(1 << i));
    Serial.print(isolatedDigit && binaryMask);
  }
  Serial.println();
}
*/
