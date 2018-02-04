#define DEBUG

/*#define UP 1
#define DOWN 2
#define OFF 3
#define ON 4

#define R 5
#define G 6
#define B 7
#define W 8

#define R1 9
#define G1 10
#define B1 11
#define FLASH 12

#define R2 13
#define G2 14
#define B2 15
#define STROBE 16

#define R3 17
#define G3 18
#define B3 19
#define FADE 20

#define R4 21
#define G4 22
#define B4 23
#define SMOOTH 24*/


#include <Wire.h>
#include <JBus.h>
#include "LEDStrip.h"
#include "BrightnessController.h"
#include "ColorController.h"

#define LED_PIN 2
#define TURN_OFF_PIN 3
#define DATA_PIN 4
#define REQUEST_PIN 5

#define numberOfSignals 24

JBus jBus(REQUEST_PIN,DATA_PIN);
LEDStrip ledStrip(LED_PIN);
ColorController colorController(&ledStrip, 2);
BrightnessController brightnessController(&ledStrip, 12);

//Function Prototypes
int getData();

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Debugging Mode active");
#endif

ledStrip.begin();

pinMode(TURN_OFF_PIN, INPUT);
if(digitalRead(TURN_OFF_PIN) == LOW){
  brightnessController.turnOn();
}
else{
  brightnessController.turnOnRunning();
}

}

void loop() {

  uint8_t input = 0;
 if(jBus.requestAvailable()){
  input = jBus.read();
 }
  if (input > 0) {
    Serial.print("Input: ");
    Serial.println(input);
    setLights(input);
  }

  colorController.update();
  brightnessController.update();
}

void setLights(uint8_t input) {

  switch (input) {
    case 1: brightnessController.up();
    break;

    
    case 2: brightnessController.down();
    break;
    
    case 4: brightnessController.turnOn();
      break;

    case 3: brightnessController.turnOff();
      break;

      case 12: 
      break;
      case 16: brightnessController.startAcelerationMode();
      break;
       case 20: colorController.startColorFading();
      break;
       case 24: colorController.startRainbow();
      break;

    case 5: colorController.changeColorTo(ledStrip.Color(255,0,0));
      break;
    case 6: colorController.changeColorTo(ledStrip.Color(255,0,0));
      break;
    case 7: colorController.changeColorTo(ledStrip.Color(255,0,0));
      break;
    case 8: colorController.changeColorTo(ledStrip.Color(255,255,255));
      break;

    case 9:                          
      break;
    case 10:  
      break;
    case 11:   
      break;

    case 13: 
      break;
    case 14: 
      break;
    case 15: 
      break;

    case 17: 
      break;
    case 18: 
      break;
    case 19:
      break;

    case 21: 
      break;
    case 22: 
      break;
    case 23: 
      break;

    default: break;

  }
}
