/** \brief This sketch is part of the Smart Footwell Light Project.
 *  It is supposed to run on an ATMega type Arduino that acts as an LED controller for a strip of WS2812B RGB LED's.
 *  
 *  Two pins of the Arduino are attached to a Digispark that is equipped with an IR Receiver and handles IR input.
 *  It translates the received Signals into the according button numbers and sends them over to this board using the two wire JBus interface.
 *  This two-MCU-solution enables me to work around the issue of not being able to receive IR signals and update RGB LED's using the NeoPixel library.
 *  
 *  The Arduino is also attached to an I2C acceleration sensor that might be used for Light effects in the future. 
 *  
 *  To see the whole project head over to \link Instructables http://www.instructables.com/member/Basement+Engineering/ \endlink
 *  or \link Youtube https://www.youtube.com/channel/UCOITWYehgOXzxSZx-iEaULQ \endlink
 *  
 * \version 1.0 \date 10.04.2018
 * \author J. Kettler aka. Basement Engineering
 */

/** IR Remote Signals and coresponding Buttons
  //first row
  0xF700FF, // -> 1. UP 
  0xF7807F, // -> 2. DOWN
  0xF740BF, // -> 3. OFF
  0xF7C03F, // -> 4. ON
  //second row
  0xF720DF, // -> 5. Red
  0xF7A05F, // -> 6. Green
  0xF7609F, // -> 7. Blue
  0xF7E01F, // -> 8. White
  //third row
  0xF710EF, // -> 9. Red1
  0xF7906F, // -> 10. Green1
  0xF750AF, // -> 11. Blue1
  0xF7D02F, // -> 12. Flash
  //fifth row
  0xF730CF, // -> 13. Red2
  0xF7B04F, // -> 14. Green2
  0xF7708F, // -> 15. Blue2
  0xF7F00F, // -> 16. Strobe
  //sixth row
  0xF708F7, // -> 17. Red3
  0xF78877, // -> 18. Green3
  0xF748B7, // -> 19. Blue3
  0xF7C837, // -> 20. Fade
  //seventh row
  0xF728D7, // -> 21. Red4
  0xF7A857, // -> 22. Green4
  0xF76897, // -> 23. Blue4
  0xF7E817, // -> 24. Smooth

  0xFFFFFFFF, // -> 25. Repeat Message
*/
const int numberOfSignals = 24;

//Classes
#include <JBus.h>
#include <Timer.h>

#include "BrightnessController.h"
#include "ColorController.h"
#include "LEDStrip.h"

//Pin Declarations
const int LED_PIN = 2;
const int REQUEST_PIN = 3;
const int DATA_PIN = 4;
const int TURN_OFF_PIN = 5; //lamp Voltage lower than ~ 2/3 off VBatt -> HIGH 

//Global Objects
JBus jBus(REQUEST_PIN,DATA_PIN);
Timer turnOnTimer(10000, false);
LEDStrip ledStrip(LED_PIN);
ColorController colorController(&ledStrip, 2);
BrightnessController brightnessController(&ledStrip, 12);

//Function Prototypes
void setLights(uint8_t input);
void startTurnOnAnimation(void);
void processUserInput();

#define DEBUG

void setup() {
ledStrip.begin();

pinMode(TURN_OFF_PIN, INPUT);

startTurnOnAnimation();
turnOnTimer.start();

#ifdef DEBUG
Serial.begin(9600);
Serial.println("Ready");
#endif
}

void loop() {
  
  if(jBus.requestAvailable() && turnOnTimer.isFinished())
  {
    processUserInput();
  }

  colorController.update();
  brightnessController.update();

  if(digitalRead(TURN_OFF_PIN) == LOW && turnOnTimer.isFinished())
  {
  brightnessController.turnOff();
}
}

void processUserInput()
{
Serial.println("Request Available ");  
  bool success = false;
  byte input = jBus.read(success);
  if(success)
  {
    #ifdef DEBUG
    Serial.println("Successfully received a message");
    #endif
    if (input > 0) {
    #ifdef DEBUG
    Serial.print("Input: ");
    Serial.println(input);
    #endif
    setLights(input);
  }
  else
  {
    #ifdef DEBUG
   Serial.print("Unknown Input: ");
    Serial.println(input); 
    #endif
  }
  
  }
  else
  {
    Serial.println("Reading was unsuccessfull ");  
  }
  }
  
void startTurnOnAnimation(void)
{
if(digitalRead(TURN_OFF_PIN) == LOW)
{
  brightnessController.turnOn();
}
else
{
  brightnessController.turnOnRunning();
}
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

      case 12: colorController.saveSettings();
      break;
      case 16: brightnessController.startAcelerationMode();
      break;
       case 20: colorController.startColorFading();
      break;
       case 24: colorController.startRainbow();
      break;

    case 5: colorController.changeColorTo(ledStrip.Color(255,0,0));
      break;
    case 6: colorController.changeColorTo(ledStrip.Color(0,255,0));
      break;
    case 7: colorController.changeColorTo(ledStrip.Color(0,0,255));
      break;
    case 8: colorController.changeColorTo(ledStrip.Color(200,200,255));
      break;

    case 9: colorController.changeColorTo(ledStrip.Color(255,50,0));                          
      break;
    case 10: colorController.changeColorTo(ledStrip.Color(0,255,50));  
      break;
    case 11: colorController.changeColorTo(ledStrip.Color(50,0,255));  
      break;

    case 13: colorController.changeColorTo(ledStrip.Color(255,100,0));
      break;
    case 14: colorController.changeColorTo(ledStrip.Color(0,255,100));
      break;
    case 15: colorController.changeColorTo(ledStrip.Color(100,0,255));
      break;

    case 17: colorController.changeColorTo(ledStrip.Color(255,160,0));
      break;
    case 18: colorController.changeColorTo(ledStrip.Color(0,255,160));
      break;
    case 19: colorController.changeColorTo(ledStrip.Color(160,0,255));
      break;

    case 21: colorController.changeColorTo(ledStrip.Color(255,200,0));
      break;
    case 22: colorController.changeColorTo(ledStrip.Color(0,255,200));
      break;
    case 23: colorController.changeColorTo(ledStrip.Color(200,0,255));
      break;

    default: break;

  }
}
